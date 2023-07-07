/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/07 19:53:43 by jvigny           ###   ########.fr       */
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

void	raycasting(t_game *game)
{
	int			x;
	float		height;
	float		angle;
	t_fvector2	wall;
	t_fvector2	fpos;
	float		dist; 
	
	fpos = game->player->f_real_pos;
	x = -WIN_X / 2;
	while (x < WIN_X / 2)
	{
		angle = atanf(x / game->constants[0]) * 180 / M_PI;
		if (game->player->angle + angle >= 360)
			angle = angle - 360;
		if (game->player->angle + angle < 0)
			angle = angle + 360;
		wall = get_wall_hit(fpos, game->maps, game->player->angle + angle, game->map_size);
		if (wall.x == -1 && wall.y == -1)
			height = 0;
		else
		{
			dist = get_dist(game, wall.x, wall.y, angle) ;
			height = 1 / dist * game->constants[0];		//div par 0 if sin == 0
		}
		draw_vert(game, x + WIN_X / 2, wall, height);
		x++;
	}
}
