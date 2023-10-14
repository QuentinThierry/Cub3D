/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit_end.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:04:05 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/14 18:07:39 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_dvector2	end_door_hit_hor_sw(t_dvector2 hit, float step, float door_angle,
				float player_angle);
t_dvector2	end_door_hit_ver_sw(t_dvector2 hit, float step, float door_angle,
				float player_angle);
t_dvector2	end_door_hit_ver_nw(t_dvector2 hit, float step, float door_angle,
				float player_angle);

t_ray	get_wall_hit_ne_end(t_dvector2 fpos,
								t_map **map, t_info_wall *info);
t_ray	get_wall_hit_se_end(t_dvector2 fpos,
								t_map **map, t_info_wall *info);

// xy (-1, 1)
static t_ray	_get_wall_hit_sw_end(t_dvector2 fpos,
								t_map **map, t_info_wall *info)
{
	t_fvector2	step;
	t_dvector2	comp;
	t_dvector2	door;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * info->angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / info->angle;
	step = (t_fvector2){info->angle * -1, 1 / info->angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x - 1].type & OBJECT) == OBJECT)
				((t_object *)map[(int)(comp.y)][map_pos.x - 1].arg)->visited = true;
			else if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == WALL)
			{
				if ((map[(int)(comp.y)][map_pos.x - 1].type & DOOR) == DOOR)
				{
					door = end_door_hit_ver_sw((t_dvector2){map_pos.x, comp.y}, step.y,
							(float)((t_door *)map[(int)(comp.y)][map_pos.x - 1].arg)->door_percent, info->player_angle);
					if (door.x >= 0)
						return ((t_ray){door, e_east});
					else if (info->status == e_open_door && comp.y + step.y / 2 <= (int)comp.y + 1)
						return ((t_ray){{map_pos.x - .5, comp.y + step.y / 2}, e_end_screen});
				}
				else
					return ((t_ray){{map_pos.x, comp.y}, e_east});
			}
			comp.y += step.y;
			map_pos.x -= 1;
		}
		else
		{
			if ((map[map_pos.y][(int)(comp.x)].type & OBJECT) == OBJECT)
				((t_object *)map[map_pos.y][(int)(comp.x)].arg)->visited = true;
			else if ((map[map_pos.y][(int)(comp.x)].type & WALL) == WALL)
			{
				if ((map[map_pos.y][(int)(comp.x)].type & DOOR) == DOOR)
				{
					door = end_door_hit_hor_sw((t_dvector2){comp.x, map_pos.y}, step.x,
							(float)((t_door *)map[map_pos.y][(int)comp.x].arg)->door_percent, info->player_angle);
					if (door.x >= 0)
						return ((t_ray){door, e_north});
					else if (info->status == e_open_door && comp.x + step.x / 2. >= (int)comp.x)
						return ((t_ray){{comp.x + step.x / 2, map_pos.y + .5}, e_end_screen});
				}
				else
					return ((t_ray){{comp.x, map_pos.y}, e_north});
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static t_ray	_get_wall_hit_nw_end(t_dvector2 fpos,
								t_map **map, t_info_wall *info)
{
	t_fvector2	step;
	t_dvector2	comp;
	t_dvector2	door;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * info->angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / info->angle * -1;
	step = (t_fvector2){info->angle * -1, 1 / info->angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x - 1].type & OBJECT) == OBJECT)
				((t_object *)map[(int)(comp.y)][map_pos.x - 1].arg)->visited = true;
			else if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == WALL)
			{
				if ((map[(int)(comp.y)][map_pos.x - 1].type & DOOR) == DOOR)
				{
					door = end_door_hit_ver_nw((t_dvector2){map_pos.x, comp.y}, step.y,
							(float)((t_door *)map[(int)(comp.y)][map_pos.x - 1].arg)->door_percent, info->player_angle);
					if (door.x >= 0)
						return ((t_ray){door, e_east});
					else if (info->status == e_open_door && comp.y + step.y / 2 >= (int)comp.y)
						return ((t_ray){{map_pos.x - .5, comp.y + step.y / 2}, e_end_screen});
				}
				else
					return ((t_ray){{map_pos.x, comp.y}, e_east});
			}
			comp.y += step.y;
			map_pos.x -= 1;
		}
		else
		{
			if ((map[map_pos.y - 1][(int)(comp.x)].type & OBJECT) == OBJECT)
				((t_object *)map[map_pos.y - 1][(int)(comp.x)].arg)->visited = true;
			else if ((map[map_pos.y - 1][(int)(comp.x)].type & WALL) == WALL)
			{
				if ((map[map_pos.y - 1][(int)(comp.x)].type & DOOR) == DOOR)
				{
					door = door_hit_hor_nw((t_dvector2){comp.x, map_pos.y}, step.x,
							(float)((t_door *)map[map_pos.y - 1][(int)comp.x].arg)->door_percent, info->player_angle);
					if (door.x >= 0)
						return ((t_ray){door, e_south});
					else if (info->status == e_open_door && comp.x + step.x / 2 > (int)comp.x)
						return ((t_ray){{comp.x + step.x / 2, map_pos.y - .5}, e_end_screen});
				}
				else
					return ((t_ray){{comp.x, map_pos.y}, e_south});
			}
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

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
		return (_get_wall_hit_sw_end(fpos, map, &infos));
	else
		return (_get_wall_hit_nw_end(fpos, map, &infos));
}
