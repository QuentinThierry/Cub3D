/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:20:37 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 19:26:34 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

float	get_texture_door(t_ray ray)
{
	t_fvector2	delta;
	float		dist;
	bool		left_door;

	left_door = false;
	if (ray.orient == e_south || ray.orient == e_north)
	{
		if (ray.hit.x - (int)ray.hit.x < 0.5)
			delta.x = ray.hit.x - (int)ray.hit.x;
		else
		{
			delta.x = ((int)ray.hit.x + 1) - ray.hit.x;
			left_door = true;
		}
		delta.y = fabs(ray.hit.y - ((int)ray.hit.y + 0.5));
	}
	else
	{
		delta.x = fabs(ray.hit.x - ((int)ray.hit.x + 0.5));
		if (ray.hit.y - (int)ray.hit.y < 0.5)
			delta.y = ray.hit.y - (int)ray.hit.y;
		else
		{
			left_door = true;
			delta.y = ((int)ray.hit.y + 1) - ray.hit.y;
		}
	}
	dist = sqrtf((delta.x * delta.x + delta.y * delta.y));
	if (left_door)
		dist = 1 - dist;
	return (dist);
}

void	change_adjacent_wall(t_map **map, t_vector2 map_pos, bool is_add_flag)
{
	bool	east_west;

	east_west = is_only_wall(map[map_pos.y][map_pos.x + 1].type)
			&& is_only_wall(map[map_pos.y][map_pos.x - 1].type);
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
}

bool	possible_to_open_door(enum e_orientation orient, t_dvector2 hit,
		t_dvector2 pos)
{
	if (orient == e_north && hit.y - pos.y < DIST_TO_WALL)
		return (false);
	else if (orient == e_south && (hit.y + 1 - pos.y) * -1 < DIST_TO_WALL)
		return (false);
	else if (orient == e_west && hit.x - pos.x < DIST_TO_WALL)
		return (false);
	else if (orient == e_east && (hit.x + 1 - pos.x) * -1 < DIST_TO_WALL)
		return (false);
	return (true);
}

void	open_door(t_game *game)
{
	t_ray			ray;
	t_door			*door;

	ray = get_object_hit((t_launch_ray){'\0', DOOR, 1}, game->map,
			game->player->f_pos, game->player->angle);
	if (ray.hit.x != -1)
	{
		if ((game->map[(int)ray.hit.y][(int)ray.hit.x].type & DOOR_LOCK) == DOOR_LOCK)
		{
			play_sound_fail(game, &game->map[(int)ray.hit.y][(int)ray.hit.x], game->music_array);
			return ;
		}
		if ((game->map[(int)ray.hit.y][(int)ray.hit.x].type & DOOR_UNLOCK) == DOOR_UNLOCK)
			return ;
		if ((game->map[(int)ray.hit.y][(int)ray.hit.x].type & EXIT) == EXIT)
			return (end_of_the_game(game, ray.orient));
		if ((game->map[(int)ray.hit.y][(int)ray.hit.x].type & MUSIC) == MUSIC)
			play_music(&game->map[(int)ray.hit.y][(int)ray.hit.x],
				game->music_array, game->map[(int)ray.hit.y][(int)ray.hit.x].music, IS_PLAYING_MUSIC);
		if ((game->map[(int)ray.hit.y][(int)ray.hit.x].type & NARRATOR) == NARRATOR)
		{
			play_narrator(game, &game->map[(int)ray.hit.y][(int)ray.hit.x], game->music_array);
			game->map[(int)ray.hit.y][(int)ray.hit.x].type &= ~NARRATOR;
		}
		door = game->map[(int)ray.hit.y][(int)ray.hit.x].arg;
		if (door->is_opening_door == 1)
			door->is_opening_door = -1;
		else if (door->is_opening_door == -1)
			door->is_opening_door = 1;
		else
		{
			if (door->door_percent == 0)
				door->is_opening_door = 1;
			else
			{
				if (!possible_to_open_door(ray.orient, ray.hit, game->player->f_pos))
					return ;
				door->is_opening_door = -1;
				game->map[(int)ray.hit.y][(int)ray.hit.x].type |= WALL;
			}
		}
	}
}

static void	_step_door_open(t_door *door, double time, t_map *map_cell,
				t_map **map)
{
	int		speed;

	speed = SPEEP_DOOR_OPENING;
	if ((map_cell->type & DOOR_UNLOCK) == DOOR_UNLOCK)
		speed = SPEEP_UNLOCK_DOOR_OPENING;
	if (door->is_opening_door == 1)
	{
		door->door_percent += time * speed;
		if (door->door_percent > 89)
		{
			map_cell->type ^= WALL;
			door->door_percent = 90;
			door->is_opening_door = 0;
			change_adjacent_wall(map, door->map_pos, true);
			if ((map_cell->type & DOOR_UNLOCK) == DOOR_UNLOCK)
				map_cell->type ^= DOOR;
		}
	}
	else
	{
		door->door_percent -= time * speed;
		change_adjacent_wall(map, door->map_pos, false);
		if (door->door_percent <= 0)
		{
			door->door_percent = 0;
			door->is_opening_door = 0;
			if ((map_cell->type & DOOR_UNLOCK) == DOOR_UNLOCK)
				map_cell->type ^= DOOR;
		}
	}
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
