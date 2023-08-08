/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:20:37 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/08 19:06:33 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

// xy (1, 1)
t_fvector2	door_hit_ver_se(t_fvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	a;
	float	r;

	if (hit.y + step / 2 >= (int)hit.y + 1)
		return ((t_fvector2){-1, -1});
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
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_fvector2){-1, -1});
	if (hit.y + step / 2 < (int)hit.y + 0.5)
		return ((t_fvector2){hit.x + 0.5 + sinf(door_angle * TO_RADIAN) * a,
			(int)hit.y + (cosf(door_angle * TO_RADIAN) * a)});
	return ((t_fvector2){hit.x + 0.5 + sinf(door_angle * TO_RADIAN) * a,
		(int)hit.y + 1 - (cosf(door_angle * TO_RADIAN) * a)});
}

t_fvector2	door_hit_hor_se(t_fvector2 hit, float step, float door_angle, float player_angle)
{
	float	a;
	float	r;

	if (hit.x + step / 2 >= (int)hit.x + 1)
		return ((t_fvector2){-1, -1});
	player_angle -= 90;
	if (hit.x + step / 2 < (int)hit.x + 0.5)
	{
		r = (hit.x + step / 2) - (int)hit.x;
		player_angle = 180 - player_angle;
		if (door_angle >= 180 - player_angle)
			return ((t_fvector2){-1, -1});
	}
	else
		r = (int)(hit.x + 1) - (hit.x + step / 2);
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_fvector2){-1, -1});
	if (hit.x + step / 2 < (int)hit.x + 0.5)
		return ((t_fvector2){(int)hit.x + (cosf(door_angle * TO_RADIAN) * a),
			hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
	return ((t_fvector2){(int)hit.x + 1 - (cosf(door_angle * TO_RADIAN) * a),
		hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
}

// xy (1, -1)
t_fvector2	door_hit_ver_ne(t_fvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	a;
	float	r;

	if (hit.y + step / 2 <= (int)hit.y)
		return ((t_fvector2){-1, -1});
	if (hit.y + step / 2 < (int)hit.y + 0.5)
	{
		r = (hit.y + step / 2) - (int)hit.y;
	}
	else
	{
		r = (int)(hit.y + 1) - (hit.y + step / 2);
		player_angle = 180 - player_angle;
		if (door_angle >= 180 - player_angle)
			return ((t_fvector2){-1, -1});
	}
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_fvector2){-1, -1});
	if (hit.y + step / 2 < (int)hit.y + 0.5)
		return ((t_fvector2){hit.x + 0.5 + sinf(door_angle * TO_RADIAN) * a,
			(int)hit.y + (cosf(door_angle * TO_RADIAN) * a)});
	return ((t_fvector2){hit.x + 0.5 + sinf(door_angle * TO_RADIAN) * a,
		(int)hit.y + 1 - (cosf(door_angle * TO_RADIAN) * a)});
}

t_fvector2	door_hit_hor_ne(t_fvector2 hit, float step, float door_angle, float player_angle)
{
	float	a;
	float	r;

	if (hit.x + step / 2 >= (int)hit.x + 1)
		return ((t_fvector2){-1, -1});
	if (hit.x + step / 2 < (int)hit.x + 0.5)
	{
		player_angle = 90 + player_angle;
		if (door_angle >= 180 - player_angle)
			return ((t_fvector2){-1, -1});
		r = (hit.x + step / 2) - (int)hit.x;
	}
	else
	{
		r = (int)(hit.x + 1) - (hit.x + step / 2);
		player_angle = 90 - player_angle;
	}
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_fvector2){-1, -1});
	if (hit.x + step / 2 < (int)hit.x + 0.5)
		return ((t_fvector2){(int)hit.x + (cosf(door_angle * TO_RADIAN) * a),
			hit.y - 0.5 - sinf(door_angle * TO_RADIAN) * a});
	return ((t_fvector2){(int)hit.x + 1 - (cosf(door_angle * TO_RADIAN) * a),
		hit.y - 0.5 - sinf(door_angle * TO_RADIAN) * a});
}


