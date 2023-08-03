/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:04:05 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/03 19:24:37 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

// xy (1, 1)
static t_fvector2	_get_wall_hit_se(t_fvector2 fpos,
								t_map **map, float angle, t_vector2 map_size)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_fvector2){angle, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x >= map_size.x || (int)(comp.y) >= map_size.y)
				return ((t_fvector2){-1, -1});
			if (map[(int)(comp.y)][map_pos.x].is_wall == true)
			{
				if (map[(int)(comp.y)][map_pos.x].symbol == 'c')
				{
					if (comp.y + step.y / 2 < (int)comp.y + map[(int)(comp.y)][map_pos.x].door_percent / 100.f)
						return ((t_fvector2){map_pos.x + 0.5, comp.y + step.y / 2});
				}
				else
					return ((t_fvector2){map_pos.x, comp.y});
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else 
		{
			if ((int)(comp.x) >= map_size.x || map_pos.y >= map_size.y)
				return ((t_fvector2){-1, -1});
			if (map[map_pos.y][(int)(comp.x)].is_wall == true)
			{
				if (map[map_pos.y][(int)(comp.x)].symbol == 'c')
				{
					if (comp.x + step.x / 2 < (int)comp.x + map[map_pos.y][(int)(comp.x)].door_percent / 100.f)
						return ((t_fvector2){comp.x + step.x / 2, map_pos.y + 0.5});
				}
				else
					return ((t_fvector2){comp.x, map_pos.y});
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static t_fvector2	_get_wall_hit_ne(t_fvector2 fpos,
								t_map **map, float angle, t_vector2 map_size)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x >= map_size.x || (int)(comp.y) < 0)
				return ((t_fvector2){-1, -1});
			if (map[(int)(comp.y)][map_pos.x].is_wall == true)
				return ((t_fvector2){map_pos.x, comp.y});
			// if (map[(int)(comp.y)][map_pos.x].symbol == 'c')
			// 	return ((t_fvector2){map_pos.x, comp.y});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if ((int)(comp.x) >= map_size.x || map_pos.y - 1 < 0)
				return ((t_fvector2){-1, -1});
			if (map[map_pos.y - 1][((int)(comp.x))].is_wall == true)
				return ((t_fvector2){comp.x, map_pos.y});
			// if (map[map_pos.y - 1][((int)(comp.x))].symbol == 'c')
			// 	return ((t_fvector2){comp.x, map_pos.y});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static t_fvector2	_get_wall_hit_sw(t_fvector2 fpos,
								t_map **map, float angle, t_vector2 map_size)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_fvector2){angle * -1, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x - 1 < 0 || (int)(fpos.y) >= map_size.y)
				return ((t_fvector2){-1, -1});
			if (map[(int)(comp.y)][map_pos.x - 1].is_wall == true)
				return ((t_fvector2){map_pos.x, comp.y});
			// if (map[(int)(comp.y)][map_pos.x - 1].symbol == 'c')
			// 	return ((t_fvector2){map_pos.x, comp.y});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if ((int)(comp.x) < 0 || map_pos.y >= map_size.y)
				return ((t_fvector2){-1, -1});
			if (map[map_pos.y][((int)(comp.x))].is_wall == true)
				return ((t_fvector2){comp.x, map_pos.y});
			// if (map[map_pos.y][((int)(comp.x))].symbol == 'c')
			// 	return ((t_fvector2){comp.x, map_pos.y});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static t_fvector2	_get_wall_hit_nw(t_fvector2 fpos,
								t_map **map, double angle)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle * -1, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x - 1 < 0 || (int)(comp.y) < 0)
				return ((t_fvector2){-1, -1});
			if (map[(int)(comp.y)][map_pos.x - 1].is_wall == true)
				return ((t_fvector2){map_pos.x, comp.y});
			// if (map[(int)(comp.y)][map_pos.x - 1].symbol == 'c')
			// 	return ((t_fvector2){map_pos.x, comp.y});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if ((int)(comp.x) < 0 || map_pos.y - 1 < 0)
				return ((t_fvector2){-1, -1});
			if (map[map_pos.y - 1][((int)(comp.x))].is_wall == true)
				return ((t_fvector2){comp.x, map_pos.y});
			// if (map[map_pos.y - 1][((int)(comp.x))].symbol == 'c')
			// 	return ((t_fvector2){comp.x, map_pos.y});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

inline t_fvector2	get_wall_hit(t_fvector2 fpos, t_map **map, float angle, t_vector2 map_size)
{
	t_vector2	sign;

	if (fpos.x < 0 || fpos.y < 0 || fpos.x > map_size.x || fpos.y > map_size.y)
		return((t_fvector2){-1, -1});
	sign = get_sign(angle);
	angle = fabs(tan(angle * TO_RADIAN));
	if (sign.x == 1 && sign.y == 1)
		return (_get_wall_hit_se(fpos, map, angle, map_size));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_wall_hit_ne(fpos, map, angle, map_size));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_wall_hit_sw(fpos, map, angle, map_size));
	else
		return (_get_wall_hit_nw(fpos, map, angle));
	return ((t_fvector2){0});
}
