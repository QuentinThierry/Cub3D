/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_door.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:07:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/05 14:47:42 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	change_adjacent_wall_end(enum e_orientation orient, t_map **map, t_vector2 map_pos)
{
	printf("set up door next\n");
	if (orient == e_south || orient == e_west)
	{
		if (is_only_wall(map[map_pos.y][map_pos.x + 1].type))
			map[map_pos.y][map_pos.x + 1].type |= DOOR_WEST;
		if (is_only_wall(map[map_pos.y][map_pos.x - 1].type))
			map[map_pos.y][map_pos.x - 1].type |= DOOR_EAST;
		if (is_only_wall(map[map_pos.y + 1][map_pos.x].type))
			map[map_pos.y + 1][map_pos.x].type |= DOOR_NORTH;
		if (is_only_wall(map[map_pos.y - 1][map_pos.x].type))
			map[map_pos.y - 1][map_pos.x].type |= DOOR_SOUTH;
	}
	else
	{
		if (is_only_wall(map[map_pos.y][map_pos.x + 1].type))
			map[map_pos.y][map_pos.x + 1].type |= DOOR_WEST_END;
		if (is_only_wall(map[map_pos.y][map_pos.x - 1].type))
			map[map_pos.y][map_pos.x - 1].type |= DOOR_EAST_END;
		if (is_only_wall(map[map_pos.y + 1][map_pos.x].type))
			map[map_pos.y + 1][map_pos.x].type |= DOOR_NORTH_END;
		if (is_only_wall(map[map_pos.y - 1][map_pos.x].type))
			map[map_pos.y - 1][map_pos.x].type |= DOOR_SOUTH_END;
	}
}

void	end_step_door_open(double time, t_map *map_cell, t_map **map, t_end *end)
{
	t_door	*door;
	
	door = map_cell->arg;
	if (door->is_opening_door == 1)
	{
		door->door_percent += time * (SPEEP_UNLOCK_DOOR_OPENING / 2.);
		// door->time = time;
		if (door->door_percent > 89)
		{
			map_cell->type ^= WALL;
			door->door_percent = 90;
			door->is_opening_door = 0;
			change_adjacent_wall_end(end->orient, map, door->map_pos);
		}
	}
}

t_dvector2	end_door_hit_hor_se(t_dvector2 hit, float step, float door_angle, float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

	if (door_angle == 0)
	{
		if (hit.x + step / 2 >= ((int)hit.x + 1))
			return ((t_dvector2){-2, -2});
		return ((t_dvector2){hit.x + step / 2, hit.y + 0.5});
	}
	a = hit.x + step / 2 - (int)hit.x;
	l = a * sin((90 - (180 - player_angle)) * TO_RADIAN)
		/ sin((180 - door_angle - (90 - (180 - player_angle))) * TO_RADIAN);
	if (l <= 0.5)
	{
		y = sin(door_angle * TO_RADIAN) * l;
		x = cos(door_angle * TO_RADIAN) * l;
		return ((t_dvector2){(int)hit.x + x, (int)hit.y + 0.5 - y});
	}
	a = 1 - (hit.x + step / 2 - (int)hit.x);
	if (player_angle - 90 + door_angle < 90 && a < 0)
		return ((t_dvector2){-1, -1});
	l = a * sin((180 - player_angle + 90) * TO_RADIAN)
		/ sin((180 - door_angle - (180 - player_angle + 90)) * TO_RADIAN);
	if (l > 0.5 || l < 0)
		return ((t_dvector2){-1, -1});
	x = cos(door_angle * TO_RADIAN) * l;
	y = sin(door_angle * TO_RADIAN) * l;
	return ((t_dvector2){(int)hit.x + 1 - x, (int)hit.y + 0.5 - y});
}

