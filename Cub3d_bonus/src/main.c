/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/13 16:27:59 by jvigny           ###   ########.fr       */
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

	// usleep(100000);
	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	clock_gettime(CLOCK_REALTIME, &time);
	game->time = time_to_long(&time);
	if (game->player->angle + game->player->angle >= 360)
		game->player->angle = game->player->angle - 360;
	if (game->player->angle + game->player->angle < 0)
		game->player->angle = game->player->angle + 360;
	player_move(game->player, game->delta_time, game->map);
	update_doors(game->door_array, game->nb_doors, game->time);
	raycasting(game);
	zoom_hook_handle(game->minimap, game->delta_time);
	draw_minimap(game);
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

int main(int argc, char **argv)
{
	t_game	game;

	game = (t_game){0};
	if (argc != 2)
		return (printf("Error : Invalid nubmber of arguments\n"), 1);
	if (WIN_X < 100 || WIN_Y < 100)
		return (printf("Error : Invalid Window size\n"), 1);
	game.filename = ft_calloc(6, sizeof(t_texture));
	if (game.filename == NULL)
		return (perror("Error"), 1);
	game.nb_file = 6;
	game.dist_tab = ft_calloc(WIN_X, sizeof(float));
	if (game.filename == NULL)
		return (free(game.filename), perror("Error"), 1);
	if (!parse_file(argv[1], &game))
		return (1);
	printf_texture(&game);
	if (!check_map(&game))
		return (1);
	if (init_mlx(&game) == -1)
		return (perror("Error"), ft_close(&game), 1);
	if (!loading_screen(&game))
		return (perror("Error"), ft_close(&game), 1);
	if (!load_image_tab(&game))
		return (perror("Error"), ft_close(&game), 1);
	free_filename(&game);
	free_loading_screen(&game);
	game.constants = (float[5]){(WIN_X / 2.) / tan((FOV / 2.) * TO_RADIAN)
					, tanf((FOV / 2.0) * TO_RADIAN), cos((FOV / 2.0) * TO_RADIAN)};
	init_minimap(&game);
	init_mouse(&game);
	mlx_do_key_autorepeatoff(game.mlx_ptr);
	mlx_hook(game.win, 2, (1L<<0), key_press_hook, &game);
	mlx_hook(game.win, 3, (1L<<1), key_release_hook, &game);
	mlx_hook(game.win, 17, (1L << 8), ft_close, &game);
	mlx_hook(game.win, 6, (1L << 6) | (1L << 2) , mouse_hook, &game);
	mlx_hook(game.win, 8, (1L << 5), mouse_leave, &game);
	mlx_mouse_hook(game.win, mouse_click, &game);
	mlx_loop_hook(game.mlx_ptr, on_update, &game);
	sleep(1);
	mlx_loop(game.mlx_ptr);
	return (0);
}
