/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_2d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:24:03 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/06 01:05:14 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
float	get_dist(t_game *game, float x, float y, float angle);

static inline void	my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->size_line + x * img->opp);
	*(unsigned int*)dst = color;
}

t_fvector2	get_wall_hit_2d(t_game *game, float angle)
{
	t_fvector2	step;
	t_vector2	delta;
	t_fvector2	comp;
	t_vector2	sign;
	float		error = 0.001;
	int		x,y;
	
	my_mlx_pixel_put(game->image,
		game->player->pos.x, game->player->pos.y, 0xFF0000);
	sign = get_sign(angle);
	angle = fabs((float)tan(angle * TO_RADIAN));
	x = ((game->player->pos.x) / CHUNK_SIZE) * CHUNK_SIZE + (sign.x == 1) * CHUNK_SIZE;
	y = ((game->player->pos.y) / CHUNK_SIZE) * CHUNK_SIZE + (sign.y == 1) * CHUNK_SIZE;
	delta.x = abs(game->player->pos.x - (x));
	delta.y = abs(game->player->pos.y - (y));

	step.x = CHUNK_SIZE * angle * sign.x;
	step.y = CHUNK_SIZE / angle * sign.y;
	
	comp.x = game->player->pos.x + delta.y * angle * sign.x;
	comp.y = game->player->pos.y + delta.x / angle * sign.y;
	while (true)
	{
		while ((sign.y == 1 && y >= comp.y - error) || (y <= comp.y + error && sign.y == -1))		//x
		{
			my_mlx_pixel_put(game->image,
					x, (int)(comp.y), 0xFF0000);
			if (game->maps[((int)comp.y) / CHUNK_SIZE][x / CHUNK_SIZE + (sign.x == -1) * -1] == '1')
				return ((t_fvector2){x, comp.y});
			comp.y += step.y;
			x += sign.x * CHUNK_SIZE;
		}
		while ((sign.x == 1 && x >= comp.x - error) || (x <= comp.x + error && sign.x == -1))		//y
		{
			my_mlx_pixel_put(game->image,
					(int)(comp.x), y, 0x00FF00);
			if (game->maps[y / CHUNK_SIZE + (sign.y == -1) * -1][((int)comp.x) / CHUNK_SIZE] == '1')
				return ((t_fvector2){comp.x, y});
			comp.x += step.x;
			y += sign.y * CHUNK_SIZE;
		}
	}
	return ((t_fvector2){0});
}

void	raycasting_2d(t_game *game)
{
	int		x;
	float	angle;

	x = - game->map_size.x * CHUNK_SIZE / 2.0;
	while (x <  game->map_size.x * CHUNK_SIZE / 2.0)
	{
		angle = ((float)x * (FOV / 2.0)) / (game->map_size.x * CHUNK_SIZE / 2.0);
		if (game->player->angle + angle >= 360)
			game->player->angle = game->player->angle - 360;
		if (game->player->angle + angle < 0)
			game->player->angle = game->player->angle + 360;
		get_wall_hit_2d(game, game->player->angle + angle);
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
