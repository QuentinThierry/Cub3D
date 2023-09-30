/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:16:58 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/30 16:52:10 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const double	max_fps_frequence = 1. / MAX_MENU_FPS;

void	choose_key_hook(t_keybind key, t_game *game)
{
	int			i;
	static bool	has_drawn_key_used;

	if (key == XK_Escape)
		ft_close(game);
	i = 0;
	while (i < NB_OPTIONS_BUTTONS)
	{
		if (key == game->keybinds[i]
			&& i != game->menu->option_menu.pressed_button)
		{
			if (!has_drawn_key_used)
				draw_centered_text_at_y(game, game->menu->image,
					WIN_Y / 2 + WIN_Y / 12, "Key is already used.");
			has_drawn_key_used = true;
			return ;
		}
		i++;
	}
	has_drawn_key_used = false;
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
		if (x >= pause_menu->option_button.pos.x && x <= pause_menu->option_button.pos.x + pause_menu->option_button.size.x
				&& y >= pause_menu->option_button.pos.y && y <= pause_menu->option_button.pos.y + pause_menu->option_button.size.y)
			game->menu->state = OPTION_MENU;
		else if (x >= pause_menu->play_button.pos.x && x <= pause_menu->play_button.pos.x + pause_menu->play_button.size.x
				&& y >= pause_menu->play_button.pos.y && y <= pause_menu->play_button.pos.y + pause_menu->play_button.size.y)
			resume_menu(game, game->menu);
		else if (x >= pause_menu->quit_button.pos.x && x <= pause_menu->quit_button.pos.x + pause_menu->quit_button.size.x
				&& y >= pause_menu->quit_button.pos.y && y <= pause_menu->quit_button.pos.y + pause_menu->quit_button.size.y)
		ft_close(game);
	}
	else if (game->menu->state == OPTION_MENU)
	{
		i = 0;
		while (i < NB_OPTIONS_BUTTONS)
		{
			button = &opt_menu->buttons[i];
			if (x >= button->pos.x && x <= button->pos.x + button->size.x
				&& y >= button->pos.y && y <= button->pos.y + button->size.y)
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
		if (x >= exit_button->pos.x && x <= exit_button->pos.x
			+ exit_button->size.x && y >= exit_button->pos.y
			&& y < exit_button->pos.y + exit_button->size.y)
				resume_menu(game, game->menu);
		if (x >= opt_menu->slider_fov.pos.x && x <= opt_menu->slider_fov.pos.x
			+ opt_menu->slider_fov.size.x + opt_menu->slider_fov.vert_image->size.x / 2
			&& y >= opt_menu->slider_fov.pos.y
			&& y <= opt_menu->slider_fov.pos.y + opt_menu->slider_fov.size.y)
			opt_menu->pressed_slider_ref = &opt_menu->slider_fov;
		if (x >= opt_menu->slider_sound.pos.x && x <= opt_menu->slider_sound.pos.x
			+ opt_menu->slider_sound.size.x + opt_menu->slider_sound.vert_image->size.x / 2
			&& y >= opt_menu->slider_sound.pos.y
			&& y <= opt_menu->slider_sound.pos.y + opt_menu->slider_sound.size.y)
			opt_menu->pressed_slider_ref = &opt_menu->slider_sound;
	}
}

void	menu_mouse_up_hook(int mouse_button, int x, int y, t_game *game)
{
	void			*ref;

	(void)x;
	(void)y;
	if (mouse_button != 1)
		return ;
	if (game->menu->state == OPTION_MENU)
	{
		game->menu->option_menu.pressed_slider_ref = NULL;
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
	register double			delay;
	t_menu					*menu;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	menu = game->menu;
	UpdateMusicStream(game->music_array[0].music);
	if (menu->state != CHOOSING_KEY_MENU)
		ft_memcpy(game->menu->image->addr,
				game->menu->background_image->addr, WIN_X * WIN_Y * 4);
	if (menu->state == PAUSE_MENU)
		draw_pause_menu(game, &menu->pause_menu);
	if (menu->state == OPTION_MENU)
		draw_option_menu(game, &menu->option_menu);
	SetMasterVolume(game->menu->option_menu.slider_sound.percent);
	mlx_put_image_to_window(game->mlx_ptr, game->win, menu->image->img, 0, 0);
	clock_gettime(CLOCK_REALTIME, &cur_time);
	delay = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	while (delay <= max_fps_frequence - 0.000001)
	{
		clock_gettime(CLOCK_REALTIME, &cur_time);
		delay = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	}
	last_time = cur_time;
	return (0);
}