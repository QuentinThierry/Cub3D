/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:04:05 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/06 23:16:58 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// xy (1, 1)
static inline t_fvector2	_get_wall_hit_se(t_fvector2 fpos,
								char **map, float angle)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y + 1};
	comp.x = fpos.x + fabsf(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabsf(fpos.x - map_pos.x) / angle;
	step = (t_fvector2){angle, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map[(int)comp.y][map_pos.x] == '1')
				return ((t_fvector2){map_pos.x, comp.y});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else // (map_pos.x >= comp.x - 0.0001)
		{
			if (map[map_pos.y][((int)comp.x)] == '1')
				return ((t_fvector2){comp.x, map_pos.y});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static inline t_fvector2	_get_wall_hit_ne(t_fvector2 fpos,
								char **map, float angle)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y};
	comp.x = fpos.x + fabsf(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabsf(fpos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map[(int)comp.y][map_pos.x] == '1')
				return ((t_fvector2){map_pos.x, comp.y});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else// (map_pos.x >= comp.x - 0.0001)
		{
			if (map[map_pos.y - 1][((int)comp.x)] == '1')
				return ((t_fvector2){comp.x, map_pos.y});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static inline t_fvector2	_get_wall_hit_sw(t_fvector2 fpos,
								char **map, float angle)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y + 1};
	comp.x = fpos.x + fabsf(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabsf(fpos.x - map_pos.x) / angle;
	step = (t_fvector2){angle * -1, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map[(int)comp.y][map_pos.x - 1] == '1')
				return ((t_fvector2){map_pos.x, comp.y});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else // (map_pos.x <= comp.x + 0.0001)
		{
			if (map[map_pos.y][((int)comp.x)] == '1')
				return ((t_fvector2){comp.x, map_pos.y});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static inline t_fvector2	_get_wall_hit_nw(t_fvector2 fpos,
								char **map, float angle)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y};
	comp.x = fpos.x + fabsf(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabsf(fpos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle * -1, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map[(int)comp.y][map_pos.x - 1] == '1')
				return ((t_fvector2){map_pos.x, comp.y});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else // (map_pos.x <= comp.x + 0.0001)
		{
			if (map[map_pos.y - 1][((int)comp.x)] == '1')
				return ((t_fvector2){comp.x, map_pos.y});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

inline t_fvector2	get_wall_hit(t_fvector2 fpos, char **map, float angle)
{
	t_fvector2	delta;
	t_vector2	sign;

	sign = get_sign(angle);
	// angle *= 100000;
	// angle = (int)angle;
	// angle /= 100000;
	angle = fabsf(tanf(angle * TO_RADIAN));
	if (sign.x == 1 && sign.y == 1)
		return (_get_wall_hit_se(fpos, map, angle));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_wall_hit_ne(fpos, map, angle));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_wall_hit_sw(fpos, map, angle));
	else
		return (_get_wall_hit_nw(fpos, map, angle));
	return ((t_fvector2){0});
}
