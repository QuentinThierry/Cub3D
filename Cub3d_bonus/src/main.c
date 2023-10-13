/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 17:11:21 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

long tot_fps = 0;
long nb_fps = 0;

int	on_update(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	struct timespec			time;
	long					fps;

	if (last_time.tv_sec == 0)
	{
		game->last_time = &last_time;
		clock_gettime(CLOCK_REALTIME, &last_time);
	}
	clock_gettime(CLOCK_REALTIME, &time);
	update_sounds(game->music_array);
	game->time = time_to_long(&time);
	if ((game->map[(int)game->player->f_real_pos.y][(int)game->player->f_real_pos.x].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE)
		take_object(game, game->player, &game->map[(int)game->player->f_real_pos.y][(int)game->player->f_real_pos.x], game->music_array);
	player_move(game, game->player, game->delta_time, game->map);
	if (game->player->angle + game->player->angle >= 360)
		game->player->angle = game->player->angle - 360;
	if (game->player->angle + game->player->angle < 0)
		game->player->angle = game->player->angle + 360;
	update_doors(game->door_array, game->nb_doors, game->delta_time, game->map);
	raycasting(game);
	if (game->player->has_item == true)
		draw_hand_item(game, game->player);
	zoom_hook_handle(game->minimap, game->delta_time);
	draw_minimap(game);
	print_subtitle(game, game->music_array[1].map_cell);
	
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);

	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	fps = (long)(1.0 / game->delta_time);
	tot_fps += fps;
	nb_fps++;
	if ((nb_fps % 50) == 0)
		printf("fps : %ld\n", fps);
	last_time = cur_time;
	return (0);
}
void	move_mouse(t_game *game);

int main(int argc, char **argv)
{
	t_game	game;
	bool	error;
	
	game = (t_game){0};
	if (argc != 2)
		return (print_error("Invalid number of arguments\n", 1), 1);
	if (WIN_X < 100 || WIN_Y < 100)
		return (print_error("Invalid window size\n", 1), 1);
	if (!init_game(&game, argv[1]))
		return (1);
	if (!load_image_tab(&game, &error))
	{
		if (error == true)
			print_error(NULL, 0);
		return (ft_close(&game), 1);
	}
	free_filename(&game);
	ft_memcpy(game.image->addr, game.loading_screen->background->addr,
		WIN_X * WIN_Y * 4);
	free_loading_screen(&game);
	exit_door_no_receptacle(game.exit, game.total_receptacle, game.tab_images);
	PlayMusicStream(game.music_array[0].music);
	move_mouse(&game);
	set_pause_menu_mode(&game);
	mlx_loop(game.mlx_ptr);
	return (0);
}
