/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:20:37 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/11 14:56:14 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

// xy (1, 1)
t_dvector2	door_hit_ver_se(t_dvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	a;
	float	r;

	if (hit.y + step / 2 >= (int)hit.y + 1)
		return ((t_dvector2){-1, -1});
	if (door_angle == 0)
		return ((t_dvector2){hit.x + 0.5, hit.y + step / 2});
	if (door_angle < 180 - player_angle)
	{
		r = (hit.y + step / 2) - (int)hit.y;
		a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
		if (a <= 0.5)
			return ((t_dvector2){hit.x + 0.5 + sinf(door_angle * TO_RADIAN) * a,
				(int)hit.y + (cosf(door_angle * TO_RADIAN) * a)});
		if (hit.y + step < (int)hit.y + 0.5)
			return ((t_dvector2){-1, -1});
	}
	r = (int)(hit.y + 1) - (hit.y + step / 2);
	player_angle = 180 - player_angle;
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_dvector2){-1, -1});
	return ((t_dvector2){hit.x + 0.5 + sinf(door_angle * TO_RADIAN) * a,
		(int)hit.y + 1 - (cosf(door_angle * TO_RADIAN) * a)});
}

t_dvector2	door_hit_hor_se(t_dvector2 hit, float step, float door_angle, float player_angle)
{
	float	a;
	float	r;

	if (hit.x + step / 2 >= (int)hit.x + 1)
		return ((t_dvector2){-1, -1});
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
	float	a;
	float	r;

	if (hit.y + step / 2 <= (int)hit.y)
		return ((t_dvector2){-1, -1});
	if (door_angle == 0)
		return ((t_dvector2){hit.x + 0.5, hit.y + step / 2});
	player_angle = 180 - player_angle;
	if (door_angle < 180 - player_angle)
	{
		r = (int)(hit.y + 1) - (hit.y + step / 2);
		a = (r * sinf(player_angle * TO_RADIAN))
			/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
		if (a <= 0.5)
			return ((t_dvector2){hit.x + 0.5 + sinf(door_angle * TO_RADIAN) * a,
				(int)hit.y + 1 - (cosf(door_angle * TO_RADIAN) * a)});
		if (hit.y + step > (int)hit.y + 0.5)
			return ((t_dvector2){-1, -1});
	}
	r = (hit.y + step / 2) - (int)hit.y;
	player_angle = -player_angle + 180;
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_dvector2){-1, -1});
	return ((t_dvector2){hit.x + 0.5 + sinf(door_angle * TO_RADIAN) * a,
		(int)hit.y + (cosf(door_angle * TO_RADIAN) * a)});
}

t_dvector2	door_hit_hor_ne(t_dvector2 hit, float step, float door_angle, float player_angle)
{
	float	a;
	float	r;

	if (hit.x + step / 2 >= (int)hit.x + 1)
		return ((t_dvector2){-1, -1});
	if (door_angle == 0)
		return ((t_dvector2){hit.x + step / 2, hit.y - 0.5});
	player_angle = 90 + player_angle;
	if (door_angle < 180 - player_angle)
	{
		r = (hit.x + step / 2) - (int)hit.x;
		a = (r * sinf(player_angle * TO_RADIAN))
			/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
		if (a <= 0.5)
			return ((t_dvector2){(int)hit.x + (cosf(door_angle * TO_RADIAN) * a),
				hit.y - 0.5 - sinf(door_angle * TO_RADIAN) * a});
		if (hit.x + step < (int)hit.x + 0.5)
			return ((t_dvector2){-1, -1});
	}
	r = (int)(hit.x + 1) - (hit.x + step / 2);
	player_angle = 180 - player_angle;
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_dvector2){-1, -1});
	return ((t_dvector2){(int)hit.x + 1 - (cosf(door_angle * TO_RADIAN) * a),
		hit.y - 0.5 - sinf(door_angle * TO_RADIAN) * a});
}


