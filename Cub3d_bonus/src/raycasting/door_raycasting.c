/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_raycasting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 19:20:13 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 19:25:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline t_dvector2	_get_second_door_ver_se(t_dvector2 hit, float step,
				float door_angle, float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

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
	return (_get_second_door_ver_se(hit, step, door_angle, player_angle));
}

__attribute__((always_inline))
static inline t_dvector2	_get_second_door_hor_se(t_dvector2 hit, float step,
				float door_angle, float player_angle)
{
	float	a;
	float	r;

	r = (int)(hit.x + 1) - (hit.x + step / 2);
	player_angle = -player_angle + 180;
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_dvector2){-1, -1});
	return ((t_dvector2){(int)hit.x + 1 - (cosf(door_angle * TO_RADIAN) * a),
		hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
}

t_dvector2	door_hit_hor_se(t_dvector2 hit, float step, float door_angle,
			float player_angle)
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
			return ((t_dvector2){(int)hit.x
				+ (cosf(door_angle * TO_RADIAN) * a),
				hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
		if (hit.x + step < (int)hit.x + 0.5)
			return ((t_dvector2){-1, -1});
	}
	return (_get_second_door_hor_se(hit, step, door_angle, player_angle));
}
