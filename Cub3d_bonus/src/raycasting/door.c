/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:20:37 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/27 16:59:36 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

// xy (1, 1)
t_dvector2	door_hit_ver_se(t_dvector2 hit, float step, float door_angle,
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
		return ((t_dvector2){hit.x + 0.5, hit.y + step / 2});
	}
	a = hit.y + step / 2 - (int)hit.y;
	l = a * sin((180 - player_angle) * TO_RADIAN)
		/ sin((180 - door_angle - (180 - player_angle)) * TO_RADIAN);
	if (l <= 0.5)
	{
		x = sin(door_angle * TO_RADIAN) * l;
		y = cos(door_angle * TO_RADIAN) * l;
		return ((t_dvector2){(int)hit.x + 0.5 - x, (int)hit.y + y});
	}
	a = ((int)hit.y + 1) - (hit.y + step / 2);
	if ((90 - player_angle) + door_angle >= 90 && a > 0)
		return ((t_dvector2){-1, -1});
	l = a * sin(player_angle * TO_RADIAN)
		/ sin((180 - player_angle - door_angle) * TO_RADIAN);
	if (l > 0.5 || l < 0)
		return ((t_dvector2){-1, -1});
	x = sin(door_angle * TO_RADIAN) * l;
	y = cos(door_angle * TO_RADIAN) * l;
	return ((t_dvector2){(int)hit.x + 0.5 - x, (int)hit.y + 1 - y});
}

t_dvector2	door_hit_hor_se(t_dvector2 hit, float step, float door_angle, float player_angle)
{
	float	a;
	float	r;

	if (hit.x + step / 2 >= (int)hit.x + 1)
		return ((t_dvector2){-2, -2});
	if (door_angle == 0)
		return ((t_dvector2){hit.x + step / 2, hit.y + 0.5});
	player_angle -= 90;
	player_angle = 180 - player_angle;
	if (door_angle < 180 - player_angle)
	{
		r = (hit.x + step / 2) - (int)hit.x;
		a = (r * sinf(player_angle * TO_RADIAN))
			/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
		if (a <= 0.5)
			return ((t_dvector2){(int)hit.x + (cosf(door_angle * TO_RADIAN) * a),
				hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
		if (hit.x + step < (int)hit.x + 0.5)
			return ((t_dvector2){-1, -1});
	}
	r = (int)(hit.x + 1) - (hit.x + step / 2);
	player_angle = -player_angle + 180;
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_dvector2){-1, -1});
	return ((t_dvector2){(int)hit.x + 1 - (cosf(door_angle * TO_RADIAN) * a),
		hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
}

// xy (1, -1)
t_dvector2	door_hit_ver_ne(t_dvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

	if (door_angle == 0)
	{
		if (hit.y + step / 2 <= ((int)hit.y))
			return ((t_dvector2){-2, -2});
		return ((t_dvector2){hit.x + 0.5, hit.y + step / 2});
	}
	a = (int)hit.y + 1 - (hit.y + step / 2);
	l = a * sin(player_angle * TO_RADIAN)
		/ sin((180 - door_angle - player_angle) * TO_RADIAN);
	if (l <= 0.5)
	{
		y = sin(door_angle * TO_RADIAN) * l;
		x = cos(door_angle * TO_RADIAN) * l;
		return ((t_dvector2){(int)hit.x + 0.5 - y, (int)hit.y + 1 - x});
	}
	a = (hit.y + step / 2) - (int)hit.y;
	if ((90 - player_angle) + door_angle < 90 && a < 0)
		return ((t_dvector2){-1, -1});
	if ((90 - player_angle) + door_angle >= 90 && a > 0)
		return ((t_dvector2){-1, -1});
	l = a * sin((180 - player_angle) * TO_RADIAN)
		/ sin((180 - door_angle - (180 - player_angle)) * TO_RADIAN);
	if (l > 0.5 || l < -0.5)
		return ((t_dvector2){-1, -1});
	x = cos(door_angle * TO_RADIAN) * l;
	y = sin(door_angle * TO_RADIAN) * l;
	return ((t_dvector2){(int)hit.x + 0.5 - y, (int)hit.y + x});
}

t_dvector2	door_hit_hor_ne(t_dvector2 hit, float step, float door_angle, float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

	if (door_angle == 0)
	{
		if (hit.x + step / 2 >= ((int)hit.x + 1))
			return ((t_dvector2){-2, -2});
		return ((t_dvector2){hit.x + step / 2, hit.y - 0.5});
	}
	a = hit.x + step / 2 - (int)hit.x;
	l = a * sin((90 - player_angle) * TO_RADIAN)
		/ sin((180 - door_angle - (90 - player_angle)) * TO_RADIAN);
	if (l <= 0.5)
	{
		y = sin(door_angle * TO_RADIAN) * l;
		x = cos(door_angle * TO_RADIAN) * l;
		return ((t_dvector2){(int)hit.x + x, (int)hit.y - 0.5 + y});
	}
	a = 1 - (hit.x + step / 2 - (int)hit.x);
	if (player_angle + door_angle < 90 && a < 0)
		return ((t_dvector2){-1, -1});
	if (player_angle + door_angle >= 90 && a > 0)
		return ((t_dvector2){-1, -1});
	l = a * sin((180 - (90 - player_angle)) * TO_RADIAN)
		/ sin((180 - door_angle - (180 - (90 - player_angle))) * TO_RADIAN);
	if (l > 0.5)
		return ((t_dvector2){-1, -1});
	x = cos(door_angle * TO_RADIAN) * l;
	y = sin(door_angle * TO_RADIAN) * l;
	return ((t_dvector2){(int)hit.x + 1 - x, (int)hit.y - 0.5 + y});
}

// xy (-1, 1)
t_dvector2	door_hit_ver_sw(t_dvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	a;
	float	r;

	if (hit.y + step / 2 > (int)hit.y + 1)
		return ((t_dvector2){-2, -2});
	if (door_angle == 0)
		return ((t_dvector2){hit.x - 0.5, hit.y + step / 2});
	player_angle = 360 - player_angle;
	if (door_angle < 180 - player_angle)
	{
		r = (hit.y + step / 2) - (int)hit.y;
		a = (r * sinf(player_angle * TO_RADIAN))
			/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
		if (a <= 0.5)
			return ((t_dvector2){hit.x - 0.5 - sinf(door_angle * TO_RADIAN) * a,
				(int)hit.y + (cosf(door_angle * TO_RADIAN) * a)});
		if (hit.y + step < (int)hit.y + 0.5)
			return ((t_dvector2){-1, -1});
	}
	r = (int)(hit.y + 1) - (hit.y + step / 2);
	player_angle = -player_angle + 180;
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_dvector2){-1, -1});
	return ((t_dvector2){hit.x - 0.5 - sinf(door_angle * TO_RADIAN) * a,
		(int)hit.y + 1 - (cosf(door_angle * TO_RADIAN) * a)});
}

