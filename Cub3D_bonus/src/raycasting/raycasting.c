/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/14 14:48:56 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline float	get_dist(t_dvector2 fpos, t_dvector2 wall)
{
	return (sqrtf((wall.x - fpos.x) * (wall.x - fpos.x) + (wall.y - fpos.y)
			* (wall.y - fpos.y)));
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
	double		height;
	float		angle;
	t_ray		ray;
	float		dist;

	x = -WIN_X / 2 - 1;
	while (++x < WIN_X / 2)
	{
		angle = atanf(x / game->constants[0]) * 180 / M_PI;
		if (game->player->angle + angle >= 360)
			angle = angle - 360;
		else if (game->player->angle + angle < 0)
			angle = angle + 360;
		ray = get_wall_hit(game->player->f_pos, game->map,
				game->player->angle + angle);
		dist = get_dist(game->player->f_pos, ray.hit);
		game->dist_tab[x + WIN_X / 2] = dist;
		dist *= cosf(angle * TO_RADIAN);
		dist += (dist == 0) * 0.01;
		height = 1 / dist * game->constants[0];
		game->height_tab[x + WIN_X / 2] = height / 2;
		draw_vert(game, x + WIN_X / 2, ray, height);
	}
	(draw_ceiling(game), draw_objects(game));
}