// xy (-1, 1)
t_dvector2	door_hit_ver_sw(t_dvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	a;
	float	r;

	if (hit.y + step / 2 > (int)hit.y + 1)
		return ((t_dvector2){-1, -1});
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
		return ((t_dvector2){-1, -1});
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
		return ((t_dvector2){-1, -1});
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
	float	a;
	float	r;

	if (hit.x + step / 2 <= (int)hit.x)
		return ((t_dvector2){-1, -1});
	if (door_angle == 0)
		return ((t_dvector2){hit.x + step / 2, hit.y - 0.5});
	player_angle = 360 - player_angle + 90;
	if (door_angle < 180 - player_angle)
	{
		r = (int)(hit.x + 1) - (hit.x + step / 2);
		a = (r * sinf(player_angle * TO_RADIAN))
			/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
		if (a <= 0.5)
			return ((t_dvector2){(int)hit.x + 1 - (cosf(door_angle * TO_RADIAN) * a),
				hit.y - 0.5 - sinf(door_angle * TO_RADIAN) * a});
		if (hit.x + step > (int)hit.x + 0.5)
			return ((t_dvector2){-1, -1});
	}
	player_angle = (-player_angle + 360 + 90) - 270;
	r = (hit.x + step / 2) - (int)hit.x;
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_dvector2){-1, -1});
	return ((t_dvector2){(int)hit.x + (cosf(door_angle * TO_RADIAN) * a),
		hit.y - 0.5 - sinf(door_angle * TO_RADIAN) * a});
}

float	get_texture_door(t_ray ray)
{
	t_dvector2	delta;
	double		dist;
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
	dist = sqrt((delta.x * delta.x + delta.y * delta.y));
	if (left_door)
		dist = 1 - dist;
	return (dist);
}

void	open_door(t_game *game)
{
	t_ray			hit;
	struct timespec	time;

	hit = get_object_hit((t_launch_ray){'\0', DOOR_CLOSE, 1}, game, game->player->f_real_pos, game->player->angle);
	if (hit.hit.x != -1)
	{
		clock_gettime(CLOCK_REALTIME, &time);
		if (((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->is_opening_door == 1)
		{
			((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->is_opening_door = -1;
			((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->door_percent -= game->delta_time * SPEEP_DOOR_OPENING;
			((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->time = time_to_long(&time);
		}
		else
		{
			((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->is_opening_door = 1;
			((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->door_percent += game->delta_time * SPEEP_DOOR_OPENING;
			((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->time = time_to_long(&time);
		}
	}
	else
	{
		hit = get_object_hit((t_launch_ray){'\0', DOOR_OPEN, 1}, game, game->player->f_real_pos, game->player->angle);
		if (hit.hit.x != -1)
		{
			clock_gettime(CLOCK_REALTIME, &time);
			game->map[(int)hit.hit.y][(int)hit.hit.x].type |= DOOR_CLOSE;
			game->map[(int)hit.hit.y][(int)hit.hit.x].type ^= DOOR_OPEN;
			game->map[(int)hit.hit.y][(int)hit.hit.x].type |= WALL;
			((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->is_opening_door = -1;
			((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->door_percent -= game->delta_time * SPEEP_DOOR_OPENING;
			((t_door *)game->map[(int)hit.hit.y][(int)hit.hit.x].arg)->time = time_to_long(&time);
		}
	}
}

static void	_step_door_open(t_door *door, long time, t_map *map_cell)
{
	long	tmp;

	tmp = time - door->time;
	if (door->is_opening_door == 1)
	{
		door->door_percent += tmp / 1000.0 * SPEEP_DOOR_OPENING;
		door->time = time;
		if (door->door_percent > 90)
		{
			map_cell->type ^= DOOR_CLOSE;
			map_cell->type |= DOOR_OPEN;
			map_cell->type ^= WALL;
			door->door_percent = 90;
			door->is_opening_door = 0;
		}
	}
	else
	{
		door->door_percent -= tmp / 1000.0 * SPEEP_DOOR_OPENING;
		door->time = time;
		if (door->door_percent < 0)
		{
			door->door_percent = 0;
			door->is_opening_door = 0;
		}
	}
}

void	update_doors(t_map **doors, int	nb_doors, long time)
{
	int	i;

	i = 0;
	while (i < nb_doors)
	{
		if (((t_door *)doors[i]->arg)->is_opening_door != 0)
			_step_door_open(doors[i]->arg, time, doors[i]);
		i++;
	}
}

