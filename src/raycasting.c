/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/27 02:03:18 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


float	get_dist(t_game *game, float y, float angle)
{
	t_fvector2	delta;
	float		alpha;

	delta.y = fabsf(y - game->player->pos.y);
	alpha = game->player->angle + angle;
	if (alpha >= 360)
		alpha = alpha - 360;
	else if (alpha < 0)
		alpha = alpha + 360;
	if (alpha == 0)
		return (0);
	float h = delta.y / fabsf(cosf(alpha * TO_RADIAN));
	float c = cosf(angle * TO_RADIAN);
	return (c * h);
}

t_vector2	get_sign(float angle)
{
	t_vector2	sign;

	if (angle >= 0 && angle <= 180)
		sign.x = 1;
	else
		sign.x = -1;
	if (angle >= 90 && angle <= 270)
		sign.y = 1;
	else
		sign.y = -1;
	return (sign);
}

t_fvector2	get_wall_hit(t_game *game, float angle)
{
	const float	error = 0.001;
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	delta;
	t_vector2	sign;
	int			x,y;
	
	sign = get_sign(angle);
	angle = fabsf(tanf(angle * TO_RADIAN));
	x = (game->player->pos.x / CHUNK_SIZE) * CHUNK_SIZE + (sign.x == 1) * CHUNK_SIZE;
	y = (game->player->pos.y / CHUNK_SIZE) * CHUNK_SIZE + (sign.y == 1) * CHUNK_SIZE;
	delta.x = abs(game->player->pos.x - x);
	delta.y = abs(game->player->pos.y - y);

	step.x = CHUNK_SIZE * angle * sign.x;
	step.y = CHUNK_SIZE / angle * sign.y;
	
	comp.x = game->player->pos.x + delta.y * angle * sign.x;
	comp.y = game->player->pos.y + delta.x / angle * sign.y;
	while (true)
	{
		while ((sign.y == 1 && y >= comp.y - error) || (y <= comp.y + error && sign.y == -1))		//x
		{
			if (game->maps[((int)comp.y) / CHUNK_SIZE][x / CHUNK_SIZE + (sign.x == -1) * -1] == '1')
			{
				return ((t_fvector2){x, comp.y});
			}
			comp.y += step.y;
			x += sign.x * CHUNK_SIZE;
		}
		while ((sign.x == 1 && x >= comp.x - error) || (x <= comp.x + error && sign.x == -1))		//y
		{
			if (game->maps[y / CHUNK_SIZE + (sign.y == -1) * -1][((int)comp.x) / CHUNK_SIZE] == '1')
			{
				return ((t_fvector2){comp.x, y});
			}
			comp.x += step.x;
			y += sign.y * CHUNK_SIZE;
		}
	}
	return ((t_fvector2){0});
}

void	raycasting(t_game *game)
{
	int			x;
	float		height;
	float		angle;
	t_fvector2	wall;
	float		delta_angle;

	x = -WIN_X / 2;
	while (x < WIN_X / 2)
	{
		delta_angle = ((WIN_X / 2.0) / (tanf((FOV / 2.0) * TO_RADIAN)));
		angle = atanf(x / delta_angle) * 180 / M_PI;
		if (game->player->angle + angle >= 360)
			game->player->angle = game->player->angle - 360;
		if (game->player->angle + angle < 0)
			game->player->angle = game->player->angle + 360;
		wall = get_wall_hit(game, game->player->angle + angle);
		height = CHUNK_SIZE / get_dist(game, wall.y, angle) * delta_angle;		//div par 0 if sin == 0
		draw_vert_sprite(game, x + WIN_X / 2, wall, height);
		x++;
	}
}
