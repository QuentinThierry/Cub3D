/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 01:05:58 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/14 18:07:01 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static t_dvector2	slide_wall_x(t_dvector2 fpos, t_map **map, t_dvector2 dest)
{
	int	x;
	int	dir;
	double	pos;

	x = (int)fpos.x;
	dir = ((dest.x - fpos.x) > 0) * 2 - 1;
	while ((dir == 1 && x < dest.x) || (dir == -1 && x > dest.x))
	{
		if ((map[(int)fpos.y][x].type & WALL) == WALL)
			return ((t_dvector2){x + (dir == -1) + (DIST_TO_WALL + 0.0001) * dir * -1, fpos.y});
		x += dir;
	}
	if (dir == -1 && ((map[(int)fpos.y][x].type & WALL) == WALL))
		return ((t_dvector2){x + (dir == -1) + (DIST_TO_WALL + 0.0001) * dir * -1, fpos.y});
	pos = dest.x + DIST_TO_WALL * dir;
	if ((map[(int)fpos.y][(int)pos].type & WALL) == WALL)
		return ((t_dvector2){(int)dest.x + (dir == 1) + (DIST_TO_WALL + 0.0001) * dir * -1, fpos.y});
	return ((t_dvector2){dest.x, fpos.y});
}

static t_dvector2	slide_wall_y(t_dvector2 fpos, t_map **map, t_dvector2 dest)
{
	int	y;
	int	dir;
	double	pos;

	y = (int)fpos.y;
	dir = ((dest.y - fpos.y) > 0) * 2 - 1;
	while ((dir == 1 && y < dest.y) || (dir == -1 && y > dest.y))
	{
		if ((map[y][(int)fpos.x].type & WALL) == WALL)
			return ((t_dvector2){fpos.x, y + (dir == -1) + (DIST_TO_WALL + 0.0001) * dir * -1});
		y += dir;
	}
	if (dir == -1 && ((map[y][(int)fpos.x].type & WALL) == WALL))
		return ((t_dvector2){fpos.x, y + (dir == -1) + (DIST_TO_WALL + 0.0001) * dir * -1});
	pos = dest.y + DIST_TO_WALL * dir;
	if ((map[(int)pos][(int)fpos.x].type & WALL) == WALL)
		return ((t_dvector2){fpos.x, (int)dest.y + (dir == 1) + (DIST_TO_WALL + 0.0001) * dir * -1});
	return ((t_dvector2){fpos.x, dest.y});
}

