/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_loop_hook.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 19:50:10 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/13 19:50:34 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const double	g_max_fps_frequence = 1. / MAX_MENU_FPS;

static void	wait_fps(struct timespec cur_time,
		register struct timespec last_time)
{
	register double	delay;

	delay = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	while (delay <= g_max_fps_frequence - 0.000001)
	{
		clock_gettime(CLOCK_REALTIME, &cur_time);
		delay = (cur_time.tv_sec - last_time.tv_sec
				+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	}
}

int	menu_loop_hook(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	t_menu					*menu;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	menu = game->menu;
	if (menu->state != CHOOSING_KEY_MENU)
		ft_memcpy(game->menu->image->addr,
			game->menu->background_image->addr, WIN_X * WIN_Y * 4);
	if (menu->state == PAUSE_MENU)
		draw_pause_menu(game, &menu->pause_menu);
	if (menu->state == OPTION_MENU)
		draw_option_menu(game, &menu->option_menu);
	UpdateMusicStream(game->music_array[0].music);
	SetMasterVolume(game->menu->option_menu.slider_sound.percent);
	mlx_put_image_to_window(game->mlx_ptr, game->win, menu->image->img, 0, 0);
	clock_gettime(CLOCK_REALTIME, &cur_time);
	wait_fps(cur_time, last_time);
	last_time = cur_time;
	return (0);
}