t_dvector2	door_hit_hor_sw(t_dvector2 hit, float step, float door_angle, float player_angle)
{
	float	a;
	float	r;

	if (hit.x + step / 2 <= (int)hit.x)
		return ((t_dvector2){-2, -2});
	if (door_angle == 0)
		return ((t_dvector2){hit.x + step / 2, hit.y + 0.5});
	player_angle = player_angle - 90;
	if (door_angle < 180 - player_angle)
	{
		r = (int)(hit.x + 1) - (hit.x + step / 2);
		a = (r * sinf(player_angle * TO_RADIAN))
			/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
		if (a <= 0.5)
			return ((t_dvector2){(int)hit.x + 1 - (cosf(door_angle * TO_RADIAN) * a),
				hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
		if (hit.x + step > (int)hit.x + 0.5)
			return ((t_dvector2){-1, -1});
	}
	player_angle = 180 - player_angle;
	r = (hit.x + step / 2) - (int)hit.x;
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_dvector2){-1, -1});
	return ((t_dvector2){(int)hit.x + (cosf(door_angle * TO_RADIAN) * a),
		hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
}

// xy (-1, -1)
t_dvector2	door_hit_ver_nw(t_dvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	a;
	float	r;

	if (hit.y + step / 2 <= (int)hit.y)
		return ((t_dvector2){-2, -2});
	if (door_angle == 0)
		return ((t_dvector2){hit.x - 0.5, hit.y + step / 2});
	player_angle = player_angle - 180;
	if (door_angle < 180 - player_angle)
	{
		r = (int)(hit.y + 1) - (hit.y + step / 2);
		a = (r * sinf(player_angle * TO_RADIAN))
			/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
		if (a <= 0.5)
			return ((t_dvector2){hit.x - 0.5 - sinf(door_angle * TO_RADIAN) * a,
				(int)hit.y + 1 - (cosf(door_angle * TO_RADIAN) * a)});
		if (hit.y + step > (int)hit.y + 0.5)
			return ((t_dvector2){-1, -1});
	}
	r = (hit.y + step / 2) - (int)hit.y;
	player_angle = 180 - player_angle;
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_dvector2){-1, -1});
	return ((t_dvector2){hit.x - 0.5 - sinf(door_angle * TO_RADIAN) * a,
		(int)hit.y + (cosf(door_angle * TO_RADIAN) * a)});
	
}

