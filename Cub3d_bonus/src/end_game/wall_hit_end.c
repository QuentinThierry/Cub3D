/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit_end.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:04:05 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/15 14:14:41 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_ray	get_wall_hit_ne_end(t_dvector2 fpos,
			t_map **map, t_info_wall *info);
t_ray	get_wall_hit_se_end(t_dvector2 fpos,
			t_map **map, t_info_wall *info);
t_ray	get_wall_hit_sw_end(t_dvector2 fpos,
			t_map **map, t_info_wall *info);
t_ray	get_wall_hit_nw_end(t_dvector2 fpos,
			t_map **map, t_info_wall *info);

t_ray	get_wall_hit_end(t_dvector2 fpos, t_map **map, float angle,
		enum e_status status)
{
	t_vector2	sign;
	t_info_wall	infos;

	infos.status = status;
	infos.player_angle = angle;
	infos.angle = fabsf(tanf(angle * TO_RADIAN));
	if ((map[(int)fpos.y][(int)fpos.x].type & OBJECT) == OBJECT)
		((t_object *)map[(int)fpos.y][(int)fpos.x].arg)->visited = true;
	sign = get_sign(angle);
	if (sign.x == 1 && sign.y == 1)
		return (get_wall_hit_se_end(fpos, map, &infos));
	else if (sign.x == 1 && sign.y == -1)
		return (get_wall_hit_ne_end(fpos, map, &infos));
	else if (sign.x == -1 && sign.y == 1)
		return (get_wall_hit_sw_end(fpos, map, &infos));
	else
		return (get_wall_hit_nw_end(fpos, map, &infos));
}
