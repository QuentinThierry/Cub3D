/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/25 20:37:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

double	get_dist(t_game *game, double x, double y, double angle)
{
	t_fvector2	delta;
	double		res;

	delta.x = fabs(x - game->player->f_real_pos.x);
	delta.y = fabs(y - game->player->f_real_pos.y);

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
	
	bzero(game->image->addr, WIN_X * WIN_Y * 4);
	
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
		// printf("wall : %f %f\n", wall.x, wall.y);
		if (ray.hit.x == -1)
			height = 0;
		else
		{
			dist = get_dist(game, ray.hit.x, ray.hit.y, angle) ;
			height = 1 / dist * game->constants[0];		//div par 0 if sin == 0
			// printf("dist : %f	height : %f\n", dist, height);
		}
		draw_vert(game, x + WIN_X / 2, ray, height);
		x++;
	}
}