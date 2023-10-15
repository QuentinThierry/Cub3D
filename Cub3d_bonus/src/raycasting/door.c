/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:20:37 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 19:16:59 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	_suppr_door_open(t_map **map, t_vector2 map_pos)
{
	if ((map[map_pos.y][map_pos.x + 1].type & DOOR_WEST) == DOOR_WEST)
		map[map_pos.y][map_pos.x + 1].type &= ~DOOR_WEST;
	if ((map[map_pos.y][map_pos.x - 1].type & DOOR_EAST) == DOOR_EAST)
		map[map_pos.y][map_pos.x - 1].type &= ~DOOR_EAST;
	if ((map[map_pos.y + 1][map_pos.x].type & DOOR_NORTH) == DOOR_NORTH)
		map[map_pos.y + 1][map_pos.x].type &= ~DOOR_NORTH;
	if ((map[map_pos.y - 1][map_pos.x].type & DOOR_SOUTH) == DOOR_SOUTH)
		map[map_pos.y - 1][map_pos.x].type &= ~DOOR_SOUTH;
}

static void	_change_adjacent_wall(t_map **map, t_vector2 map_pos,
			bool is_add_flag)
{
	bool	east_west;

	east_west = (is_only_wall(map[map_pos.y][map_pos.x + 1].type)
			&& is_only_wall(map[map_pos.y][map_pos.x - 1].type));
	if (is_add_flag)
	{
		if (east_west)
		{
			if (is_only_wall(map[map_pos.y][map_pos.x + 1].type))
				map[map_pos.y][map_pos.x + 1].type |= DOOR_WEST;
			if (is_only_wall(map[map_pos.y][map_pos.x - 1].type))
				map[map_pos.y][map_pos.x - 1].type |= DOOR_EAST;
		}
		else
		{
			if (is_only_wall(map[map_pos.y + 1][map_pos.x].type))
				map[map_pos.y + 1][map_pos.x].type |= DOOR_NORTH;
			if (is_only_wall(map[map_pos.y - 1][map_pos.x].type))
				map[map_pos.y - 1][map_pos.x].type |= DOOR_SOUTH;
		}
	}
	else
		_suppr_door_open(map, map_pos);
}

static void	_step_door_open(t_door *door, double time, t_map *map_cell,
				t_map **map)
{
	int	speed;

	speed = SPEEP_DOOR_OPENING;
	if ((map_cell->type & DOOR_UNLOCK) == DOOR_UNLOCK)
		speed = SPEEP_UNLOCK_DOOR_OPENING;
	if (door->is_opening_door == 1)
	{
		door->door_percent += time * speed;
		if (door->door_percent > 89)
		{
			map_cell->type &= ~WALL;
			door->door_percent = 90;
			door->is_opening_door = 0;
			_change_adjacent_wall(map, door->map_pos, true);
		}
		return ;
	}
	door->door_percent -= time * speed;
	_change_adjacent_wall(map, door->map_pos, false);
	door->door_percent *= (door->door_percent > 0);
	door->is_opening_door *= (door->door_percent > 0);
}

void	update_doors(t_map **doors, int nb_doors, double time, t_map **map)
{
	int	i;

	i = 0;
	while (i < nb_doors)
	{
		if (((t_door *)doors[i]->arg)->is_opening_door != 0)
			_step_door_open(doors[i]->arg, time, doors[i], map);
		i++;
	}
}
