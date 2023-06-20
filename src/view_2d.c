/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_2d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:24:03 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/20 16:25:25 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_fvector2	get_wall_hit_2d(t_game *game, float angle)
{
	t_fvector2	step;
	t_fvector2	delta;
	t_fvector2	comp;
	t_vector2	sign;
	int		x,y;
	
	sign = get_sign(angle);
	my_mlx_pixel_put(game->image,
		game->player->pos.x, game->player->pos.y, 0xFF0000);
	angle = fabs((float)tan(angle * M_PI / 180));
	
	x = (int)(game->player->f_real_pos.x) + (sign.x == 1);
	y = (int)(game->player->f_real_pos.y) + (sign.y == 1);
	delta.x = fabsf(game->player->f_real_pos.x - (x));
	delta.y = fabsf(game->player->f_real_pos.y - (y));

	step.x = 1 * angle * sign.x;
	step.y = 1 / angle * sign.y;
	
	comp.x = game->player->f_real_pos.x + delta.y * angle * sign.x;
	comp.y = game->player->f_real_pos.y + delta.x / angle * sign.y;

	while (true)
	{
		while ((sign.y == 1 && y >= comp.y) || (y <= comp.y && sign.y == -1))		//x
		{
			my_mlx_pixel_put(game->image,
					x * CHUNK_SIZE, (int)(comp.y * CHUNK_SIZE), 0xFF0000);
			if (game->maps[(int)comp.y][x + (sign.x == -1) * -1] == '1')
			{
				return ((t_fvector2){x, comp.y});
			}
			comp.y += step.y;
			x += sign.x;
		}
		while ((sign.x == 1 && x >= comp.x) || (x <= comp.x && sign.x == -1))		//y
		{
			my_mlx_pixel_put(game->image,
					(int)(comp.x * CHUNK_SIZE), y * CHUNK_SIZE, 0x00FF00);
			if (game->maps[y + (sign.y == -1) * -1][(int)comp.x] == '1')
			{
				return ((t_fvector2){comp.x, y});
			}
			comp.x += step.x;
			y += sign.y;
		}
	}
	return ((t_fvector2){0});
}

void	raycasting_2d(t_game *game)
{
	int		x;
	float	height;
	float	angle;
	t_fvector2	wall;

	x = - game->map_size.x * CHUNK_SIZE / 2.0;
	while (x <  game->map_size.x * CHUNK_SIZE / 2.0)
	{
		angle = ((float)x * (FOV / 2.0)) / (game->map_size.x * CHUNK_SIZE/ 2.0);
		if (game->player->angle + angle >= 360)
			game->player->angle = game->player->angle - 360;
		if (game->player->angle + angle < 0)
			game->player->angle = game->player->angle + 360;
		wall = get_wall_hit_2d(game, game->player->angle + angle);
		x++;
	}
}

void	quadrillage(t_game *game)
{
	int	x;
	int	y;
	unsigned color;
	
	y = 0;
	while (y < game->map_size.y)
	{
		x = 0;
		while (x < game->map_size.x * CHUNK_SIZE)
		{
			if (game->maps[y][x / CHUNK_SIZE] == '1'
				|| game->maps[(y-1)][(x-1) / CHUNK_SIZE] == '1')
				color = 0x404040;
			else
				color = 0x202020;
			my_mlx_pixel_put(game->image, x, y * CHUNK_SIZE, color);
			x++;
		}
		y++;
	}
	x = 0;
	while (x < game->map_size.x)
	{
		y = 0;
		while (y < game->map_size.y * CHUNK_SIZE)
		{
			if (game->maps[y / CHUNK_SIZE][x] == '1'
				|| game->maps[(y-1) / CHUNK_SIZE][(x-1)] == '1')
				color = 0x404040;
			else
				color = 0x202020;
			my_mlx_pixel_put(game->image, x * CHUNK_SIZE, y, color);
			y++;
		}
		x++;
	}
}
