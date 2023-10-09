/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:04:05 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/09 19:34:16 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static t_vector2	get_sign(double angle)
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

t_fvector2	get_wall_hit(t_fvector2 fpos, char **map, float angle,
		t_vector2 map_size)
{
	t_vector2	sign;

	if (fpos.x < 0 || fpos.y < 0 || fpos.x > map_size.x || fpos.y > map_size.y)
		return ((t_fvector2){-1, -1});
	sign = get_sign(angle);
	angle = fabs(tan(angle * TO_RADIAN));
	if (sign.x == 1 && sign.y == 1)
		return (get_wall_hit_se(fpos, map, angle, map_size));
	else if (sign.x == 1 && sign.y == -1)
		return (get_wall_hit_ne(fpos, map, angle, map_size));
	else if (sign.x == -1 && sign.y == 1)
		return (get_wall_hit_sw(fpos, map, angle, map_size));
	else
		return (get_wall_hit_nw(fpos, map, angle));
	return ((t_fvector2){0});
}
