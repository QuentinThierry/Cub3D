/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_door.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:05:59 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 20:08:24 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

float	get_texture_door(t_ray ray)
{
	t_fvector2	delta;

	if (ray.orient == e_south || ray.orient == e_north)
	{
		delta.y = fabs(ray.hit.y - ((int)ray.hit.y + 0.5));
		if (ray.hit.x - (int)ray.hit.x < 0.5)
			delta.x = ray.hit.x - (int)ray.hit.x;
		else
		{
			delta.x = ((int)ray.hit.x + 1) - ray.hit.x;
			return (1 - sqrtf((delta.x * delta.x + delta.y * delta.y)));
		}
	}
	else
	{
		delta.x = fabs(ray.hit.x - ((int)ray.hit.x + 0.5));
		if (ray.hit.y - (int)ray.hit.y < 0.5)
			delta.y = ray.hit.y - (int)ray.hit.y;
		else
		{
			delta.y = ((int)ray.hit.y + 1) - ray.hit.y;
			return (1 - sqrtf((delta.x * delta.x + delta.y * delta.y)));
		}
	}
	return (sqrtf((delta.x * delta.x + delta.y * delta.y)));
}

static bool	_possible_to_open_door(enum e_orientation orient, t_dvector2 hit,
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

static void	_start_open_door(t_game *game, t_ray ray)
{
	t_door	*door;

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
			if (!_possible_to_open_door(ray.orient, ray.hit,
					game->player->f_pos))
				return ;
			door->is_opening_door = -1;
			game->map[(int)ray.hit.y][(int)ray.hit.x].type |= WALL;
		}
	}
}

void	open_door(t_game *game)
{
	t_ray	ray;
	t_type	type;
	t_map	*map_cell;

	ray = get_object_hit((t_launch_ray){'\0', DOOR, 1}, game->map,
			game->player->f_pos, game->player->angle);
	if (ray.hit.x == -1)
		return ;
	map_cell = &game->map[(int)ray.hit.y][(int)ray.hit.x];
	type = game->map[(int)ray.hit.y][(int)ray.hit.x].type;
	if ((type & DOOR_LOCK) == DOOR_LOCK)
		return (play_sound_fail(game, map_cell, game->music_array), (void)0);
	if ((type & DOOR_UNLOCK) == DOOR_UNLOCK)
		return ;
	if ((type & EXIT) == EXIT)
		return (end_of_the_game(game, ray.orient));
	if ((type & MUSIC) == MUSIC)
		play_music(map_cell, game->music_array, map_cell->music,
			IS_PLAYING_MUSIC);
	if ((type & NARRATOR) == NARRATOR)
	{
		play_narrator(game, map_cell, game->music_array);
		type &= ~NARRATOR;
	}
	_start_open_door(game, ray);
}