// xy (1, 1)
static t_dvector2	_get_collision_se(t_dvector2 fpos,
								t_map **map, t_dvector2 new_pos, t_player *player)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_dvector2	map_pos;
	float		angle;
	t_dvector2	tmp;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_dvector2){(int)fpos.x + 1 - DIST_TO_WALL, (int)fpos.y + 1 - DIST_TO_WALL};
	if (map_pos.x < fpos.x)
		map_pos.x += 1;
	if (map_pos.y < fpos.y)
		map_pos.y += 1;
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_dvector2){angle, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x > new_pos.x)
				return (new_pos);
			if ((map[(int)(comp.y + DIST_TO_WALL)][(int)(map_pos.x + DIST_TO_WALL)].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE)
				take_object(player, &map[(int)(comp.y + DIST_TO_WALL)][(int)(map_pos.x + DIST_TO_WALL)]);
			if ((map[(int)(comp.y + DIST_TO_WALL)][(int)(map_pos.x + DIST_TO_WALL)].type & WALL) == WALL
				|| (map[(int)(comp.y - DIST_TO_WALL)][(int)(map_pos.x + DIST_TO_WALL)].type & WALL) == WALL)
			{
				tmp = (t_dvector2){map_pos.x - 0.0001, comp.y - (0.0001 / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))};
				return (slide_wall_y(tmp, map, new_pos));
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else 
		{
			if (map_pos.y > new_pos.y)
				return (new_pos);
			if ((map[(int)(map_pos.y + DIST_TO_WALL)][(int)(comp.x + DIST_TO_WALL)].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE)
				take_object(player, &map[(int)(map_pos.y + DIST_TO_WALL)][(int)(comp.x + DIST_TO_WALL)]);
			if ((map[(int)(map_pos.y + DIST_TO_WALL)][(int)(comp.x + DIST_TO_WALL)].type & WALL) == WALL
				|| (map[(int)(map_pos.y + DIST_TO_WALL)][(int)(comp.x - DIST_TO_WALL)].type & WALL) == WALL)
			{
				tmp = (t_dvector2){comp.x - (0.0001 / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y - 0.0001};
				return (slide_wall_x(tmp, map, new_pos));
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static t_dvector2	_get_collision_ne(t_dvector2 fpos,
								t_map **map, t_dvector2 new_pos, t_player *player)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_dvector2	map_pos;
	float		angle;
	t_dvector2	tmp;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_dvector2){(int)fpos.x + 1 - DIST_TO_WALL, (int)fpos.y + DIST_TO_WALL};
	if (map_pos.x < fpos.x)
		map_pos.x += 1;
	if (map_pos.y > fpos.y)
		map_pos.y -= 1;
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_dvector2){angle, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x > new_pos.x)
				return (new_pos);
			if ((map[(int)(comp.y - DIST_TO_WALL)][(int)(map_pos.x + DIST_TO_WALL)].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE)
				take_object(player, &map[(int)(comp.y - DIST_TO_WALL)][(int)(map_pos.x + DIST_TO_WALL)]);
			if ((map[(int)(comp.y + DIST_TO_WALL)][(int)(map_pos.x + DIST_TO_WALL)].type & WALL) == WALL
				|| (map[(int)(comp.y - DIST_TO_WALL)][(int)(map_pos.x + DIST_TO_WALL)].type & WALL) == WALL)
			{
				tmp = (t_dvector2){map_pos.x - 0.0001, comp.y + (0.0001 / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))};
				return (slide_wall_y(tmp, map, new_pos));
			}
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if (map_pos.y < new_pos.y)
				return (new_pos);
			if ((map[(int)(map_pos.y) - 1][((int)(comp.x + DIST_TO_WALL))].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE)
				take_object(player, &map[(int)(map_pos.y) - 1][((int)(comp.x + DIST_TO_WALL))]);
			if ((map[(int)(map_pos.y) - 1][((int)(comp.x + DIST_TO_WALL))].type & WALL) == WALL
				|| (map[(int)(map_pos.y) - 1][((int)(comp.x - DIST_TO_WALL))].type & WALL) == WALL)
			{
				tmp = (t_dvector2){comp.x - (0.0001 / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y + 0.0001};
				return (slide_wall_x(tmp, map, new_pos));
			}
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static t_dvector2	_get_collision_sw(t_dvector2 fpos,
								t_map **map, t_dvector2 new_pos, t_player *player)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_dvector2	map_pos;
	float		angle;
	t_dvector2	tmp;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_dvector2){(int)fpos.x + DIST_TO_WALL, (int)fpos.y + 1 - DIST_TO_WALL};
	if (map_pos.x > fpos.x)
		map_pos.x -= 1;
	if (map_pos.y < fpos.y)
		map_pos.y += 1;
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_dvector2){angle * -1, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x < new_pos.x)
				return (new_pos);
			if ((map[(int)(comp.y + DIST_TO_WALL)][(int)(map_pos.x) - 1].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE)
				take_object(player, &map[(int)(comp.y + DIST_TO_WALL)][(int)(map_pos.x) - 1]);
			if ((map[(int)(comp.y + DIST_TO_WALL)][(int)(map_pos.x) - 1].type & WALL) == WALL
				|| (map[(int)(comp.y - DIST_TO_WALL)][(int)(map_pos.x) - 1].type & WALL) == WALL)
			{
				tmp = (t_dvector2){map_pos.x + 0.0001, comp.y - (0.0001 / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))};
				return (slide_wall_y(tmp, map, new_pos));
			}
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y > new_pos.y)
				return (new_pos);
			if ((map[(int)(map_pos.y + DIST_TO_WALL)][(int)(comp.x - DIST_TO_WALL)].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE)
				take_object(player, &map[(int)(map_pos.y + DIST_TO_WALL)][(int)(comp.x - DIST_TO_WALL)]);
			if ((map[(int)(map_pos.y + DIST_TO_WALL)][((int)(comp.x + DIST_TO_WALL))].type & WALL) == WALL
				|| (map[(int)(map_pos.y + DIST_TO_WALL)][((int)(comp.x - DIST_TO_WALL))].type & WALL) == WALL)
			{
				tmp = (t_dvector2){comp.x + (0.0001 / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y - 0.0001};
				return (slide_wall_x(tmp, map, new_pos));
			}
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static t_dvector2	_get_collision_nw(t_dvector2 fpos,
								t_map **map, t_dvector2 new_pos, t_player *player)
{
	t_dvector2	step;
	t_dvector2	comp;
	t_dvector2	map_pos;
	float		angle;
	t_dvector2	tmp;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_dvector2){(int)fpos.x + DIST_TO_WALL, (int)fpos.y + DIST_TO_WALL};
	if (map_pos.x > fpos.x)
		map_pos.x -= 1;
	if (map_pos.y > fpos.y)
		map_pos.y -= 1;
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_dvector2){angle * -1, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x < new_pos.x)
				return (new_pos);
			if ((map[(int)(comp.y - DIST_TO_WALL)][(int)(map_pos.x) - 1].type & OBJECT) == OBJECT)
				take_object(player, &map[(int)(comp.y - DIST_TO_WALL)][(int)(map_pos.x) - 1]);
			if ((map[(int)(comp.y + DIST_TO_WALL)][(int)(map_pos.x) - 1].type & WALL) == WALL
				|| (map[(int)(comp.y - DIST_TO_WALL)][(int)(map_pos.x) - 1].type & WALL) == WALL)
			{
				tmp = (t_dvector2){map_pos.x + 0.0001, comp.y + (0.0001 / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))};
				return (slide_wall_y(tmp, map, new_pos));
			}
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y < new_pos.y)
				return (new_pos);
			if ((map[(int)(map_pos.y) - 1][((int)(comp.x - DIST_TO_WALL))].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE)
				take_object(player, &map[(int)(map_pos.y) - 1][((int)(comp.x - DIST_TO_WALL))]);
			if ((map[(int)(map_pos.y) - 1][((int)(comp.x + DIST_TO_WALL))].type & WALL) == WALL
				|| (map[(int)(map_pos.y) - 1][((int)(comp.x - DIST_TO_WALL))].type & WALL) == WALL)
			{
				tmp = (t_dvector2){comp.x + (0.0001 / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y + 0.0001};
				return (slide_wall_x(tmp, map, new_pos));
			}
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

t_dvector2	check_colliding(t_player *player, t_dvector2 new_pos, t_map **map)
{
	t_vector2	sign;

	if (player->f_real_pos.x - new_pos.x < 0)
		sign.x = 1;
	else
		sign.x = -1;
	if (player->f_real_pos.y - new_pos.y < 0)
		sign.y = 1;
	else
		sign.y = -1;
	if (sign.x == 1 && sign.y == 1)
		return (_get_collision_se(player->f_real_pos, map, new_pos, player));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_collision_ne(player->f_real_pos, map, new_pos, player));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_collision_sw(player->f_real_pos, map, new_pos, player));
	else
		return (_get_collision_nw(player->f_real_pos, map, new_pos, player));
}
