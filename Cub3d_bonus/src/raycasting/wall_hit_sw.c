/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit_sw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 15:56:18 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/14 16:33:40 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline t_ray	_wall_hit_ver_sw(t_map *map_cell, t_dvector2 *hit,
					float step, float player_angle)
{
	t_dvector2	door;

	if ((map_cell->type & OBJECT) == OBJECT)
		((t_object *)map_cell->arg)->visited = true;
	else if ((map_cell->type & WALL) == WALL)
	{
		if ((map_cell->type & DOOR) == DOOR)
		{
			door = door_hit_ver_sw(*hit, step,
					(float)((t_door *)map_cell->arg)->door_percent,
					player_angle);
			if (door.x >= 0)
				return ((t_ray){door, e_east});
		}
		else
			return ((t_ray){(*hit), e_east});
	}
	hit->x -= 1;
	hit->y += step;
	return ((t_ray){{-1, -1}, -1});
}

__attribute__((always_inline))
static inline t_ray	_wall_hit_hor_sw(t_map *map_cell, t_dvector2 *hit,
					float step, float player_angle)
{
	t_dvector2	door;

	if ((map_cell->type & OBJECT) == OBJECT)
		((t_object *)map_cell->arg)->visited = true;
	else if ((map_cell->type & WALL) == WALL)
	{
		if ((map_cell->type & DOOR) == DOOR)
		{
			door = door_hit_hor_sw(*hit, step,
					(float)((t_door *)map_cell->arg)->door_percent,
					player_angle);
			if (door.x >= 0)
				return ((t_ray){door, e_north});
		}
		else
			return ((t_ray){(*hit), e_north});
	}
	hit->x += step;
	hit->y += 1;
	return ((t_ray){{-1, -1}, -1});
}

// xy (-1, 1)
t_ray	get_wall_hit_sw(t_dvector2 fpos, t_map **map, float angle,
			float player_angle)
{
	t_dvector2	ver;
	t_dvector2	hor;
	t_ray		ray;

	ver = (t_dvector2){(int)fpos.x,
		fpos.y + fabs(fpos.x - (int)fpos.x) / angle};
	hor = (t_dvector2){fpos.x + fabs(fpos.y - ((int)fpos.y + 1)) * angle * -1,
		(int)fpos.y + 1};
	while (true)
	{
		if (hor.y >= ver.y)
		{
			ray = _wall_hit_ver_sw(&map[(int)ver.y][(int)ver.x - 1],
					&ver, 1 / angle, player_angle);
			if (ray.hit.x != -1)
				return (ray);
		}
		else
		{
			ray = _wall_hit_hor_sw(&map[(int)hor.y][(int)hor.x],
					&hor, angle * -1, player_angle);
			if (ray.hit.x != -1)
				return (ray);
		}
	}
}
