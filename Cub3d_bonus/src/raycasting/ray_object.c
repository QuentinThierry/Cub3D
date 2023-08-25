/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 18:21:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/25 20:37:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/cub3d_bonus.h"


// xy (1, 1)
static t_ray	_get_object_se(t_player *player,
								t_map **map, float dist, char object, t_type type)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	t_fvector2	max_pos;
	float		angle;

	max_pos = (t_fvector2){player->f_real_pos.x + cos((player->angle - 90) * TO_RADIAN) * dist,
			player->f_real_pos.y + sin((player->angle - 90) * TO_RADIAN) * dist};
	angle = fabs(tan(player->angle * TO_RADIAN));
	map_pos = (t_vector2){(int)player->f_real_pos.x + 1, (int)player->f_real_pos.y + 1};
	comp.x = player->f_real_pos.x + fabs(player->f_real_pos.y - map_pos.y) * angle;
	comp.y = player->f_real_pos.y + fabs(player->f_real_pos.x - map_pos.x) / angle;
	step = (t_fvector2){angle, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x > max_pos.x)
				return ((t_ray){{-1, -1}, e_west});
			if ((map[(int)(comp.y)][map_pos.x].type & type) == type &&
				(map[(int)(comp.y)][map_pos.x].symbol == object || object == '\0'))
				return ((t_ray){{map_pos.x, (int)(comp.y)}, e_west});
			if ((map[(int)(comp.y)][map_pos.x].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_west});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else 
		{
			if (map_pos.y > max_pos.y)
				return ((t_ray){{-1, -1}, e_north});
			if ((map[map_pos.y][(int)(comp.x)].type & type) == type &&
				(map[map_pos.y][(int)(comp.x)].symbol == object || object == '\0'))
				return ((t_ray){{(int)(comp.x), map_pos.y}, e_north});
			if ((map[map_pos.y][(int)(comp.x)].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_north});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static t_ray	_get_object_ne(t_player *player,
								t_map **map, float dist, char object, t_type type)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	t_fvector2	max_pos;
	float		angle;

	max_pos = (t_fvector2){player->f_real_pos.x + cos((90 - player->angle) * TO_RADIAN) * dist,
			player->f_real_pos.y - sin((90 - player->angle) * TO_RADIAN) * dist};
	angle = fabs(tan(player->angle * TO_RADIAN));
	map_pos = (t_vector2){(int)player->f_real_pos.x + 1, (int)player->f_real_pos.y};
	comp.x = player->f_real_pos.x + fabs(player->f_real_pos.y - map_pos.y) * angle;
	comp.y = player->f_real_pos.y + fabs(player->f_real_pos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x > max_pos.x)
				return ((t_ray){{-1, -1}, e_west});
			if ((map[(int)(comp.y)][map_pos.x].type & type) == type &&
				(map[(int)(comp.y)][map_pos.x].symbol == object || object == '\0'))
				return ((t_ray){{map_pos.x, (int)(comp.y)}, e_west});
			if ((map[(int)(comp.y)][map_pos.x].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_west});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if (map_pos.y < max_pos.y)
				return ((t_ray){{-1, -1}, e_south});
			if ((map[map_pos.y - 1][(int)(comp.x)].type & type) == type &&
				(map[map_pos.y - 1][((int)(comp.x))].symbol == object || object == '\0'))
				return ((t_ray){{(int)(comp.x), map_pos.y - 1}, e_south});
			if ((map[map_pos.y - 1][(int)(comp.x)].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_south});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static t_ray	_get_object_sw(t_player *player,
								t_map **map, float dist, char object, t_type type)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	t_fvector2		max_pos;
	float		angle;

	max_pos = (t_fvector2){player->f_real_pos.x - sin((player->angle - 180) * TO_RADIAN) * dist,
			player->f_real_pos.y + cos((player->angle - 180) * TO_RADIAN) * dist};
	angle = fabs(tan(player->angle * TO_RADIAN));
	map_pos = (t_vector2){(int)player->f_real_pos.x, (int)player->f_real_pos.y + 1};
	comp.x = player->f_real_pos.x + fabs(player->f_real_pos.y - map_pos.y) * angle * -1;
	comp.y = player->f_real_pos.y + fabs(player->f_real_pos.x - map_pos.x) / angle;
	step = (t_fvector2){angle * -1, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x < max_pos.x)
				return ((t_ray){{-1, -1}, e_east});
			if ((map[(int)(comp.y)][map_pos.x - 1].type & type) == type &&
				(map[(int)(comp.y)][map_pos.x - 1].symbol == object || object == '\0'))
				return ((t_ray){{map_pos.x - 1, (int)(comp.y)}, e_east});
			if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_east});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y > max_pos.y)
				return ((t_ray){{-1, -1}, e_north});
			if ((map[map_pos.y][(int)(comp.x)].type & type) == type &&
				(map[map_pos.y][(int)(comp.x)].symbol == object || object == '\0'))
				return ((t_ray){{(int)(comp.x), map_pos.y}, e_north});
			if ((map[map_pos.y][(int)(comp.x)].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_north});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static t_ray	_get_object_nw(t_player *player,
								t_map **map, float dist, char object, t_type type)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	t_fvector2	max_pos;
	float		angle;

	max_pos = (t_fvector2){player->f_real_pos.x - sin((360 - player->angle) * TO_RADIAN) * dist,
			player->f_real_pos.y - cos((360 - player->angle) * TO_RADIAN) * dist};
	angle = fabs(tan(player->angle * TO_RADIAN));
	map_pos = (t_vector2){(int)player->f_real_pos.x, (int)player->f_real_pos.y};
	comp.x = player->f_real_pos.x + fabs(player->f_real_pos.y - map_pos.y) * angle * -1;
	comp.y = player->f_real_pos.y + fabs(player->f_real_pos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle * -1, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x < max_pos.x)
				return ((t_ray){{-1, -1}, e_east});
			if ((map[(int)(comp.y)][map_pos.x - 1].type & type) == type
				&& (map[(int)(comp.y)][map_pos.x - 1].symbol == object || object == '\0'))
				return ((t_ray){{map_pos.x - 1, (int)(comp.y)}, e_east});
			if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_east});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y < max_pos.y)
				return ((t_ray){{-1, -1}, e_south});
			if ((map[map_pos.y - 1][(int)(comp.x)].type & type) == type
				&& (map[map_pos.y - 1][(int)(comp.x)].symbol == object || object == '\0'))
				return ((t_ray){{(int)(comp.x), map_pos.y - 1}, e_south});
			if ((map[map_pos.y - 1][(int)(comp.x)].type & WALL) == WALL)
				return ((t_ray){{-1, -1}, e_south});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

t_ray	get_object_hit(char object, t_type type, float dist, t_game *game)
{
	t_vector2	sign;

	sign = get_sign(game->player->angle);
	if (sign.x == 1 && sign.y == 1)
		return (_get_object_se(game->player, game->map, dist, object, type));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_object_ne(game->player, game->map, dist, object, type));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_object_sw(game->player, game->map, dist, object, type));
	else
		return (_get_object_nw(game->player, game->map, dist, object, type));
}
