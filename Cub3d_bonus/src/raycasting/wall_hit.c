/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:04:05 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/11 14:56:14 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

// xy (1, 1)
static t_ray	_get_wall_hit_se(t_dvector2 fpos,
								t_map **map, float angle) // change game to game->time ?
{
	t_dvector2	step;
	t_dvector2	comp;
	t_vector2	map_pos;
	t_dvector2	door;
	float		player_angle;

	player_angle = angle;
	angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_dvector2){angle, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x].type & OBJECT) == OBJECT)
				((t_object *)map[(int)(comp.y)][map_pos.x].arg)->visited = true;
			else if ((map[(int)(comp.y)][map_pos.x].type & WALL) == WALL)
			{
				if ((map[(int)(comp.y)][map_pos.x].type & DOOR_CLOSE) == DOOR_CLOSE)
				{
					door = door_hit_ver_se((t_dvector2){map_pos.x, comp.y}, step.y
						, (float)((t_door *)map[(int)(comp.y)][map_pos.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_west});
				}
				else
					return ((t_ray){{map_pos.x, comp.y}, e_west});
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else 
		{
			if ((map[map_pos.y][(int)(comp.x)].type & OBJECT) == OBJECT)
				((t_object *)map[map_pos.y][(int)(comp.x)].arg)->visited = true;
			else if ((map[map_pos.y][(int)(comp.x)].type & WALL) == WALL)
			{
				if ((map[map_pos.y][(int)(comp.x)].type & DOOR_CLOSE) == DOOR_CLOSE)
				{
					door = door_hit_hor_se((t_dvector2){comp.x, map_pos.y}, step.x
						, (float)((t_door *)map[map_pos.y][(int)comp.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_north});
				}
				else
					return ((t_ray){{comp.x, map_pos.y}, e_north});
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static t_ray	_get_wall_hit_ne(t_dvector2 fpos,
								t_map **map, float angle)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_dvector2	door;
	t_vector2	map_pos;
	float		player_angle;

	player_angle = angle;
	angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_dvector2){angle, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x].type & OBJECT) == OBJECT)
				((t_object *)map[(int)(comp.y)][map_pos.x].arg)->visited = true;
			else if ((map[(int)(comp.y)][map_pos.x].type & WALL) == WALL)
			{
				if ((map[(int)(comp.y)][map_pos.x].type & DOOR_CLOSE) == DOOR_CLOSE)
				{
					door = door_hit_ver_ne((t_dvector2){map_pos.x, comp.y}, step.y
						, (float)((t_door *)map[(int)(comp.y)][map_pos.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_west});
				}
				else
					return ((t_ray){{map_pos.x, comp.y}, e_west});
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if ((map[map_pos.y - 1][(int)(comp.x)].type & OBJECT) == OBJECT)
				((t_object *)map[map_pos.y - 1][(int)(comp.x)].arg)->visited = true;
			else if ((map[map_pos.y - 1][(int)(comp.x)].type & WALL) == WALL)
			{
				if ((map[map_pos.y - 1][(int)(comp.x)].type & DOOR_CLOSE) == DOOR_CLOSE)
				{
					door = door_hit_hor_ne((t_dvector2){comp.x, map_pos.y}, step.x
						, (float)((t_door *)map[map_pos.y - 1][(int)comp.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_south});
				}
				else
					return ((t_ray){{comp.x, map_pos.y}, e_south});
			}

			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static t_ray	_get_wall_hit_sw(t_dvector2 fpos,
								t_map **map, float angle)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_dvector2	door;
	t_vector2	map_pos;
	float		player_angle;

	player_angle = angle;
	angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_dvector2){angle * -1, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x - 1].type & OBJECT) == OBJECT)
				((t_object *)map[(int)(comp.y)][map_pos.x - 1].arg)->visited = true;
			else if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == WALL)
			{
				if ((map[(int)(comp.y)][map_pos.x - 1].type & DOOR_CLOSE) == DOOR_CLOSE)
				{
					door = door_hit_ver_sw((t_dvector2){map_pos.x, comp.y}, step.y
						, (float)((t_door *)map[(int)(comp.y)][map_pos.x - 1].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_east});
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
				if ((map[map_pos.y][(int)(comp.x)].type & DOOR_CLOSE) == DOOR_CLOSE)
				{
					door = door_hit_hor_sw((t_dvector2){comp.x, map_pos.y}, step.x
						, (float)((t_door *)map[map_pos.y][(int)comp.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_north});
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
static t_ray	_get_wall_hit_nw(t_dvector2 fpos,
								t_map **map, float angle)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_dvector2	door;
	t_vector2	map_pos;
	float		player_angle;

	player_angle = angle;
	angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_dvector2){angle * -1, 1 / angle * -1};
	while (true)
	{
		
		if (map_pos.y <= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x - 1].type & OBJECT) == OBJECT)
				((t_object *)map[(int)(comp.y)][map_pos.x - 1].arg)->visited = true;
			else if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == WALL)
			{
				if ((map[(int)(comp.y)][map_pos.x - 1].type & DOOR_CLOSE) == DOOR_CLOSE)
				{
					door = door_hit_ver_nw((t_dvector2){map_pos.x, comp.y}, step.y
						, (float)((t_door *)map[(int)(comp.y)][map_pos.x - 1].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_east});
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
				if ((map[map_pos.y - 1][(int)(comp.x)].type & DOOR_CLOSE) == DOOR_CLOSE)
				{
					door = door_hit_hor_nw((t_dvector2){comp.x, map_pos.y}, step.x
						, (float)((t_door *)map[map_pos.y - 1][(int)comp.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_south});
				}
				else
					return ((t_ray){{comp.x, map_pos.y}, e_south});
			}
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

t_ray	get_wall_hit(t_dvector2 fpos, t_map **map, float angle)
{
	t_vector2	sign;

	if ((map[(int)fpos.y][(int)fpos.x].type & OBJECT) == OBJECT)
		((t_object *)map[(int)fpos.y][(int)fpos.x].arg)->visited = true;
	sign = get_sign(angle);
	if (sign.x == 1 && sign.y == 1)
		return (_get_wall_hit_se(fpos, map, angle));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_wall_hit_ne(fpos, map, angle));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_wall_hit_sw(fpos, map, angle));
	else
		return (_get_wall_hit_nw(fpos, map, angle));
	return ((t_ray){});
}
