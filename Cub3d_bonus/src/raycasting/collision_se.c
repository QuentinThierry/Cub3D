/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_se.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 17:08:54 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 17:12:32 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline t_dvector2	_collision_hit_ver_se(t_game *game, t_map **map,
					float step, t_info_collison *inf)
{
	t_dvector2	tmp;

	if (inf->pos.x > inf->new_pos.x)
		return (check_interactive_object(game, map, inf->new_pos),
			inf->new_pos);
	if ((map[(int)(inf->comp.y + DIST_WALL)][(int)(inf->pos.x + DIST_WALL)]
			.type & WALL) == WALL
		|| (map[(int)(inf->comp.y - DIST_WALL)][(int)(inf->pos.x + DIST_WALL)]
			.type & WALL) == WALL)
	{
		tmp = (t_dvector2){inf->pos.x - 0.0001, inf->comp.y - (0.0001
				/ fabs(inf->fpos.x - inf->pos.x)
				* fabs(inf->fpos.y - inf->comp.y))};
		return (slide_wall_y(game, tmp, map, inf->new_pos));
	}
	inf->comp.y += step;
	inf->pos.x += 1;
	return ((t_dvector2){-1, -1});
}

__attribute__((always_inline))
static inline t_dvector2	_collision_hit_hor_se(t_game *game, t_map **map,
					float step, t_info_collison *inf)
{
	t_dvector2	tmp;

	if (inf->pos.y > inf->new_pos.y)
		return (check_interactive_object(game, map, inf->new_pos),
			inf->new_pos);
	if ((map[(int)(inf->pos.y + DIST_WALL)][(int)(inf->comp.x + DIST_WALL)]
			.type & WALL) == WALL
		|| (map[(int)(inf->pos.y + DIST_WALL)][(int)(inf->comp.x - DIST_WALL)]
			.type & WALL) == WALL)
	{
		tmp = (t_dvector2){inf->comp.x
			- (0.0001 / fabs(inf->fpos.y - inf->pos.y)
				* fabs(inf->fpos.x - inf->comp.x)), inf->pos.y - 0.0001};
		return (slide_wall_x(game, tmp, map, inf->new_pos));
	}
	inf->comp.x += step;
	inf->pos.y += 1;
	return ((t_dvector2){-1, -1});
}

// xy (1, 1)
t_dvector2	get_collision_se(t_game *game, t_map **map, float angle,
					t_info_collison *inf)
{
	t_dvector2	res;

	inf->pos = (t_dvector2){(int)inf->fpos.x +1 - DIST_WALL,
		(int)inf->fpos.y +1 - DIST_WALL};
	inf->pos = (t_dvector2){inf->pos.x + (inf->pos.x < inf->fpos.x),
		inf->pos.y + (inf->pos.y < inf->fpos.y)};
	inf->comp.x = inf->fpos.x + fabs(inf->fpos.y - inf->pos.y) * angle;
	inf->comp.y = inf->fpos.y + fabs(inf->fpos.x - inf->pos.x) / angle;
	while (true)
	{
		if (inf->pos.y >= inf->comp.y)
		{
			res = _collision_hit_ver_se(game, map, 1 / angle, inf);
			if (res.x != -1)
				return (res);
		}
		else
		{
			res = _collision_hit_hor_se(game, map, angle, inf);
			if (res.x != -1)
				return (res);
		}
	}
}