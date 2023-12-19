/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_object1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 15:08:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 15:09:20 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline t_ray	_hit_object_se(t_dvector2 *comp, t_vector2 *pos,
	t_map **map, t_info_get_object *inf)
{
	if (pos->y >= comp->y)
	{
		if (pos->x > inf->max_pos.x)
			return ((t_ray){{-1, -1}, e_west});
		if ((map[(int)(comp->y)][pos->x].type & inf->obj.type) == inf->obj.type
			&& (map[(int)(comp->y)][pos->x].symbol == inf->obj.symbol
				|| inf->obj.symbol == '\0'))
			return ((t_ray){{pos->x, comp->y}, e_west});
		if ((map[(int)(comp->y)][pos->x].type & WALL) == WALL)
			return ((t_ray){{-1, -1}, e_west});
		comp->y += inf->step.y;
		pos->x += 1;
		return ((t_ray){{-1, -1}, -1});
	}
	if (pos->y > inf->max_pos.y)
		return ((t_ray){{-1, -1}, e_north});
	if ((map[pos->y][(int)(comp->x)].type & inf->obj.type) == inf->obj.type
		&& (map[pos->y][(int)(comp->x)].symbol == inf->obj.symbol
			|| inf->obj.symbol == '\0'))
		return ((t_ray){{comp->x, pos->y}, e_north});
	if ((map[pos->y][(int)(comp->x)].type & WALL) == WALL)
		return ((t_ray){{-1, -1}, e_north});
	comp->x += inf->step.x;
	pos->y += 1;
	return ((t_ray){{-1, -1}, -1});
}

// xy (1, 1)
t_ray	get_object_se(t_dvector2 begin,
								t_map **map, t_launch_ray object, float angle)
{
	t_info_get_object	infos;
	t_dvector2			comp;
	t_vector2			map_pos;
	t_ray				ray;
	float				t_angle;

	infos.obj = object;
	infos.max_pos = (t_dvector2){begin.x + cos((angle - 90) * TO_RADIAN)
		* object.dist, begin.y + sin((angle - 90) * TO_RADIAN) * object.dist};
	t_angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)begin.x + 1, (int)begin.y + 1};
	comp.x = begin.x + fabs(begin.y - map_pos.y) * t_angle;
	comp.y = begin.y + fabs(begin.x - map_pos.x) / t_angle;
	infos.step = (t_dvector2){t_angle, 1 / t_angle};
	while (true)
	{
		ray = _hit_object_se(&comp, &map_pos, map, &infos);
		if (ray.orient != -1)
			return (ray);
	}
}

__attribute__((always_inline))
static inline t_ray	_hit_object_ne(t_dvector2 *comp, t_vector2 *pos,
		t_map **map, t_info_get_object *inf)
{
	if (pos->y <= comp->y)
	{
		if (pos->x > inf->max_pos.x)
			return ((t_ray){{-1, -1}, e_west});
		if ((map[(int)(comp->y)][pos->x].type & inf->obj.type) == inf->obj.type
			&& (map[(int)(comp->y)][pos->x].symbol == inf->obj.symbol
				|| inf->obj.symbol == '\0'))
			return ((t_ray){{pos->x, comp->y}, e_west});
		if ((map[(int)(comp->y)][pos->x].type & WALL) == WALL)
			return ((t_ray){{-1, -1}, e_west});
		comp->y += inf->step.y;
		pos->x += 1;
		return ((t_ray){{-1, -1}, -1});
	}
	if (pos->y < inf->max_pos.y)
		return ((t_ray){{-1, -1}, e_south});
	if ((map[pos->y - 1][(int)(comp->x)].type & inf->obj.type) == inf->obj.type
		&& (map[pos->y - 1][((int)(comp->x))].symbol == inf->obj.symbol
			|| inf->obj.symbol == '\0'))
		return ((t_ray){{comp->x, pos->y - 1}, e_south});
	if ((map[pos->y - 1][(int)(comp->x)].type & WALL) == WALL)
		return ((t_ray){{-1, -1}, e_south});
	comp->x += inf->step.x;
	pos->y += -1;
	return ((t_ray){{-1, -1}, -1});
}

// xy (1, -1)
t_ray	get_object_ne(t_dvector2 begin,
								t_map **map, t_launch_ray object, float angle)
{
	t_info_get_object	infos;
	t_dvector2			comp;
	t_vector2			map_pos;
	float				t_angle;
	t_ray				ray;

	infos.obj = object;
	infos.max_pos = (t_dvector2){begin.x + cos((90 - angle) * TO_RADIAN)
		* object.dist, begin.y - sin((90 - angle) * TO_RADIAN) * object.dist};
	t_angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)begin.x + 1, (int)begin.y};
	comp.x = begin.x + fabs(begin.y - map_pos.y) * t_angle;
	comp.y = begin.y + fabs(begin.x - map_pos.x) / t_angle * -1;
	infos.step = (t_dvector2){t_angle, 1 / t_angle * -1};
	while (true)
	{
		ray = _hit_object_ne(&comp, &map_pos, map, &infos);
		if (ray.orient != -1)
			return (ray);
	}
}
