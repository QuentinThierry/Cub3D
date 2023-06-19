/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/19 19:52:40 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_player	find_player(char **maps)
{
	t_player	player;
	t_vector2	index;

	index.y = 0;
	index.x = 0;
	while(maps[index.y] != NULL)
	{
		index.x = 0;
		while(maps[index.y][index.x] != '\0')
		{	
			if (maps[index.y][index.x] == 'N')
			{
				player.angle = 0;
				player.pos.x = index.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player.pos.y = index.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player.f_pos.x = index.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player.f_pos.y = index.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player.f_real_pos.x = index.x + 1 / 2.0;
				player.f_real_pos.y = index.y + 1 / 2.0;
			}
			index.x++;
		}
		index.y++;
	}
	return (player);
}

int	on_update(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	// long					fps;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
		
	quadrillage(game);
	raycasting(game);
	// get_wall_dist(game, game->player.angle);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->ptr, 0, 0);
	// usleep(100000);
	// mlx_clear_window(game->mlx_ptr, game->win);
	
	clock_gettime(CLOCK_REALTIME, &cur_time);
	// fps = 1.F / (cur_time.tv_sec - last_time.tv_sec + (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	// printf("%ld\n", fps);
	last_time = cur_time;
	return (0);
}

int sound(t_game *game);

int main(void)
{
	char		*maps[20] = {0};
	t_player	player;
	t_image		img;
	t_game		game;
	int y;
	int fd;

	putenv("PULSE_LATENCY_MSEC=60");

	game.image = &img;
	game.mlx_ptr = mlx_init();
	game.win = mlx_new_window(game.mlx_ptr, WIN_X, WIN_Y, "cub3d");
	game.image->ptr = mlx_new_image(game.mlx_ptr, WIN_X, WIN_Y);
	game.image->data = mlx_get_data_addr(game.image->ptr,
		&game.image->bpp, &game.image->size_line, &game.image->endian);

	fd = open("maps/test.cub", O_RDONLY);
	y = 0;
	maps[y] = get_next_line(fd);
	while (maps[y] != NULL)
	{
		maps[y][strlen(maps[y]) - 1] = 0;
		y++;
		maps[y] = get_next_line(fd);
	}
	close(fd);
	player = find_player(maps);
	// printf("%d, %d\n", player.pos.x, player.pos.y);
	// print_map(maps);
	game.player = &player;
	game.maps = maps;
	// sound(&game);
	quadrillage(&game);
	mlx_loop_hook(game.mlx_ptr, on_update, &game);
	mlx_key_hook(game.win, key_press_hook, &game);
	mlx_loop(game.mlx_ptr);
	return (0);
}