t_dvector2	end_door_hit_ver_sw(t_dvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

	if (door_angle == 0)
	{
		if (hit.y + step / 2 >= ((int)hit.y + 1))
			return ((t_dvector2){-2, -2});
		return ((t_dvector2){hit.x - 0.5, hit.y + step / 2});
	}
	a = hit.y + step / 2 - (int)hit.y;
	l = a * sin((player_angle - 180) * TO_RADIAN)
		/ sin((180 - door_angle - (player_angle - 180)) * TO_RADIAN);
	if (l <= 0.5)
	{
		x = sin(door_angle * TO_RADIAN) * l;
		y = cos(door_angle * TO_RADIAN) * l;
		return ((t_dvector2){(int)hit.x - 0.5 + x, (int)hit.y + y});
	}
	a = ((int)hit.y + 1) - (hit.y + step / 2);
	if (player_angle - 180 + door_angle < 90 && a < 0)
		return ((t_dvector2){-1, -1});
	l = a * sin((360 - player_angle) * TO_RADIAN)
		/ sin((180 - (360 - player_angle) - door_angle) * TO_RADIAN);
	if (l > 0.5 || l < 0)
		return ((t_dvector2){-1, -1});
	x = sin(door_angle * TO_RADIAN) * l;
	y = cos(door_angle * TO_RADIAN) * l;
	return ((t_dvector2){(int)hit.x - 0.5 + x, (int)hit.y + 1 - y});
}

t_dvector2	end_door_hit_hor_sw(t_dvector2 hit, float step, float door_angle, float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

	if (door_angle == 0)
	{
		if (hit.x + step / 2 <= ((int)hit.x))
			return ((t_dvector2){-2, -2});
		return ((t_dvector2){hit.x + step / 2, hit.y + 0.5});
	}
	a = 1 - (hit.x + step / 2 - (int)hit.x);
	l = a * sin((90 - (player_angle - 180)) * TO_RADIAN)
		/ sin((180 - door_angle - (90 - (player_angle - 180))) * TO_RADIAN);
	if (l <= 0.5)
	{
		y = sin(door_angle * TO_RADIAN) * l;
		x = cos(door_angle * TO_RADIAN) * l;
		return ((t_dvector2){(int)hit.x + 1 - x, (int)hit.y + 0.5 - y});
	}
	a = hit.x + step / 2 - (int)hit.x;
	if (player_angle - 180 + door_angle < 90 && a < 0)
		return ((t_dvector2){-1, -1});
	if (player_angle - 180 + door_angle >= 90 && a > 0)
		return ((t_dvector2){-1, -1});
	l = a * sin((90 + player_angle - 180) * TO_RADIAN)
		/ sin((180 - door_angle - (90 + player_angle - 180)) * TO_RADIAN);
	if (l > 0.5 || l < 0)
		return ((t_dvector2){-1, -1});
	x = cos(door_angle * TO_RADIAN) * l;
	y = sin(door_angle * TO_RADIAN) * l;
	return ((t_dvector2){(int)hit.x + x, (int)hit.y + 0.5 - y});
}

t_dvector2	end_door_hit_ver_nw(t_dvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

	if (door_angle == 0)
	{
		if (hit.y + step / 2 < ((int)hit.y))
			return ((t_dvector2){-2, -2});
		return ((t_dvector2){hit.x - 0.5, hit.y + step / 2});
	}
	a = ((int)hit.y + 1) - (hit.y + step / 2);
	l = a * sin((360 - player_angle) * TO_RADIAN)
		/ sin((180 - (360 - player_angle) - door_angle) * TO_RADIAN);
	if (l <= 0.5)
	{
		x = sin(door_angle * TO_RADIAN) * l;
		y = cos(door_angle * TO_RADIAN) * l;
		return ((t_dvector2){(int)hit.x - 0.5 + x, (int)hit.y + 1 - y});
	}
	a = hit.y + step / 2 - (int)hit.y;
	if (player_angle - 270 + door_angle < 90 && a < 0)
		return ((t_dvector2){-1, -1});
	if (player_angle - 270 + door_angle >= 90 && a > 0)
		return ((t_dvector2){-1, -1});
	l = a * sin((player_angle - 180) * TO_RADIAN)
		/ sin((180 - door_angle - (player_angle - 180)) * TO_RADIAN);
	if (l > 0.5)
		return ((t_dvector2){-1, -1});
	x = sin(door_angle * TO_RADIAN) * l;
	y = cos(door_angle * TO_RADIAN) * l;
	return ((t_dvector2){(int)hit.x - 0.5 + x, (int)hit.y + y});
}
