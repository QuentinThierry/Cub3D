/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 01:05:58 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/11 14:48:54 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"


t_dvector2	get_collision(t_dvector2 fpos, t_map **map, t_dvector2 new_pos);

void	check_colliding(t_player *player, t_dvector2 new_pos, t_map **map)
{
	int dir_x;
	int dir_y;

	dir_x = ((player->f_real_pos.x - new_pos.x) > 0) * 2 -1;
	dir_y = ((player->f_real_pos.y - new_pos.y) > 0) * 2 -1;
	new_pos.x -= DIST_TO_WALL * dir_x;
	new_pos.y -= DIST_TO_WALL * dir_y;
	// player->f_real_pos.x += DIST_TO_WALL;
	player->f_real_pos = get_collision(player->f_real_pos, map, new_pos);
	// player->f_real_pos.x -= DIST_TO_WALL;
	if (player->f_real_pos.x == new_pos.x)
		player->f_real_pos.x += DIST_TO_WALL * dir_x;
	if (player->f_real_pos.y == new_pos.y)
		player->f_real_pos.y += DIST_TO_WALL * dir_y;


}

static t_dvector2	slide_wall_x(t_dvector2 fpos, t_map **map, t_dvector2 dest)
{
	int	dir;
	int	x;

	dir = ((dest.x - fpos.x) > 0) * 2 - 1;
	x = (int)fpos.x;
	while ((dest.x - x) * dir > 0)
	{
		if ((map[(int)fpos.y][x].type & WALL) == WALL)
			return ((t_dvector2){x + (DIST_TO_WALL * dir * -1) + (dir == -1), fpos.y});
		x += dir;
	}
	if (dir == -1 && ((map[(int)fpos.y][x].type & WALL) == WALL))
		return ((t_dvector2){x + (DIST_TO_WALL * dir * -1) + (dir == -1), fpos.y});
	return ((t_dvector2){dest.x, fpos.y});
}

static t_dvector2	slide_wall_y(t_dvector2 fpos, t_map **map, t_dvector2 dest)
{
	int	dir;
	int	y;

	dir = ((dest.y - fpos.y) > 0) * 2 - 1;
	y = (int)fpos.y;
	while ((dest.y - y) * dir > 0)
	{
		if ((map[y][(int)fpos.x].type & WALL) == WALL)
			return ((t_dvector2){fpos.x, y + (DIST_TO_WALL * dir * -1) + (dir == -1)});
		y += dir;
	}
	if (dir == -1 && ((map[y][(int)fpos.x].type & WALL) == WALL))
		return ((t_dvector2){fpos.x, y + (DIST_TO_WALL * dir * -1) + (dir == -1)});
	return ((t_dvector2){fpos.x, dest.y});
}

// xy (1, 1)
static inline t_dvector2	_get_collision_se(t_dvector2 fpos,
								t_map **map, t_dvector2 new_pos)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_vector2	map_pos;
	float		angle;
	t_dvector2	tmp;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_dvector2){angle, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x > new_pos.x)
				return (new_pos);
			if ((map[(int)(comp.y)][map_pos.x].type & WALL) == WALL)
			{
				tmp = (t_dvector2){map_pos.x - DIST_TO_WALL, comp.y - (DIST_TO_WALL / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))};
				return (slide_wall_y(tmp, map, new_pos));
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else 
		{
			if (map_pos.y > new_pos.y)
				return (new_pos);
			if ((map[map_pos.y][(int)(comp.x)].type & WALL) == WALL)
			{
				tmp = (t_dvector2){comp.x - (DIST_TO_WALL / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y - DIST_TO_WALL};
				return (slide_wall_x(tmp, map, new_pos));
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static inline t_dvector2	_get_collision_ne(t_dvector2 fpos,
								t_map **map, t_dvector2 new_pos)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_vector2	map_pos;
	float		angle;
	t_dvector2	tmp;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_dvector2){angle, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x > new_pos.x)
				return (new_pos);
			if ((map[(int)(comp.y)][map_pos.x].type & WALL) == WALL)
			{
				tmp = (t_dvector2){map_pos.x - DIST_TO_WALL, comp.y + (DIST_TO_WALL / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))};
				return (slide_wall_y(tmp, map, new_pos));
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if (map_pos.y < new_pos.y)
				return (new_pos);
			if ((map[map_pos.y - 1][((int)(comp.x))].type & WALL) == WALL)
			{
				tmp = (t_dvector2){comp.x - (DIST_TO_WALL / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y + DIST_TO_WALL};
				return (slide_wall_x(tmp, map, new_pos));
			}
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static inline t_dvector2	_get_collision_sw(t_dvector2 fpos,
								t_map **map, t_dvector2 new_pos)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_vector2	map_pos;
	float		angle;
	t_dvector2	tmp;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_dvector2){angle * -1, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x < new_pos.x)
				return (new_pos);
			if ((map[(int)(comp.y)][map_pos.x - 1].type & WALL) == WALL)
			{
				tmp = (t_dvector2){map_pos.x + DIST_TO_WALL, comp.y - (DIST_TO_WALL / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))};
				return (slide_wall_y(tmp, map, new_pos));
			}
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y > new_pos.y)
				return (new_pos);
			if ((map[map_pos.y][((int)(comp.x))].type & WALL) == WALL)
			{
				tmp = (t_dvector2){comp.x + (DIST_TO_WALL / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y - DIST_TO_WALL};
				return (slide_wall_x(tmp, map, new_pos));
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static inline t_dvector2	_get_collision_nw(t_dvector2 fpos,
								t_map **map, t_dvector2 new_pos)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_vector2	map_pos;
	float		angle;
	t_dvector2	tmp;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_dvector2){angle * -1, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x < new_pos.x)
				return (new_pos);
			if ((map[(int)comp.y][map_pos.x - 1].type & WALL) == WALL)
			{
				tmp = (t_dvector2){map_pos.x + DIST_TO_WALL, comp.y + (DIST_TO_WALL / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))};
				return (slide_wall_y(tmp, map, new_pos));
			}
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y < new_pos.y)
				return (new_pos);
			if ((map[map_pos.y - 1][((int)(comp.x))].type & WALL) == WALL)
			{
				tmp = (t_dvector2){comp.x + (DIST_TO_WALL / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y + DIST_TO_WALL};
				return (slide_wall_x(tmp, map, new_pos));
			}
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

inline t_dvector2	get_collision(t_dvector2 fpos, t_map **map, t_dvector2 new_pos)
{
	t_vector2	sign;

	if (fpos.x - new_pos.x < 0)
		sign.x = 1;
	else
		sign.x = -1;
	if (fpos.y - new_pos.y < 0)
		sign.y = 1;
	else
		sign.y = -1;
	if (sign.x == 1 && sign.y == 1)
		return (_get_collision_se(fpos, map, new_pos));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_collision_ne(fpos, map, new_pos));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_collision_sw(fpos, map, new_pos));
	else
		return (_get_collision_nw(fpos, map, new_pos));
	return ((t_dvector2){0});
}
