/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit_nw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 15:56:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/14 16:31:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline t_ray	_wall_hit_ver_nw(t_map *map_cell, t_dvector2 *hit,
				float step, float player_angle)
{
	t_dvector2	door;

	if ((map_cell->type & OBJECT) == OBJECT)
		((t_object *)map_cell->arg)->visited = true;
	else if ((map_cell->type & WALL) == WALL)
	{
		if ((map_cell->type & DOOR) == DOOR)
		{
			door = door_hit_ver_nw(*hit, step,
					(float)((t_door *)map_cell->arg)->door_percent,
					player_angle);
			if (door.x >= 0)
				return ((t_ray){door, e_east});
		}
		else
			return ((t_ray){(*hit), e_east});
	}
	hit->y += step;
	hit->x -= 1;
	return ((t_ray){{-1, -1}, -1});
}

__attribute__((always_inline))
static inline t_ray	_wall_hit_hor_nw(t_map *map_cell, t_dvector2 *hit,
				float step, float player_angle)
{
	t_dvector2	door;

	if ((map_cell->type & OBJECT) == OBJECT)
		((t_object *)map_cell->arg)->visited = true;
	else if ((map_cell->type & WALL) == WALL)
	{
		if ((map_cell->type & DOOR) == DOOR)
		{
			door = door_hit_hor_nw(*hit, step,
					(float)((t_door *)map_cell->arg)->door_percent,
					player_angle);
			if (door.x >= 0)
				return ((t_ray){door, e_south});
		}
		else
			return ((t_ray){(*hit), e_south});
	}
	hit->x += step;
	hit->y += -1;
	return ((t_ray){{-1, -1}, -1});
}

// xy (-1, -1)
t_ray	get_wall_hit_nw(t_dvector2 fpos, t_map **map, float angle,
		float player_angle)
{
	t_dvector2	hor;
	t_dvector2	ver;
	t_ray		ray;

	ver = (t_dvector2){(int)fpos.x,
		fpos.y + fabs(fpos.x - (int)fpos.x) / angle * -1};
	hor = (t_dvector2){fpos.x + fabs(fpos.y - (int)fpos.y) * angle * -1,
		(int)fpos.y};
	while (true)
	{
		if (hor.y <= ver.y)
		{
			ray = _wall_hit_ver_nw(&map[(int)ver.y][(int)ver.x - 1],
					&ver, 1 / angle * -1, player_angle);
			if (ray.hit.x != -1)
				return (ray);
		}
		else
		{
			ray = _wall_hit_hor_nw(&map[(int)hor.y - 1][(int)hor.x],
					&hor, angle * -1, player_angle);
			if (ray.hit.x != -1)
				return (ray);
		}
	}
}
