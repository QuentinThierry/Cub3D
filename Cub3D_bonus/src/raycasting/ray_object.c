/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 18:21:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 15:11:32 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_ray	get_object_ne(t_dvector2 begin,
			t_map **map, t_launch_ray object, float angle);
t_ray	get_object_se(t_dvector2 begin,
			t_map **map, t_launch_ray object, float angle);

__attribute__((always_inline))
static inline t_ray	_hit_object_sw(t_dvector2 *comp, t_vector2 *pos,
		t_map **map, t_info_get_object *inf)
{
	if (pos->y < comp->y)
	{
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
	if (pos->x < inf->max_pos.x)
		return ((t_ray){{-1, -1}, e_east});
	if ((map[(int)(comp->y)][pos->x - 1].type & inf->obj.type) == inf->obj.type
		&& (map[(int)(comp->y)][pos->x - 1].symbol == inf->obj.symbol
			|| inf->obj.symbol == '\0'))
		return ((t_ray){{pos->x - 1, comp->y}, e_east});
	if ((map[(int)(comp->y)][pos->x - 1].type & WALL) == WALL)
		return ((t_ray){{-1, -1}, e_east});
	comp->y += inf->step.y;
	pos->x += -1;
	return ((t_ray){{-1, -1}, -1});
}

// xy (-1, 1)
static t_ray	_get_object_sw(t_dvector2 begin,
								t_map **map, t_launch_ray object, float angle)
{
	t_info_get_object	infos;
	t_dvector2			comp;
	t_vector2			map_pos;
	float				t_angle;
	t_ray				ray;

	infos.obj = object;
	infos.max_pos = (t_dvector2){begin.x - sin((angle - 180) * TO_RADIAN)
		* object.dist, begin.y + cos((angle - 180) * TO_RADIAN) * object.dist};
	t_angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)begin.x, (int)begin.y + 1};
	comp.x = begin.x + fabs(begin.y - map_pos.y) * t_angle * -1;
	comp.y = begin.y + fabs(begin.x - map_pos.x) / t_angle;
	infos.step = (t_dvector2){t_angle * -1, 1 / t_angle};
	while (true)
	{
		ray = _hit_object_sw(&comp, &map_pos, map, &infos);
		if (ray.orient != -1)
			return (ray);
	}
}

__attribute__((always_inline))
static inline t_ray	_hit_object_nw(t_dvector2 *comp, t_vector2 *pos,
		t_map **map, t_info_get_object *inf)
{
	if (pos->y <= comp->y)
	{
		if (pos->x < inf->max_pos.x)
			return ((t_ray){{-1, -1}, e_east});
		if ((map[(int)comp->y][pos->x -1].type & inf->obj.type) == inf->obj.type
			&& (map[(int)(comp->y)][pos->x - 1].symbol == inf->obj.symbol
				|| inf->obj.symbol == '\0'))
			return ((t_ray){{pos->x - 1, comp->y}, e_east});
		if ((map[(int)(comp->y)][pos->x - 1].type & WALL) == WALL)
			return ((t_ray){{-1, -1}, e_east});
		comp->y += inf->step.y;
		pos->x += -1;
		return ((t_ray){{-1, -1}, -1});
	}
	if (pos->y < inf->max_pos.y)
		return ((t_ray){{-1, -1}, e_south});
	if ((map[pos->y - 1][(int)(comp->x)].type & inf->obj.type) == inf->obj.type
		&& (map[pos->y - 1][(int)(comp->x)].symbol == inf->obj.symbol
			|| inf->obj.symbol == '\0'))
		return ((t_ray){{comp->x, pos->y - 1}, e_south});
	if ((map[pos->y - 1][(int)(comp->x)].type & WALL) == WALL)
		return ((t_ray){{-1, -1}, e_south});
	comp->x += inf->step.x;
	pos->y += -1;
	return ((t_ray){{-1, -1}, -1});
}

// xy (-1, -1)
static t_ray	_get_object_nw(t_dvector2 begin,
								t_map **map, t_launch_ray object, float angle)
{
	t_info_get_object	infos;
	t_ray				ray;
	t_dvector2			comp;
	t_vector2			map_pos;
	float				t_angle;

	infos.obj = object;
	infos.max_pos = (t_dvector2){begin.x - sin((360 - angle) * TO_RADIAN)
		* object.dist, begin.y - cos((360 - angle) * TO_RADIAN) * object.dist};
	t_angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)begin.x, (int)begin.y};
	comp.x = begin.x + fabs(begin.y - map_pos.y) * t_angle * -1;
	comp.y = begin.y + fabs(begin.x - map_pos.x) / t_angle * -1;
	infos.step = (t_dvector2){t_angle * -1, 1 / t_angle * -1};
	while (true)
	{
		ray = _hit_object_nw(&comp, &map_pos, map, &infos);
		if (ray.orient != -1)
			return (ray);
	}
}

t_ray	get_object_hit(t_launch_ray object, t_map **map, t_dvector2 begin,
		float angle)
{
	t_vector2	sign;

	sign = get_sign(angle);
	if (sign.x == 1 && sign.y == 1)
		return (get_object_se(begin, map, object, angle));
	else if (sign.x == 1 && sign.y == -1)
		return (get_object_ne(begin, map, object, angle));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_object_sw(begin, map, object, angle));
	else
		return (_get_object_nw(begin, map, object, angle));
}
