/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/13 17:27:27 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	my_mlx_pixel_put(t_image *img, int x, int y, int color);

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