/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/27 00:50:19 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

#if THREED
int	on_update(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;

	bzero(game->image->addr, WIN_X * WIN_Y * 4);
	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);

	ft_mouv(game->player, game->delta_time);
	// printf("player x:%f y :%f\n", game->player->f_pos.x,game->player->f_pos.y);
	raycasting(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	// usleep(1000000000);
	
	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - last_time.tv_sec + (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	// printf("%ld\n", (long)(1.0 / game->delta_time));
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

int main(void)
{
	t_game		game;

	putenv("PULSE_LATENCY_MSEC=60");
	game.maps = parse_map("maps/test1.cub", &game.map_size);
	if (game.maps == NULL)
		return (ft_close(&game), perror("Error"), 1);
	if (init_mlx(&game) == -1)
		return (ft_close(&game), perror("Error"), 1);
	game.player = find_player(game.maps);
	if (game.player == NULL)
		return (ft_close(&game), perror("Error"), 1);
	game.filename = ft_calloc(4, sizeof(char *));
	game.filename[0] = strdup("assets/blue.xpm");
	game.filename[1] = strdup("assets/flower_yellow.xpm");
	game.filename[2] = strdup("assets/wood_light.xpm");
	game.filename[3] = strdup("assets/chatmignon.xpm");
	if (load_image(&game) == -1)
		return (ft_close(&game), perror("Error"), 1);
	// quadrillage(&game);
	// raycasting(&game);
	// mlx_put_image_to_window(game.mlx_ptr, game.win, game.image->img, 0, 0);

	mlx_hook(game.win, 02, (1L<<0), key_press_hook, &game);
	mlx_hook(game.win, 03, (1L<<1), key_release_hook, game.player);
	mlx_loop_hook(game.mlx_ptr, on_update, &game);
	mlx_loop(game.mlx_ptr);
	return (0);
}
