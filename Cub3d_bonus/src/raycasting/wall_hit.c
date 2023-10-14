/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:04:05 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/14 16:27:39 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_ray	get_wall_hit_se(t_dvector2 fpos, t_map **map, float angle,
			float player_angle);
t_ray	get_wall_hit_ne(t_dvector2 fpos, t_map **map, float angle,
			float player_angle);
t_ray	get_wall_hit_sw(t_dvector2 fpos, t_map **map, float angle,
			float player_angle);
t_ray	get_wall_hit_nw(t_dvector2 fpos, t_map **map, float angle,
			float player_angle);

t_ray	get_wall_hit(t_dvector2 fpos, t_map **map, float angle)
{
	t_vector2	sign;

	if ((map[(int)fpos.y][(int)fpos.x].type & OBJECT) == OBJECT)
		((t_object *)map[(int)fpos.y][(int)fpos.x].arg)->visited = true;
	sign = get_sign(angle);
	if (sign.x == 1 && sign.y == 1)
		return (get_wall_hit_se(fpos, map, fabsf(tanf(angle * TO_RADIAN)),
				angle));
	else if (sign.x == 1 && sign.y == -1)
		return (get_wall_hit_ne(fpos, map, fabsf(tanf(angle * TO_RADIAN)),
				angle));
	else if (sign.x == -1 && sign.y == 1)
		return (get_wall_hit_sw(fpos, map, fabsf(tanf(angle * TO_RADIAN)),
				angle));
	else
		return (get_wall_hit_nw(fpos, map, fabsf(tanf(angle * TO_RADIAN)),
				angle));
}
