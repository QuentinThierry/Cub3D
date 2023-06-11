/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/12 00:45:42 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include <X11/X.h>

#define WIN_X 500
#define WIN_Y 500
#define CHUNK_SIZE 50
#define FOV 60

typedef	struct s_player{
	t_vector2	pos;
	float	angle;
	float	f_x;
	float	f_y;
}	t_player;

typedef struct s_image
{
	void	*ptr;
	char	*data;
	int		bpp;
	int		size_line;
	int		endian;
}	t_image;

typedef struct s_game
{
	void		*mlx_ptr;
	void		*win;
	char		**maps;
	t_image		image;
	t_player	player;
}	t_game;

void	print_map(char **maps)
{
	int y = 0;
	
	while(maps[y] != NULL)
	{
		printf("%s\n", maps[y]);
		y++;
	}
}

void	my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->data + (y * img->size_line + x * (img->bpp / 8));
	*(unsigned int*)dst = color;
}

// draw a vertical line on 'x' axis, from y1 up to y2 down
void	draw_vert(t_game *game, int x, int y1, int y2)
{
	int	y;

	y = y1;
	while (y < y2)
	{
		my_mlx_pixel_put(&game->image, x, y, 0xFF0000);
		y++;
	}
}

int	end_game(int key, t_game *game)
{
	(void)game;
	if (key == 65307 ) // esc
		exit(0);
	return (0);
}

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
				player.pos.x = index.x * CHUNK_SIZE;
				player.pos.y = index.y * CHUNK_SIZE;
			}
			index.x++;
		}
		index.y++;
	}
	return (player);
}

int	get_wall_dist(t_game *game, float angle)
{
	t_vector2	xstep;
	t_vector2	ystep;
	float		r_angle;

	angle = 0;
	r_angle = angle * M_PI / 180;
	game->player.pos.x = 210;
	game->player.pos.y = 120;
	ystep.y = game->player.pos.y % CHUNK_SIZE;
	ystep.x = ystep.y * (float)tan(r_angle);
	xstep.x = game->player.pos.x % CHUNK_SIZE;
	xstep.y = xstep.x / (float)tan(r_angle);

	int	x = 0;
	int	y = 0;
	
	x = game->player.pos.x;
	y = game->player.pos.y;

	int	x_cmp = game->player.pos.x - game->player.pos.x % CHUNK_SIZE;
	int	y_cmp = game->player.pos.y - game->player.pos.y % CHUNK_SIZE;

	while (true)
	{
		while (x > x_cmp) // increase y
		{
			if (game->player.pos.x == x && game->player.pos.y == y)
			{
				x -= ystep.x;
				y_cmp -= ystep.y;
			}
			else
			{
				x -= CHUNK_SIZE * (float)tan(r_angle);
				y_cmp -= CHUNK_SIZE;
			}
			if (game->maps[(y_cmp) / CHUNK_SIZE][(x) / CHUNK_SIZE] == '1')
			{
				game->maps[(y_cmp) / CHUNK_SIZE][(x) / CHUNK_SIZE] = 'A';
				return (0);
			}
		}
		while (y > y_cmp) // increase x
		{
			if (game->player.pos.x == x && game->player.pos.y == y)
			{
				x_cmp -= xstep.x;
				y -= xstep.y;
			}
			else
			{
				x_cmp -= CHUNK_SIZE;
				y -= CHUNK_SIZE / (float)tan(r_angle);
			}
			if (game->maps[(y) / CHUNK_SIZE][(x_cmp) / CHUNK_SIZE] == '1')
			{
				game->maps[(y) / CHUNK_SIZE][(x_cmp) / CHUNK_SIZE] = 'A';
				return (0);
			}
		}
	}

	return (0);
}

void	raycasting(t_game *game)
{
	int	x;
	int	angle;

	x = 0;
	while (x < WIN_X)
	{
		// angle = FOV / x;

		draw_vert(game, x, 10, 20);
		x++;
	}
}

int	on_update(t_game *game)
{
	raycasting(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image.ptr, 0, 0);
	usleep(100000);
	// bzero(game->image.data, 200 * 200 * 4);
	// mlx_clear_window(game->mlx_ptr, game->win);
	return (0);
}

int main(int argc, char **argv)
{
	char		*maps[20] = {0};
	t_player	player;
	void		*img;
	t_game		game;
	int y;
	int fd;

	game.mlx_ptr = mlx_init();
	game.win = mlx_new_window(game.mlx_ptr, WIN_X, WIN_Y, "cub3d");
	img = mlx_new_image(game.mlx_ptr, WIN_X, WIN_Y);
	game.image.data = mlx_get_data_addr(img,
		&game.image.bpp, &game.image.size_line, &game.image.endian);
	game.image.ptr = img;

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
	
	print_map(maps);

	game.player = player;
	game.maps = maps;
	get_wall_dist(&game, 0);
	print_map(game.maps);
	mlx_loop_hook(game.mlx_ptr, on_update, &game);
	mlx_key_hook(game.win, end_game, &game);
	// mlx_hook(game.win, KeyPress, KeyPressMask, end_game, &game);
	// mlx_loop(game.mlx_ptr);
	return (0);
}