t_dvector2	door_hit_hor_nw(t_dvector2 hit, float step, float door_angle, float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

	if (door_angle == 0)
	{
		if (hit.x + step / 2 <= (int)hit.x)
			return ((t_dvector2){-2, -2});
		return ((t_dvector2){hit.x + step / 2, hit.y - 0.5});
	}
	a = (int)hit.x + 1 - (hit.x + step / 2);
	player_angle = 360 - player_angle;
	l = a * sin((180 - player_angle - 90) * TO_RADIAN)
		/ sin((player_angle + (180 - door_angle - 90)) * TO_RADIAN);
	if (l <= 0.5)
	{
		y = sin(door_angle * TO_RADIAN) * l;
		x = cos(door_angle * TO_RADIAN) * l;
		return ((t_dvector2){(int)hit.x + 1 - x, (int)hit.y - 0.5 + y});
	}
	a = hit.x + step / 2 - (int)hit.x;
	if (player_angle + door_angle < 90 && a < 0)
		return ((t_dvector2){-1, -1});
	if (player_angle + door_angle >= 90 && a > 0)
		return ((t_dvector2){-1, -1});
	l = a * sin((180 - (180 - player_angle - 90)) * TO_RADIAN)
		/ sin((180 - door_angle - 90 - player_angle) * TO_RADIAN);
	if (l > 0.5 || l < -0.5)
		return ((t_dvector2){-1, -1});
	x = cos(door_angle * TO_RADIAN) * l;
	y = sin(door_angle * TO_RADIAN) * l;
	return ((t_dvector2){(int)hit.x + x, (int)hit.y - 0.5 + y});
}

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
	if (is_add_flag)
	{
		if (map[map_pos.y][map_pos.x + 1].type == WALL)
			map[map_pos.y][map_pos.x + 1].type |= DOOR_WEST;
		if (map[map_pos.y][map_pos.x - 1].type == WALL)
			map[map_pos.y][map_pos.x - 1].type |= DOOR_EAST;
		if (map[map_pos.y + 1][map_pos.x].type == WALL)
			map[map_pos.y + 1][map_pos.x].type |= DOOR_NORTH;
		if (map[map_pos.y - 1][map_pos.x].type == WALL)
			map[map_pos.y - 1][map_pos.x].type |= DOOR_SOUTH;
	}
	else
	{
		if ((map[map_pos.y][map_pos.x + 1].type & DOOR_WEST) == DOOR_WEST)
			map[map_pos.y][map_pos.x + 1].type ^= DOOR_WEST;
		if ((map[map_pos.y][map_pos.x - 1].type & DOOR_EAST) == DOOR_EAST)
			map[map_pos.y][map_pos.x - 1].type ^= DOOR_EAST;
		if ((map[map_pos.y + 1][map_pos.x].type & DOOR_NORTH) == DOOR_NORTH)
			map[map_pos.y + 1][map_pos.x].type ^= DOOR_NORTH;
		if ((map[map_pos.y - 1][map_pos.x].type & DOOR_SOUTH) == DOOR_SOUTH)
			map[map_pos.y - 1][map_pos.x].type ^= DOOR_SOUTH;
	}
}

bool	possible_to_open_door(enum e_orientation orient, t_dvector2 hit, t_dvector2 pos)
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

	ray = get_object_hit((t_launch_ray){'\0', DOOR, 1}, game->map, game->player->f_real_pos, game->player->angle);
	if (ray.hit.x != -1)
	{
		if ((game->map[(int)ray.hit.y][(int)ray.hit.x].type & DOOR_LOCK) == DOOR_LOCK)
		{
			play_sound_fail(game, &game->map[(int)ray.hit.y][(int)ray.hit.x], game->music_array);
			return ;
		}
		if ((game->map[(int)ray.hit.y][(int)ray.hit.x].type & EXIT) == EXIT)
			return (end_of_the_game(game, ray.orient));
		door = game->map[(int)ray.hit.y][(int)ray.hit.x].arg;
		if (door->is_opening_door == 1)
		{
			door->is_opening_door = -1;
			door->time = game->time;
		}
		else if (door->is_opening_door == -1)
		{
			door->is_opening_door = 1;
			door->time = game->time;
		}
		else
		{
			if (door->door_percent == 0)
			{
				door->is_opening_door = 1;
				door->time = game->time;
			}
			else
			{
				if (!possible_to_open_door(ray.orient, ray.hit, game->player->f_real_pos))
					return ;
				door->is_opening_door = -1;
				game->map[(int)ray.hit.y][(int)ray.hit.x].type |= WALL;
				door->time = game->time;
			}
		}
	}
}

static void	_step_door_open(t_door *door, long time, t_map *map_cell, t_map **map)
{
	long	tmp;

	tmp = time - door->time;
	if (door->is_opening_door == 1)
	{
		door->door_percent += tmp / 1000.0 * SPEEP_DOOR_OPENING;
		door->time = time;
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
		door->door_percent -= tmp / 1000.0 * SPEEP_DOOR_OPENING;
		door->time = time;
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

void	update_doors(t_map **doors, int	nb_doors, long time, t_map **map)
{
	int	i;

	i = 0;
	while (i < nb_doors)
	{
		if (((t_door *)doors[i]->arg)->is_opening_door != 0)
		{
			_step_door_open(doors[i]->arg, time, doors[i], map);
		}
		i++;
	}
}
