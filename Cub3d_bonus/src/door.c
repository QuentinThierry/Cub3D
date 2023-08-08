/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:20:37 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/08 17:21:33 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"


t_fvector2	door_hit_ver(t_fvector2 hit, float step, float door_angle, float player_angle)
{
	float r;

	if (hit.y + step / 2 < (int)hit.y + 1)
	{
		if (hit.y + step / 2 < (int)hit.y + 0.5)
		{
			r = (hit.y + step / 2) - (int)hit.y;
			if (door_angle >= 180 - player_angle)
				return ((t_fvector2){-1, -1});
		}
		else
		{
			r = (int)(hit.y + 1) - (hit.y + step / 2);
			player_angle = 180 - player_angle;
		}
		float a = (r * sin(player_angle * TO_RADIAN))
			/ sin((180 - door_angle - player_angle) * TO_RADIAN);
		float h = sin(door_angle * TO_RADIAN) * a;
		float b = cos(door_angle * TO_RADIAN) * a;
		if (a < 0.5)
		{
			if (hit.y + step / 2 < (int)hit.y + 0.5)
				return ((t_fvector2){hit.x + 0.5 + h, (int)hit.y + b});
			else
				return ((t_fvector2){hit.x + 0.5 + h, (int)hit.y + 1 - b});
		}
	}
	return ((t_fvector2){-1, -1});
}

t_fvector2	door_hit_hor(t_fvector2 hit, float step, float door_angle, float player_angle)
{
	float r;

	if (hit.x + step / 2 < (int)hit.x + 1)
	{
		if (hit.x + step / 2 < (int)hit.x + 0.5)
		{
			r = (hit.x + step / 2) - (int)hit.x;
			player_angle = 270 - player_angle;
			if (door_angle >= 180 - player_angle)
				return ((t_fvector2){-1, -1});
		}
		else
		{
			r = (int)(hit.x + 1) - (hit.x + step / 2);
			player_angle = player_angle - 90;
		}
		float a = (r * sin(player_angle * TO_RADIAN))
			/ sin((180 - door_angle - player_angle) * TO_RADIAN);
		float h = sin(door_angle * TO_RADIAN) * a;
		float b = cos(door_angle * TO_RADIAN) * a;
		if (a < 0.5)
		{
			if (hit.x + step / 2 < (int)hit.x + 0.5)
				return ((t_fvector2){(int)hit.x + b, hit.y + 0.5 + h});
			else
				return ((t_fvector2){(int)hit.x + 1 - b, hit.y + 0.5 + h});
		}
	}
	return ((t_fvector2){-1, -1});
}