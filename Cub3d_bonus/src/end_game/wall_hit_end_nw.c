/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit_end_nw.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:00:10 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 14:07:36 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_dvector2	end_door_hit_ver_nw(t_dvector2 hit, float step, float door_angle,
				float player_angle);

__attribute__((always_inline))
static inline t_ray	_wall_hit_ver_nw_end(t_map *map_cell, t_dvector2 *hit,
				float step, t_info_wall *info)
{
	t_dvector2	door;

	if ((map_cell->type & OBJECT) == OBJECT)
		((t_object *)map_cell->arg)->visited = true;
	else if ((map_cell->type & WALL) == WALL)
	{
		if ((map_cell->type & DOOR) == DOOR)
		{
			door = end_door_hit_ver_nw(*hit, step,
					(float)((t_door *)map_cell->arg)->door_percent,
					info->player_angle);
			if (door.x >= 0)
				return ((t_ray){door, e_east});
			else if (info->status == e_open_door
				&& hit->y + step / 2 >= (int)hit->y)
				return ((t_ray){{hit->x - .5, hit->y + step / 2},
					e_end_screen});
		}
		else
			return ((t_ray){(*hit), e_east});
	}
	hit->y += step;
	hit->x -= 1;
	return ((t_ray){{-1, -1}, -1});
}

__attribute__((always_inline))
static inline t_ray	_wall_hit_hor_nw_end(t_map *map_cell, t_dvector2 *hit,
				float step, t_info_wall *info)
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
					info->player_angle);
			if (door.x >= 0)
				return ((t_ray){door, e_south});
			else if (info->status == e_open_door
				&& hit->x + step / 2 > (int)hit->x)
				return ((t_ray){{hit->x + step / 2, hit->y - .5},
					e_end_screen});
		}
		else
			return ((t_ray){(*hit), e_south});
	}
	hit->x += step;
	hit->y += -1;
	return ((t_ray){{-1, -1}, -1});
}

// xy (-1, -1)
t_ray	get_wall_hit_nw_end(t_dvector2 fpos,
								t_map **map, t_info_wall *info)
{
	t_dvector2	ver;
	t_dvector2	hor;
	t_ray		ray;

	ver = (t_dvector2){(int)fpos.x,
		fpos.y + fabs(fpos.x - (int)fpos.x) / info->angle * -1};
	hor = (t_dvector2){fpos.x + fabs(fpos.y - (int)fpos.y) * info->angle * -1,
		(int)fpos.y};
	while (true)
	{
		if (hor.y <= ver.y)
		{
			ray = _wall_hit_ver_nw_end(&map[(int)ver.y][(int)ver.x - 1],
					&ver, 1 / info->angle * -1, info);
			if (ray.hit.x != -1)
				return (ray);
		}
		else
		{
			ray = _wall_hit_hor_nw_end(&map[(int)hor.y - 1][(int)hor.x],
					&hor, info->angle * -1, info);
			if (ray.hit.x != -1)
				return (ray);
		}
	}
}