// xy (-1, 1)
t_fvector2	door_hit_ver_sw(t_fvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	a;
	float	r;

	if (hit.y + step / 2 > (int)hit.y + 1)
		return ((t_fvector2){-1, -1});
	if (hit.y + step / 2 < (int)hit.y + 0.5)
	{
		r = (hit.y + step / 2) - (int)hit.y;
		player_angle = 360 - player_angle;
		if (door_angle >= 180 - player_angle)
			return ((t_fvector2){-1, -1});
	}
	else
	{
		r = (int)(hit.y + 1) - (hit.y + step / 2);
		player_angle = player_angle - 180;
	}
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_fvector2){-1, -1});
	if (hit.y + step / 2 < (int)hit.y + 0.5)
		return ((t_fvector2){hit.x - 0.5 - sinf(door_angle * TO_RADIAN) * a,
			(int)hit.y + (cosf(door_angle * TO_RADIAN) * a)});
	return ((t_fvector2){hit.x - 0.5 - sinf(door_angle * TO_RADIAN) * a,
		(int)hit.y + 1 - (cosf(door_angle * TO_RADIAN) * a)});
}

t_fvector2	door_hit_hor_sw(t_fvector2 hit, float step, float door_angle, float player_angle)
{
	float	a;
	float	r;

	if (hit.x + step / 2 <= (int)hit.x)
		return ((t_fvector2){-1, -1});
	if (hit.x + step / 2 < (int)hit.x + 0.5)
	{
		player_angle = 270 - player_angle;
		r = (hit.x + step / 2) - (int)hit.x;
	}
	else
	{
		r = (int)(hit.x + 1) - (hit.x + step / 2);
		player_angle = player_angle - 90;
		if (door_angle >= 180 - player_angle)
			return ((t_fvector2){-1, -1});
	}
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_fvector2){-1, -1});
	if (hit.x + step / 2 < (int)hit.x + 0.5)
		return ((t_fvector2){(int)hit.x + (cosf(door_angle * TO_RADIAN) * a),
			hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
	return ((t_fvector2){(int)hit.x + 1 - (cosf(door_angle * TO_RADIAN) * a),
		hit.y + 0.5 + sinf(door_angle * TO_RADIAN) * a});
}

// xy (-1, -1)
t_fvector2	door_hit_ver_nw(t_fvector2 hit, float step, float door_angle,
						float player_angle)
{
	float	a;
	float	r;

	if (hit.y + step / 2 <= (int)hit.y)
		return ((t_fvector2){-1, -1});
	if (hit.y + step / 2 < (int)hit.y + 0.5)
	{
		r = (hit.y + step / 2) - (int)hit.y;
		player_angle = 360 - player_angle;
	}
	else
	{
		r = (int)(hit.y + 1) - (hit.y + step / 2);
		player_angle = player_angle - 180;
		if (door_angle >= 180 - player_angle)
			return ((t_fvector2){-1, -1});
	}
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_fvector2){-1, -1});
	if (hit.y + step / 2 < (int)hit.y + 0.5)
		return ((t_fvector2){hit.x - 0.5 - sinf(door_angle * TO_RADIAN) * a,
			(int)hit.y + (cosf(door_angle * TO_RADIAN) * a)});
	return ((t_fvector2){hit.x - 0.5 - sinf(door_angle * TO_RADIAN) * a,
		(int)hit.y + 1 - (cosf(door_angle * TO_RADIAN) * a)});
}

t_fvector2	door_hit_hor_nw(t_fvector2 hit, float step, float door_angle, float player_angle)
{
	float	a;
	float	r;

	if (hit.x + step / 2 <= (int)hit.x)
		return ((t_fvector2){-1, -1});
	if (hit.x + step / 2 < (int)hit.x + 0.5)
	{
		player_angle = player_angle - 270;
		r = (hit.x + step / 2) - (int)hit.x;
	}
	else
	{
		r = (int)(hit.x + 1) - (hit.x + step / 2);
		player_angle = 360 - player_angle + 90;
		if (door_angle >= 180 - player_angle)
			return ((t_fvector2){-1, -1});
	}
	a = (r * sinf(player_angle * TO_RADIAN))
		/ sinf((180 - door_angle - player_angle) * TO_RADIAN);
	if (a > 0.5)
		return ((t_fvector2){-1, -1});
	if (hit.x + step / 2 < (int)hit.x + 0.5)
		return ((t_fvector2){(int)hit.x + (cosf(door_angle * TO_RADIAN) * a),
			hit.y - 0.5 - sinf(door_angle * TO_RADIAN) * a});
	return ((t_fvector2){(int)hit.x + 1 - (cosf(door_angle * TO_RADIAN) * a),
		hit.y - 0.5 - sinf(door_angle * TO_RADIAN) * a});
}
