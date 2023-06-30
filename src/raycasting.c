/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/30 17:43:06 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

float	get_dist(t_game *game, float x, float y, float angle)
{
	t_fvector2	delta;
	float		alpha;

	delta.x = fabsf(x - game->player->f_real_pos.x);
	delta.y = fabsf(y - game->player->f_real_pos.y);
	alpha = game->player->angle + angle;
	if (alpha >= 360)
		alpha = alpha - 360;
	else if (alpha < 0)
		alpha = alpha + 360;
	if (alpha == 0)
		alpha++;
	// float h = delta.y / fabsf(cosf(alpha * TO_RADIAN));
	float h = sqrtf((delta.x * delta.x + delta.y * delta.y));
	float c = cosf(angle * TO_RADIAN);
	if (h == 0)
		return (0.1); // anti segfault, (when too close to wall) to change
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
	const float	error = 0.0001;
	t_fvector2	step;
	t_fvector2	comp;
	t_fvector2	delta;
	t_vector2	sign;
	int			x,y;

	sign = get_sign(angle);
	angle = fabsf(tanf(angle * TO_RADIAN));
	x = ((int)game->player->f_real_pos.x) + (sign.x == 1);
	y = ((int)game->player->f_real_pos.y) + (sign.y == 1);
	delta.x = fabsf(game->player->f_real_pos.x - x);
	delta.y = fabsf(game->player->f_real_pos.y - y);

	step.x = 1 * angle * sign.x;
	step.y = 1 / angle * sign.y;
	
	comp.x = game->player->f_real_pos.x + delta.y * angle * sign.x;
	comp.y = game->player->f_real_pos.y + delta.x / angle * sign.y;
	while (true)
	{
		while ((sign.y == 1 && y >= comp.y - error) || (y <= comp.y + error && sign.y == -1))		//x
		{
			if (game->maps[(int)comp.y][x + (sign.x == -1) * -1] == '1')
				return ((t_fvector2){x, comp.y});
			comp.y += step.y;
			x += sign.x;
		}
		while ((sign.x == 1 && x >= comp.x - error) || (x <= comp.x + error && sign.x == -1))		//y
		{
			if (game->maps[y + (sign.y == -1) * -1][((int)comp.x)] == '1')
				return ((t_fvector2){comp.x, y});
			comp.x += step.x;
			y += sign.y;
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

	x = -WIN_X / 2;
	while (x < WIN_X / 2)
	{
		angle = atanf(x / game->constants[0]) * 180 / M_PI;
		if (game->player->angle + angle >= 360)
			angle = angle - 360;
		if (game->player->angle + angle < 0)
			angle = angle + 360;
		wall = get_wall_hit(game, game->player->angle + angle);
		height = 1 / get_dist(game, wall.x, wall.y, angle) * game->constants[0];		//div par 0 if sin == 0
		draw_vert_sprite(game, x + WIN_X / 2, wall, height);
		x++;
	}
}
