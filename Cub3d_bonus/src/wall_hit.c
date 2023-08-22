/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:04:05 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/21 20:03:05 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

// xy (1, 1)
static t_ray_2	_get_wall_hit_se(t_fvector2 fpos,
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
				return ((t_ray_2){{-1, -1}, {-1, -1}, -1});
			if (map[(int)(comp.y)][map_pos.x].is_wall == true)
			{
				return ((t_ray_2){{map_pos.x, comp.y},
				{fabs(fpos.x - map_pos.x), fabs(fpos.y - comp.y)}, e_west});
			}
			if (map[(int)(comp.y)][map_pos.x].symbol == 'a' || map[(int)(comp.y)][map_pos.x].symbol == 'c' || map[(int)(comp.y)][map_pos.x].symbol == 'e')
			{
				t_fvector2 b_pos = get_matching_letter(map, map_size, map[(int)(comp.y)][map_pos.x].symbol);
				t_ray_2	ray;

				ray = _get_wall_hit_se((t_fvector2){b_pos.x, b_pos.y + comp.y - (int)comp.y}
					, map, angle, map_size);
				ray.delta_point.x += fabs(fpos.x - map_pos.x);
				ray.delta_point.y += fabs(fpos.y - comp.y);
				return (ray);
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else 
		{
			if ((int)(comp.x) >= map_size.x || map_pos.y >= map_size.y)
				return ((t_ray_2){{-1, -1}, {-1, -1}, -1});
			if (map[map_pos.y][(int)(comp.x)].is_wall == true)
			{
				return ((t_ray_2){{comp.x, map_pos.y},
				{fabs(fpos.x - comp.x), fabs(fpos.y - map_pos.y)}, e_north});
			}
			if (map[map_pos.y][(int)(comp.x)].symbol == 'a' || map[map_pos.y][(int)(comp.x)].symbol == 'c' || map[map_pos.y][(int)(comp.x)].symbol == 'e')
			{
				t_fvector2 b_pos = get_matching_letter(map, map_size, map[map_pos.y][(int)(comp.x)].symbol);
				t_ray_2	ray;

				ray = _get_wall_hit_se((t_fvector2){b_pos.x + comp.x - (int)comp.x, b_pos.y}
					, map, angle, map_size);
				ray.delta_point.x += fabs(fpos.x - comp.x);
				ray.delta_point.y += fabs(fpos.y - map_pos.y);
				return (ray);
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static t_ray_2	_get_wall_hit_ne(t_fvector2 fpos,
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
			if (map_pos.x >= map_size.x || (int)(comp.y) >= map_size.y)
				return ((t_ray_2){{-1, -1}, {-1, -1}, -1});
			if (map[(int)(comp.y)][map_pos.x].is_wall == true)
			{
				return ((t_ray_2){{map_pos.x, comp.y},
				{fabs(fpos.x - map_pos.x), fabs(fpos.y - comp.y)}, e_west});
			}
			if (map[(int)(comp.y)][map_pos.x].symbol == 'a' || map[(int)(comp.y)][map_pos.x].symbol == 'c' || map[(int)(comp.y)][map_pos.x].symbol == 'e')
			{
				t_fvector2 b_pos = get_matching_letter(map, map_size, map[(int)(comp.y)][map_pos.x].symbol);
				t_ray_2	ray;

				ray = _get_wall_hit_ne((t_fvector2){b_pos.x, b_pos.y + comp.y - (int)comp.y}
					, map, angle, map_size);
				ray.delta_point.x += fabs(fpos.x - map_pos.x);
				ray.delta_point.y += fabs(fpos.y - comp.y);
				return (ray);
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if ((int)(comp.x) >= map_size.x || map_pos.y - 1 < 0)
				return ((t_ray_2){{-1, -1}, {-1, -1}, -1});
			if (map[map_pos.y - 1][((int)(comp.x))].is_wall == true)
			{
				return ((t_ray_2){{comp.x, map_pos.y},
				{fabs(fpos.x - comp.x), fabs(fpos.y - map_pos.y)}, e_south});
			}
			if (map[map_pos.y - 1][(int)(comp.x)].symbol == 'a' || map[map_pos.y - 1][(int)(comp.x)].symbol == 'c' || map[map_pos.y - 1][(int)(comp.x)].symbol == 'e')
			{
				t_fvector2 b_pos = get_matching_letter(map, map_size, map[map_pos.y - 1][(int)(comp.x)].symbol);
				t_ray_2	ray;

				ray = _get_wall_hit_ne((t_fvector2){b_pos.x + comp.x - (int)comp.x, b_pos.y + 1}
					, map, angle, map_size);
				ray.delta_point.x += fabs(fpos.x - comp.x);
				ray.delta_point.y += fabs(fpos.y - map_pos.y);
				return (ray);
			}
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static t_ray_2	_get_wall_hit_sw(t_fvector2 fpos,
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
				return ((t_ray_2){{-1, -1}, {-1, -1}, -1});
			if (map[(int)(comp.y)][map_pos.x - 1].is_wall == true)
			{
				return ((t_ray_2){{map_pos.x, comp.y},
				{fabs(fpos.x - map_pos.x), fabs(fpos.y - comp.y)}, e_east});
			}
			if (map[(int)(comp.y)][map_pos.x - 1].symbol == 'a' || map[(int)(comp.y)][map_pos.x - 1].symbol == 'c' || map[(int)(comp.y)][map_pos.x - 1].symbol == 'e')
			{
				t_fvector2 b_pos = get_matching_letter(map, map_size, map[(int)(comp.y)][map_pos.x - 1].symbol);
				t_ray_2	ray;

				ray = _get_wall_hit_sw((t_fvector2){b_pos.x + 1, b_pos.y + comp.y - (int)comp.y}
					, map, angle, map_size);
				ray.delta_point.x += fabs(fpos.x - map_pos.x);
				ray.delta_point.y += fabs(fpos.y - comp.y);
				return (ray);
			}
			comp.y += step.y;
			map_pos.x -= 1;
		}
		else
		{
			if ((int)(comp.x) >= map_size.x || map_pos.y >= map_size.y)
				return ((t_ray_2){{-1, -1}, {-1, -1}, -1});
			if (map[map_pos.y][(int)(comp.x)].is_wall == true)
			{
				return ((t_ray_2){{comp.x, map_pos.y},
				{fabs(fpos.x - comp.x), fabs(fpos.y - map_pos.y)}, e_north});
			}
			if (map[map_pos.y][(int)(comp.x)].symbol == 'a' || map[map_pos.y][(int)(comp.x)].symbol == 'c' || map[map_pos.y][(int)(comp.x)].symbol == 'e')
			{
				t_fvector2 b_pos = get_matching_letter(map, map_size, map[map_pos.y][(int)(comp.x)].symbol);
				t_ray_2	ray;

				ray = _get_wall_hit_sw((t_fvector2){b_pos.x + comp.x - (int)comp.x, b_pos.y}
					, map, angle, map_size);
				ray.delta_point.x += fabs(fpos.x - comp.x);
				ray.delta_point.y += fabs(fpos.y - map_pos.y);
				return (ray);
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static t_ray_2	_get_wall_hit_nw(t_fvector2 fpos,
								t_map **map, double angle, t_vector2 map_size)
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
				return ((t_ray_2){{-1, -1}, {-1, -1}, -1});
			if (map[(int)(comp.y)][map_pos.x - 1].is_wall == true)
			{
				return ((t_ray_2){{map_pos.x, comp.y},
				{fabs(fpos.x - map_pos.x), fabs(fpos.y - comp.y)}, e_east});
			}
			if (map[(int)(comp.y)][map_pos.x - 1].symbol == 'a' || map[(int)(comp.y)][map_pos.x - 1].symbol == 'c' || map[(int)(comp.y)][map_pos.x - 1].symbol == 'e')
			{
				t_fvector2 b_pos = get_matching_letter(map, map_size, map[(int)(comp.y)][map_pos.x - 1].symbol);
				t_ray_2	ray;

				ray = _get_wall_hit_nw((t_fvector2){b_pos.x + 1, b_pos.y + comp.y - (int)comp.y}
					, map, angle, map_size);
				ray.delta_point.x += fabs(fpos.x - map_pos.x);
				ray.delta_point.y += fabs(fpos.y - comp.y);
				return (ray);
			}
			comp.y += step.y;
			map_pos.x -= 1;
		}
		else
		{
			if ((int)(comp.x) < 0 || map_pos.y - 1 < 0)
				return ((t_ray_2){{-1, -1}, {-1, -1}, -1});
			if (map[map_pos.y - 1][((int)(comp.x))].is_wall == true)
			{
				return ((t_ray_2){{comp.x, map_pos.y},
				{fabs(fpos.x - comp.x), fabs(fpos.y - map_pos.y)}, e_south});
			}
			if (map[map_pos.y - 1][(int)(comp.x)].symbol == 'a' || map[map_pos.y - 1][(int)(comp.x)].symbol == 'c' || map[map_pos.y - 1][(int)(comp.x)].symbol == 'e')
			{
				t_fvector2 b_pos = get_matching_letter(map, map_size, map[map_pos.y - 1][(int)(comp.x)].symbol);
				t_ray_2	ray;

				ray = _get_wall_hit_nw((t_fvector2){b_pos.x + comp.x - (int)comp.x, b_pos.y + 1}
					, map, angle, map_size);
				ray.delta_point.x += fabs(fpos.x - comp.x);
				ray.delta_point.y += fabs(fpos.y - map_pos.y);
				return (ray);
			}
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

t_ray_2	get_wall_hit(t_fvector2 fpos, t_map **map, float angle, t_vector2 map_size)
{
	t_vector2	sign;

	if (fpos.x < 0 || fpos.y < 0 || fpos.x > map_size.x || fpos.y > map_size.y)
		return((t_ray_2){{-1, -1}, {-1, -1}, -1});
	sign = get_sign(angle);
	angle = fabs(tan(angle * TO_RADIAN));
	if (sign.x == 1 && sign.y == 1)
		return (_get_wall_hit_se(fpos, map, angle, map_size));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_wall_hit_ne(fpos, map, angle, map_size));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_wall_hit_sw(fpos, map, angle, map_size));
	else
		return (_get_wall_hit_nw(fpos, map, angle, map_size));
}

