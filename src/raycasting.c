/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/19 19:06:07 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


float	get_dist(t_game *game, float x, float y, float angle)
{
	t_fvector2	delta;

	delta.x = fabsf(x - game->player->f_real_pos.x);
	delta.y = fabsf(y - game->player->f_real_pos.y);
	return (cos(angle * M_PI / 180) * sqrt((delta.x * delta.x + delta.y * delta.y)));
	// return (delta.x * cos((FOV / 2.0) * M_PI / 180) + delta.y * sin((FOV / 2.0) * M_PI / 180));
}

t_fvector2	get_wall_hit(t_game *game, float angle)
{
	t_fvector2	step;
	t_fvector2	delta;
	t_fvector2	comp;
	t_vector2	sign;
	int		x,y;
	
	if (angle >= 0 && angle <= 180)
		sign.x = 1;
	else
		sign.x = -1;
	if (angle >= 90 && angle <= 270)
		sign.y = 1;
	else
		sign.y = -1;

	angle = fabsf((float)tan(angle * M_PI / 180));
	
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
			// my_mlx_pixel_put(game->image,
			// 		x * CHUNK_SIZE, (int)(comp.y * CHUNK_SIZE), 0xFF0000);
			if (game->maps[(int)comp.y][x + (sign.x == -1) * -1] == '1')
			{
				return ((t_fvector2){x, comp.y});
			}
			comp.y += step.y;
			x += sign.x;
		}
		while ((sign.x == 1 && x >= comp.x) || (x <= comp.x && sign.x == -1))		//y
		{
			// my_mlx_pixel_put(game->image,
			// 		(int)(comp.x * CHUNK_SIZE), y * CHUNK_SIZE, 0x00FF00);
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


void	raycasting(t_game *game)
{
	int		x;
	float	height;
	float	angle;
	t_fvector2	wall;

	x = - WIN_X / 2.0;
	while (x <  WIN_X / 2.0)
	{
		angle = ((float)x * (FOV / 2.0)) / (WIN_X / 2.0);
		if (game->player->angle + angle >= 360)
			game->player->angle = game->player->angle - 360;
		if (game->player->angle + angle < 0)
			game->player->angle = game->player->angle + 360;
		wall = get_wall_hit(game, game->player->angle + angle);
		height = HEIGHT_WALL / get_dist(game, wall.x, wall.y, fabsf(angle));
		draw_vert_sprite(game, x + WIN_X / 2.0, wall, height);
		x++;
	}
}