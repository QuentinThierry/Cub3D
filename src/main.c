/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/20 12:45:18 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	on_update(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	long					fps;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
		
	quadrillage(game);
	raycasting(game);
	// get_wall_dist(game, game->player.angle);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	// usleep(1000000000);
	// mlx_clear_window(game->mlx_ptr, game->win);
	
	clock_gettime(CLOCK_REALTIME, &cur_time);
	// fps = 1.F / (cur_time.tv_sec - last_time.tv_sec + (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	// printf("%ld\n", fps);
	// last_time = cur_time;
	return (0);
}

int main(void)
{
	t_image		img_wall;
	t_game		game;

	putenv("PULSE_LATENCY_MSEC=60");
	if (init_mlx(&game) == -1)
		return (ft_close(&game), perror("Error"), 1);
	game.maps = parse_map("maps/test.cub");
	if (game.maps == NULL)
		return (ft_close(&game), perror("Error"), 1);
	game.player = find_player(game.maps);
	if (game.player == NULL)
		return (ft_close(&game), perror("Error"), 1);
	
	img_wall.img = mlx_xpm_file_to_image(game.mlx_ptr,"assets/smiley.xpm", &(img_wall.size.x), &(img_wall.size.y));
	img_wall.addr = mlx_get_data_addr(img_wall.img,
		&img_wall.bpp, &img_wall.size_line, &img_wall.endian);
	game.asset = &img_wall;

	// quadrillage(&game);
	// raycasting(&game);
	// mlx_put_image_to_window(game.mlx_ptr, game.win, game.image->img, 0, 0);

	mlx_loop_hook(game.mlx_ptr, on_update, &game);
	mlx_key_hook(game.win, key_press_hook, &game);
	mlx_loop(game.mlx_ptr);
	return (0);
}
