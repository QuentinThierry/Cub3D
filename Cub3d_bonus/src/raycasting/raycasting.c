/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/27 21:02:11 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static double	get_dist(t_fvector2 fpos, t_fvector2 wall, double angle)
{
	t_fvector2	delta;
	double		res;

	delta.x = fabs(wall.x - fpos.x);
	delta.y = fabs(wall.y - fpos.y);

	res = sqrt((delta.x * delta.x + delta.y * delta.y))
				* cos(angle * TO_RADIAN);
	if (res == 0)
		return (0.01);
	return (res);
}

t_vector2	get_sign(double angle)
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
	double		angle;
	t_ray		ray;
	t_fvector2	fpos;
	double		dist; 
	
	draw_ceiling(game);
	fpos = game->player->f_real_pos;
	x = -WIN_X / 2;
	while (x < WIN_X / 2)
	{
		angle = atan(x / game->constants[0]) * 180 / M_PI;
		if (game->player->angle + angle >= 360)
			angle = angle - 360;
		if (game->player->angle + angle < 0)
			angle = angle + 360;
		ray = get_wall_hit(fpos, game->map, game->player->angle + angle, game);
		if (ray.hit.x == -1)
			height = 0;
		else
		{
			dist = get_dist(game->player->f_real_pos, ray.hit, angle) ;
			height = 1 / dist * game->constants[0];		//div par 0 if sin == 0
		}
		draw_vert(game, x + WIN_X / 2, ray, height);
		if (ray.nb_object_hit != 0)
		{
			printf("nb object : %d\n", ray.nb_object_hit);
			draw_object(game, ray, x + WIN_X / 2, game->player->angle + angle);
		}
		x++;
	}
}
