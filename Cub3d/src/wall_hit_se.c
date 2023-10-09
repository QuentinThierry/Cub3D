/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit_se.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 19:26:00 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/09 19:37:49 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static t_fvector2	find_wall(t_map_arg map, t_fvector2 comp,
		t_fvector2 step, t_vector2 map_pos)
{
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x >= map.map_size.x || (int)(comp.y) >= map.map_size.y)
				return ((t_fvector2){-1, -1});
			if (map.map[(int)(comp.y)][map_pos.x] == '1')
				return ((t_fvector2){map_pos.x, comp.y});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if ((int)(comp.x) >= map.map_size.x || map_pos.y >= map.map_size.y)
				return ((t_fvector2){-1, -1});
			if (map.map[map_pos.y][(int)(comp.x)] == '1')
				return ((t_fvector2){comp.x, map_pos.y});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, 1)
t_fvector2	get_wall_hit_se(t_fvector2 fpos,
								char **map, float angle, t_vector2 map_size)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;

	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_fvector2){angle, 1 / angle};
	return (find_wall((t_map_arg){map, map_size}, comp, step, map_pos));
}
