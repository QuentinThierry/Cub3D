/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_open_door1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:02:55 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/11 19:03:51 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static t_dvector2	get_second_door_hor_sw(t_dvector2 hit, float step,
		float door_angle, float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

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

t_dvector2	end_door_hit_hor_sw(t_dvector2 hit, float step, float door_angle,
		float player_angle)
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
	return (get_second_door_hor_sw(hit, step, door_angle, player_angle));
}

static t_dvector2	get_second_door_ver_nw(t_dvector2 hit, float step,
		float door_angle, float player_angle)
{
	float	l;
	float	x;
	float	y;
	float	a;

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
	return (get_second_door_ver_nw(hit, step, door_angle, player_angle));
}
