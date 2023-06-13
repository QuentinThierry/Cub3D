/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/13 17:08:46 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include <X11/X.h>

#define WIN_X 400
#define WIN_Y 300
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

int	key_press_hook(int key, t_game *game)
{
	(void)game;
	if (key == 65307 ) // esc
		exit(0);
	if (key == 65362) // up arrow
		game->player.angle -= 1;
	if (key == 65364) // down arrow
		game->player.angle += 1;
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
	float	x_step;
	float	y_step;
	float	r_angle;
	int		x,y;
	float	dx;
	float	dy;
	float	x_comp;
	float	y_comp;

	game->player.pos.x = 210;
	game->player.pos.y = 120;
	my_mlx_pixel_put(&game->image,
		game->player.pos.x, game->player.pos.y, 0xFF0000);
	r_angle = angle * M_PI / 180;
	dx = (float)game->player.pos.x / CHUNK_SIZE - (int)(game->player.pos.x / CHUNK_SIZE);
	dy = (float)game->player.pos.y / CHUNK_SIZE - (int)(game->player.pos.y / CHUNK_SIZE);

	x_step = 1 * (float)tan(r_angle);
	y_step = 1 / (float)tan(r_angle);

	x = game->player.pos.x / CHUNK_SIZE;
	y = game->player.pos.y / CHUNK_SIZE;

	x_comp = x + dx - dy * (float)tan(r_angle);
	y_comp = y + dy - dx / (float)tan(r_angle);

	while (true)
	{
		while (y < y_comp)
		{
			my_mlx_pixel_put(&game->image,
					x * CHUNK_SIZE, (int)(y_comp * CHUNK_SIZE), 0xD77BBA);
			if (game->maps[(int)y_comp][x] == '1')
			{
				return (0);
			}
			y_comp -= y_step;
			x--;
		}
		while (x < x_comp)
		{
			my_mlx_pixel_put(&game->image,
					(int)(x_comp * CHUNK_SIZE), y * CHUNK_SIZE, 0x00FF00);
			if (game->maps[y][(int)x_comp] == '1')
			{
				return (0);
			}
			x_comp -= x_step;
			y--;
		}
	}

	return (0);
}

void	quadrillage(t_game *game)
{
	int	x;
	int	y;
	unsigned color;
	
	y = 0;
	while (y < WIN_Y)
	{
		x = 0;
		while (x < WIN_X)
		{
			if (game->maps[y / CHUNK_SIZE][x / CHUNK_SIZE] == '1'
				|| game->maps[(y-1) / CHUNK_SIZE][(x-1) / CHUNK_SIZE] == '1')
				color = 0x404040;
			else
				color = 0x202020;
			my_mlx_pixel_put(&game->image, x, y, color);
			x++;
		}
		y += CHUNK_SIZE;
	}
	x = 0;
	while (x < WIN_X)
	{
		y = 0;
		while (y < WIN_Y)
		{
			if (game->maps[y / CHUNK_SIZE][x / CHUNK_SIZE] == '1'
				|| game->maps[(y-1) / CHUNK_SIZE][(x-1) / CHUNK_SIZE] == '1')
				color = 0x404040;
			else
				color = 0x202020;
			my_mlx_pixel_put(&game->image, x, y, color);
			y++;
		}
		x += CHUNK_SIZE;
	}
}

void	raycasting(t_game *game)
{
	int	x;
	// int	angle;

	x = 0;
	(void)game;
	while (x < FOV)
	{
		// angle = FOV / x;

		// draw_vert(game, x, 10, 20);
		x++;
	}
}

int	on_update(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	// long					fps;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
		
	raycasting(game);
	quadrillage(game);
	get_wall_dist(game, game->player.angle);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image.ptr, 0, 0);
	// usleep(100000);
	// bzero(game->image.data, 200 * 200 * 4);
	// mlx_clear_window(game->mlx_ptr, game->win);
	
	clock_gettime(CLOCK_REALTIME, &cur_time);
	// fps = 1.F / (cur_time.tv_sec - last_time.tv_sec + (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	// printf("%ld\n", fps);
	last_time = cur_time;
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

	(void)argc, (void)argv;
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
	player.angle = 45;
	// print_map(maps);
	game.player = player;
	game.maps = maps;
	quadrillage(&game);
	
	// print_map(game.maps);
	mlx_loop_hook(game.mlx_ptr, on_update, &game);
	mlx_key_hook(game.win, key_press_hook, &game);
	mlx_loop(game.mlx_ptr);
	return (0);
}
