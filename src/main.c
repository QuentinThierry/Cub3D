/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/30 23:14:52 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

long tot_fps = 0;
long nb_fps = 0;

#if THREED
int	on_update(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	long					fps;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	


	
	if (game->player->angle + game->player->angle >= 360)
		game->player->angle = game->player->angle - 360;
	if (game->player->angle + game->player->angle < 0)
		game->player->angle = game->player->angle + 360;
	player_move(game->player, game->delta_time);
	raycasting(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	


	
	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - last_time.tv_sec + (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	fps = (long)(1.0 / game->delta_time);
	tot_fps += fps;
	nb_fps++;
	if ((nb_fps % 50) == 0)
		printf("fps : %ld\n", fps);
	last_time = cur_time;
	return (0);
}

#else
int	on_update(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;

	bzero(game->image->addr, game->map_size.x * game->map_size.y * 4 * CHUNK_SIZE * CHUNK_SIZE);
	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
		
	ft_mouv(game->player, game->delta_time);
	quadrillage(game);
	raycasting_2d(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	// usleep(1000000000);
	
	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - last_time.tv_sec + (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	// printf("%ld\n", (long)(1.0 / game->delta_time));
	last_time = cur_time;
	return (0);
}
#endif

int main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (printf("Error : args\n"), 1);
	game.filename = ft_calloc(4, sizeof(char *));
	if (parse_file(argv[1], &game) == -1)
		return (1);
	printf_texture(&game);
	// if (init_mlx(&game) == -1)
	// 	return (ft_close(&game), perror("Error"), 1);
	// if (load_image(&game) == -1)
	// 	return (ft_close(&game), perror("Error"), 1); // ((WIN_X / 2.0) / (tanf((FOV / 2.0) * TO_RADIAN))
	// game.constants = (float[5]){(WIN_X / 2.0) / (tanf((FOV / 2.0) * TO_RADIAN))};
	// game.player->angle = 0;

	// mlx_hook(game.win, 02, (1L<<0), key_press_hook, &game);
	// mlx_hook(game.win, 03, (1L<<1), key_release_hook, game.player);
	// mlx_loop_hook(game.mlx_ptr, on_update, &game);
	// mlx_loop(game.mlx_ptr);
	return (0);
}
