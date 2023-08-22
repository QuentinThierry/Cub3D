/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:04:05 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/22 20:58:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

// xy (1, 1)
static t_ray	_get_wall_hit_se(t_fvector2 fpos,
								t_map **map, float angle, t_game *game) // change game to game->time ?
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	t_fvector2	door;
	float		player_angle;

	player_angle = angle;
	angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_fvector2){angle, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x].type & WALL) == true)
			{
				if (map[(int)(comp.y)][map_pos.x].symbol == 'c')
				{
					step_door_open(map[(int)(comp.y)][map_pos.x].arg, game->time, &map[(int)(comp.y)][map_pos.x]);
					door = door_hit_ver_se((t_fvector2){map_pos.x, comp.y}, step.y
						, (float)((t_door *)map[(int)(comp.y)][map_pos.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_west});
				}
				else
					return ((t_ray){{map_pos.x, comp.y}, e_west});
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else 
		{
			if ((map[map_pos.y][(int)(comp.x)].type & WALL) == true)
			{
				if (map[map_pos.y][(int)(comp.x)].symbol == 'c')
				{
					step_door_open(map[map_pos.y][(int)(comp.x)].arg, game->time, &map[map_pos.y][(int)(comp.x)]);
					door = door_hit_hor_se((t_fvector2){comp.x, map_pos.y}, step.x
						, (float)((t_door *)map[map_pos.y][(int)comp.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_north});
				}
				else
					return ((t_ray){{comp.x, map_pos.y}, e_north});
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static t_ray	_get_wall_hit_ne(t_fvector2 fpos,
								t_map **map, float angle, t_game *game)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_fvector2	door;
	t_vector2	map_pos;
	float		player_angle;

	player_angle = angle;
	angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x].type & WALL) == true)
			{
				if (map[(int)(comp.y)][map_pos.x].symbol == 'c')
				{
					step_door_open(map[(int)(comp.y)][map_pos.x].arg, game->time, &map[(int)(comp.y)][map_pos.x]);
					door = door_hit_ver_ne((t_fvector2){map_pos.x, comp.y}, step.y
						, (float)((t_door *)map[(int)(comp.y)][map_pos.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_west});
				}
				else
					return ((t_ray){{map_pos.x, comp.y}, e_west});
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if ((map[map_pos.y - 1][(int)(comp.x)].type & WALL) == true)
			{
				if (map[map_pos.y - 1][(int)(comp.x)].symbol == 'c')
				{
					step_door_open(map[map_pos.y - 1][(int)(comp.x)].arg, game->time, &map[map_pos.y - 1][(int)(comp.x)]);
					door = door_hit_hor_ne((t_fvector2){comp.x, map_pos.y}, step.x
						, (float)((t_door *)map[map_pos.y - 1][(int)comp.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_south});
				}
				else
					return ((t_ray){{comp.x, map_pos.y}, e_south});
			}

			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static t_ray	_get_wall_hit_sw(t_fvector2 fpos,
								t_map **map, float angle, t_game *game)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_fvector2	door;
	t_vector2	map_pos;
	float		player_angle;

	player_angle = angle;
	angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_fvector2){angle * -1, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == true)
			{
				if (map[(int)(comp.y)][map_pos.x - 1].symbol == 'c')
				{
					step_door_open(map[(int)(comp.y)][map_pos.x - 1].arg, game->time, &map[(int)(comp.y)][map_pos.x - 1]);
					door = door_hit_ver_sw((t_fvector2){map_pos.x, comp.y}, step.y
						, (float)((t_door *)map[(int)(comp.y)][map_pos.x - 1].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_east});
				}
				else
					return ((t_ray){{map_pos.x, comp.y}, e_east});
			}
			comp.y += step.y;
			map_pos.x -= 1;
		}
		else
		{
			if ((map[map_pos.y][(int)(comp.x)].type & WALL) == true)
			{
				if (map[map_pos.y][(int)(comp.x)].symbol == 'c')
				{
					step_door_open(map[map_pos.y][(int)(comp.x)].arg, game->time, &map[map_pos.y][(int)(comp.x)]);
					door = door_hit_hor_sw((t_fvector2){comp.x, map_pos.y}, step.x
						, (float)((t_door *)map[map_pos.y][(int)comp.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_north});
				}
				else
					return ((t_ray){{comp.x, map_pos.y}, e_north});
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static t_ray	_get_wall_hit_nw(t_fvector2 fpos,
								t_map **map, float angle, t_game *game)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_fvector2	door;
	t_vector2	map_pos;
	float		player_angle;

	player_angle = angle;
	angle = fabs(tan(angle * TO_RADIAN));
	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle * -1, 1 / angle * -1};
	while (true)
	{
		
		if (map_pos.y <= comp.y)
		{
			if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == true)
			{
				if (map[(int)(comp.y)][map_pos.x - 1].symbol == 'c')
				{
					step_door_open(map[(int)(comp.y)][map_pos.x - 1].arg, game->time, &map[(int)(comp.y)][map_pos.x - 1]);
					door = door_hit_ver_nw((t_fvector2){map_pos.x, comp.y}, step.y
						, (float)((t_door *)map[(int)(comp.y)][map_pos.x - 1].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_east});
				}
				else
					return ((t_ray){{map_pos.x, comp.y}, e_east});
			}
			comp.y += step.y;
			map_pos.x -= 1;
		}
		else
		{
			if ((map[map_pos.y - 1][(int)(comp.x)].type & WALL) == true)
			{
				if (map[map_pos.y - 1][(int)(comp.x)].symbol == 'c')
				{
					step_door_open(map[map_pos.y - 1][(int)(comp.x)].arg, game->time, &map[map_pos.y - 1][(int)(comp.x)]);
					door = door_hit_hor_nw((t_fvector2){comp.x, map_pos.y}, step.x
						, (float)((t_door *)map[map_pos.y - 1][(int)comp.x].arg)->door_percent, player_angle);
					if (door.x != -1)
						return ((t_ray){door, e_south});
				}
				else
					return ((t_ray){{comp.x, map_pos.y}, e_south});
			}

			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

t_ray	get_wall_hit(t_fvector2 fpos, t_map **map, float angle, t_game *game)
{
	t_vector2	sign;

	sign = get_sign(angle);
	if (sign.x == 1 && sign.y == 1)
		return (_get_wall_hit_se(fpos, map, angle, game));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_wall_hit_ne(fpos, map, angle, game));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_wall_hit_sw(fpos, map, angle, game));
	else
		return (_get_wall_hit_nw(fpos, map, angle, game));
	return ((t_ray){});
}
