/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:16:58 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/23 22:09:23 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"


void	choose_key_hook(t_keybind key, t_game *game)
{
	int	i;

	if (key == XK_Escape)
		ft_close(game);
	i = 0;
	while (i < NB_OPTIONS_BUTTONS)
	{
		if (key == game->keybinds[i] && i != game->menu->option_menu.pressed_button)
		{
			draw_centered_text_at_y(game, game->menu->image,
				WIN_Y / 2 + 100, "Key is already used.");
			return ;
		}
		i++;
	}
	game->menu->option_menu.buttons[game->menu->option_menu.pressed_button].text
		= get_key_str(key);
	game->menu->state = OPTION_MENU;
	game->keybinds[game->menu->option_menu.pressed_button] = key;
	mlx_hook(game->win, 2, (1L << 0), (void *)menu_key_hook, game);
}

void	menu_mouse_down_hook(int mouse_button, int x, int y, t_game *game)
{
	t_option_menu	*opt_menu;
	t_pause_menu	*pause_menu;
	t_button		*button;
	t_button		*exit_button;
	int				i;

	if (mouse_button != 1)
		return ;
	opt_menu = &game->menu->option_menu;
	pause_menu = &game->menu->pause_menu;
	if (game->menu->state == PAUSE_MENU)
	{
		if (x > pause_menu->play_button.pos.x && x < pause_menu->play_button.pos.x + pause_menu->play_button.size.x
				&& y > pause_menu->play_button.pos.y && y < pause_menu->play_button.pos.y + pause_menu->play_button.size.y)
			resume_menu(game, game->menu);
		if (x > pause_menu->option_button.pos.x && x < pause_menu->option_button.pos.x + pause_menu->option_button.size.x
				&& y > pause_menu->option_button.pos.y && y < pause_menu->option_button.pos.y + pause_menu->option_button.size.y)
			game->menu->state = OPTION_MENU;
		if (x > pause_menu->quit_button.pos.x && x < pause_menu->quit_button.pos.x + pause_menu->quit_button.size.x
				&& y > pause_menu->quit_button.pos.y && y < pause_menu->quit_button.pos.y + pause_menu->quit_button.size.y)
		ft_close(game);
	}
	else if (game->menu->state == OPTION_MENU)
	{
		i = 0;
		while (i < NB_OPTIONS_BUTTONS)
		{
			button = &opt_menu->buttons[i];
			if (x > button->pos.x && x < button->pos.x + button->size.x
				&& y > button->pos.y && y < button->pos.y + button->size.y)
			{
				game->menu->state = CHOOSING_KEY_MENU;
				opt_menu->pressed_button = i;
				apply_menu_dark_filter(game->menu->image);
				draw_centered_text_at_y(game, game->menu->image, WIN_Y / 2. - game->size_letter.y / 2,
					KEY_TEXT_CHANGE);
				mlx_hook(game->win, 2, (1L << 0), (void *)choose_key_hook, game);
				break ;
			}
			i++;
		}
		exit_button = &opt_menu->exit_opt_button;
		if (x > exit_button->pos.x && x < exit_button->pos.x
			+ exit_button->size.x && y > exit_button->pos.y
			&& y < exit_button->pos.y + exit_button->size.y)
			resume_menu(game, game->menu);
		if (x > opt_menu->slider_fov.pos.x && x < opt_menu->slider_fov.pos.x
			+ opt_menu->slider_fov.size.x && y > opt_menu->slider_fov.pos.y
			&& y < opt_menu->slider_fov.pos.y + opt_menu->slider_fov.size.y)
			opt_menu->pressed_slider_ref = &opt_menu->slider_fov;
	}
}

void	menu_mouse_up_hook(int mouse_button, int x, int y, t_game *game)
{
	void			*ref;
	t_option_menu	*opt_menu;

	printf("up\n");
	fflush(stdout);

	if (mouse_button != 1)
		return ;
	if (game->menu->state == OPTION_MENU)
	{
		opt_menu = &game->menu->option_menu;
		ref = opt_menu->pressed_slider_ref;
		if (ref == &opt_menu->slider_fov)
		{
			opt_menu->pressed_slider_ref = NULL;
		}
	}
}

void	menu_key_hook(t_keybind key, t_game *game)
{
	if (key == game->keybinds[e_key_pause])
		resume_menu(game, game->menu);
	if (key == XK_Escape)
		ft_close(game);
}

int	menu_loop_hook(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	struct timespec			time;
	long					fps;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	clock_gettime(CLOCK_REALTIME, &time);
	//////////

	
	t_menu	*menu;
	
	menu = game->menu;
	if (menu->state != CHOOSING_KEY_MENU)
		ft_memcpy(game->menu->image->addr,
				game->menu->background_image->addr, WIN_X * WIN_Y * 4);
	if (menu->state == PAUSE_MENU)
		draw_pause_menu(game, &menu->pause_menu);
	if (menu->state == OPTION_MENU)
		draw_option_menu(game, &menu->option_menu);
	mlx_put_image_to_window(game->mlx_ptr, game->win, menu->image->img, 0, 0);


	//////////
	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	fps = (long)(1.0 / game->delta_time);
	tot_fps += fps;
	nb_fps++;
	if ((nb_fps % 500) == 0)
		printf("fps : %ld\n", fps);
	last_time = cur_time;
	
	return (0);
}