/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 18:21:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/27 19:25:56 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/cub3d_bonus.h"


// xy (1, 1)
static t_ray	_get_object_se(t_fvector2 begin,
								t_map **map, t_objet object, float angle)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	t_fvector2	max_pos;
	float		t_angle;

	max_pos = (t_fvector2){begin.x + cos((angle - 90) * TO_RADIAN) * object.dist,
			begin.y + sin((angle - 90) * TO_RADIAN) * object.dist};
	t_angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)begin.x + 1, (int)begin.y + 1};
	comp.x = begin.x + fabs(begin.y - map_pos.y) * t_angle;
	comp.y = begin.y + fabs(begin.x - map_pos.x) / t_angle;
	step = (t_fvector2){t_angle, 1 / t_angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x > max_pos.x)
				return ((t_ray){{-1, -1}, e_west, 0});
			if ((map[(int)(comp.y)][map_pos.x].type & object.type) == object.type &&
				(map[(int)(comp.y)][map_pos.x].symbol == object.symbol || object.symbol == '\0'))
				return ((t_ray){{map_pos.x, (int)(comp.y)}, e_west, 0});
			if ((map[(int)(comp.y)][map_pos.x].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_west, 0});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else 
		{
			if (map_pos.y > max_pos.y)
				return ((t_ray){{-1, -1}, e_north, 0});
			if ((map[map_pos.y][(int)(comp.x)].type & object.type) == object.type &&
				(map[map_pos.y][(int)(comp.x)].symbol == object.symbol || object.symbol == '\0'))
				return ((t_ray){{(int)(comp.x), map_pos.y}, e_north, 0});
			if ((map[map_pos.y][(int)(comp.x)].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_north, 0});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static t_ray	_get_object_ne(t_fvector2 begin,
								t_map **map, t_objet object, float angle)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	t_fvector2	max_pos;
	float		t_angle;

	max_pos = (t_fvector2){begin.x + cos((90 - angle) * TO_RADIAN) * object.dist,
			begin.y - sin((90 - angle) * TO_RADIAN) * object.dist};
	t_angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)begin.x + 1, (int)begin.y};
	comp.x = begin.x + fabs(begin.y - map_pos.y) * t_angle;
	comp.y = begin.y + fabs(begin.x - map_pos.x) / t_angle * -1;
	step = (t_fvector2){t_angle, 1 / t_angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x > max_pos.x)
				return ((t_ray){{-1, -1}, e_west, 0});
			if ((map[(int)(comp.y)][map_pos.x].type & object.type) == object.type &&
				(map[(int)(comp.y)][map_pos.x].symbol == object.symbol || object.symbol == '\0'))
				return ((t_ray){{map_pos.x, (int)(comp.y)}, e_west, 0});
			if ((map[(int)(comp.y)][map_pos.x].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_west, 0});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if (map_pos.y < max_pos.y)
				return ((t_ray){{-1, -1}, e_south, 0});
			if ((map[map_pos.y - 1][(int)(comp.x)].type & object.type) == object.type &&
				(map[map_pos.y - 1][((int)(comp.x))].symbol == object.symbol || object.symbol == '\0'))
				return ((t_ray){{(int)(comp.x), map_pos.y - 1}, e_south, 0});
			if ((map[map_pos.y - 1][(int)(comp.x)].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_south, 0});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static t_ray	_get_object_sw(t_fvector2 begin,
								t_map **map, t_objet object, float angle)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	t_fvector2	max_pos;
	float		t_angle;

	max_pos = (t_fvector2){begin.x - sin((angle - 180) * TO_RADIAN) * object.dist,
			begin.y + cos((angle - 180) * TO_RADIAN) * object.dist};
	t_angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)begin.x, (int)begin.y + 1};
	comp.x = begin.x + fabs(begin.y - map_pos.y) * t_angle * -1;
	comp.y = begin.y + fabs(begin.x - map_pos.x) / t_angle;
	step = (t_fvector2){t_angle * -1, 1 / t_angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x < max_pos.x)
				return ((t_ray){{-1, -1}, e_east, 0});
			if ((map[(int)(comp.y)][map_pos.x - 1].type & object.type) == object.type &&
				(map[(int)(comp.y)][map_pos.x - 1].symbol == object.symbol || object.symbol == '\0'))
				return ((t_ray){{map_pos.x - 1, (int)(comp.y)}, e_east, 0});
			if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_east, 0});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y > max_pos.y)
				return ((t_ray){{-1, -1}, e_north, 0});
			if ((map[map_pos.y][(int)(comp.x)].type & object.type) == object.type &&
				(map[map_pos.y][(int)(comp.x)].symbol == object.symbol || object.symbol == '\0'))
				return ((t_ray){{(int)(comp.x), map_pos.y}, e_north, 0});
			if ((map[map_pos.y][(int)(comp.x)].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_north, 0});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static t_ray	_get_object_nw(t_fvector2 begin,
								t_map **map, t_objet object, float angle)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	t_fvector2	max_pos;
	float		t_angle;

	max_pos = (t_fvector2){begin.x - sin((360 - angle) * TO_RADIAN) * object.dist,
			begin.y - cos((360 - angle) * TO_RADIAN) * object.dist};
	t_angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)begin.x, (int)begin.y};
	comp.x = begin.x + fabs(begin.y - map_pos.y) * t_angle * -1;
	comp.y = begin.y + fabs(begin.x - map_pos.x) / t_angle * -1;
	step = (t_fvector2){t_angle * -1, 1 / t_angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x < max_pos.x)
				return ((t_ray){{-1, -1}, e_east, 0});
			if ((map[(int)(comp.y)][map_pos.x - 1].type & object.type) == object.type
				&& (map[(int)(comp.y)][map_pos.x - 1].symbol == object.symbol || object.symbol == '\0'))
				return ((t_ray){{map_pos.x - 1, (int)(comp.y)}, e_east, 0});
			if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_east, 0});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y < max_pos.y)
				return ((t_ray){{-1, -1}, e_south, 0});
			if ((map[map_pos.y - 1][(int)(comp.x)].type & object.type) == object.type
				&& (map[map_pos.y - 1][(int)(comp.x)].symbol == object.symbol || object.symbol == '\0'))
				return ((t_ray){{(int)(comp.x), map_pos.y - 1}, e_south, 0});
			if ((map[map_pos.y - 1][(int)(comp.x)].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_south, 0});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

t_ray	get_object_hit(t_objet object, t_game *game, t_fvector2 begin, float angle)
{
	t_vector2	sign;

	sign = get_sign(angle);
	if (sign.x == 1 && sign.y == 1)
		return (_get_object_se(begin, game->map, object, angle));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_object_ne(begin, game->map, object, angle));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_object_sw(begin, game->map, object, angle));
	else
		return (_get_object_nw(begin, game->map, object, angle));
}
