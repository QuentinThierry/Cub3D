/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_raycasting1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 19:21:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 19:24:07 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline t_dvector2	_get_second_door_ver_ne(t_dvector2 hit, float step,
				float door_angle, float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

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
	return (_get_second_door_ver_ne(hit, step, door_angle, player_angle));
}

__attribute__((always_inline))
static inline t_dvector2	_get_second_door_hor_ne(t_dvector2 hit, float step,
				float door_angle, float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

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

t_dvector2	door_hit_hor_ne(t_dvector2 hit, float step, float door_angle,
				float player_angle)
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
	return (_get_second_door_hor_ne(hit, step, door_angle, player_angle));
}
