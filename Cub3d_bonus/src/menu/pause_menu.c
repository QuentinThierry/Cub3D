/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pause_menu.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:57:18 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/13 19:44:18 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	adjust_animation_time(t_game *game)
{
	int				i;
	int				j;
	int				k;
	long			time_diff;
	struct timespec	time_now;

	clock_gettime(CLOCK_REALTIME, &time_now);
	time_diff = time_to_long(&time_now) - game->menu->time_start_menu;
	i = 0;
	while (i < game->map_size.y)
	{
		j = 0;
		while (j < game->map_size.x)
		{
			k = -1;
			while (++k < 6)
				if (game->map[i][j].type != 0
					&& game->map[i][j].sprite[k].index != -1
					&& game->map[i][j].sprite[k].frame != -1)
					game->map[i][j].sprite[k].time += time_diff;
			j++;
		}
		i++;
	}
	*game->last_time = time_now;
}

void	resume_menu(t_game *game, t_menu *menu)
{
	if (menu->state == PAUSE_MENU)
	{
		game->fov = MIN_FOV + (MAX_FOV - MIN_FOV)
			* game->menu->option_menu.slider_fov.percent;
		game->constants[0] = (WIN_X / 2.) / tan((game->fov / 2.) * TO_RADIAN);
		game->constants[1] = tanf((game->fov / 2.0) * TO_RADIAN);
		game->constants[2] = cos((game->fov / 2.0) * TO_RADIAN);
		SetMasterVolume(game->menu->option_menu.slider_sound.percent);
		mlx_hook(game->win, 2, (1L << 0), (void *)key_press_hook, game);
		mlx_hook(game->win, 3, (1L << 1), (void *)key_release_hook, game);
		mlx_hook(game->win, 5, (1L << 3), NULL, NULL);
		mlx_hook(game->win, 6, (1L << 6), mouse_hook, game);
		mlx_hook(game->win, 8, (1L << 5), mouse_leave, game);
		mlx_mouse_hook(game->win, mouse_click, game);
		mlx_loop_hook(game->mlx_ptr, on_update, game);
		if (ft_strcmp(game->menu->pause_menu.play_button.text, "RESUME") == 0)
			adjust_animation_time(game);
		else
			game->menu->pause_menu.play_button.text = "RESUME";
		init_mouse(game);
	}
	else if (menu->state == OPTION_MENU)
		menu->state = PAUSE_MENU;
}

void	set_pause_menu_mode(t_game *game)
{
	struct timespec	time_start;

	clock_gettime(CLOCK_REALTIME, &time_start);
	game->menu->time_start_menu = time_to_long(&time_start);
	mlx_hook(game->win, 2, (1L << 0), (void *)menu_key_press, game);
	mlx_hook(game->win, 3, (1L << 1), (void *)menu_key_release, game);
	mlx_hook(game->win, 4, (1L << 2), (void *)menu_mouse_down_hook, game);
	mlx_hook(game->win, 5, (1L << 3), (void *)menu_mouse_up_hook, game);
	mlx_hook(game->win, 8, (1L << 5), NULL, NULL);
	mlx_hook(game->win, 6, (1L << 6), NULL, NULL);
	mlx_mouse_show(game->mlx_ptr, game->win);
	mlx_loop_hook(game->mlx_ptr, menu_loop_hook, game);
	apply_menu_dark_filter(game->image, DARK_PERCENT_PAUSE);
	blur_image(game->menu->background_image,
		game->image, game->menu->h_rgb_blur_buffer,
		game->menu->v_rgb_blur_buffer);
	game->menu->state = PAUSE_MENU;
}

void	draw_pause_menu(t_game *game, t_pause_menu *pause_menu)
{
	int	x_mouse;
	int	y_mouse;

	mlx_mouse_get_pos(game->mlx_ptr, game->win, &x_mouse, &y_mouse);
	check_mouse_is_in_button(&pause_menu->play_button, x_mouse, y_mouse);
	draw_button(&pause_menu->play_button, game->menu->image);
	draw_text_in_button(game, game->menu->image, &pause_menu->play_button);
	check_mouse_is_in_button(&pause_menu->option_button, x_mouse, y_mouse);
	draw_button(&pause_menu->option_button, game->menu->image);
	draw_text_in_button(game, game->menu->image, &pause_menu->option_button);
	check_mouse_is_in_button(&pause_menu->quit_button, x_mouse, y_mouse);
	draw_button(&pause_menu->quit_button, game->menu->image);
	draw_text_in_button(game, game->menu->image, &pause_menu->quit_button);
}
