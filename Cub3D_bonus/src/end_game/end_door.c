/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_door.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:07:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/11 17:58:18 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	change_adjacent_wall_end(enum e_orientation orient, t_map **map,
		t_vector2 map_pos)
{
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

void	end_step_door_open(double time, t_map *map_cell, t_map **map,
		t_end *end)
{
	t_door	*door;

	door = map_cell->arg;
	if (door->is_opening_door == 1)
	{
		door->door_percent += time * (SPEEP_UNLOCK_DOOR_OPENING / 2.);
		if (door->door_percent > 89)
		{
			map_cell->type ^= WALL;
			door->door_percent = 90;
			door->is_opening_door = 0;
			change_adjacent_wall_end(end->orient, map, door->map_pos);
		}
	}
}
