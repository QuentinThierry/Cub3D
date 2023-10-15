/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 01:05:58 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/15 17:33:54 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_dvector2	get_collision_se(t_game *game, t_map **map, float angle,
				t_info_collison *inf);
t_dvector2	get_collision_ne(t_game *game, t_map **map, float angle,
				t_info_collison *inf);
t_dvector2	get_collision_sw(t_game *game, t_map **map, float angle,
				t_info_collison *inf);
t_dvector2	get_collision_nw(t_game *game, t_map **map, float angle,
				t_info_collison *inf);

static const float	g_dist_wall = DIST_WALL + 0.0001;

__attribute__((always_inline))
static inline float	get_dist(t_dvector2 fpos, t_dvector2 wall)
{
	return (sqrtf((wall.x - fpos.x) * (wall.x - fpos.x)
			+ (wall.y - fpos.y) * (wall.y - fpos.y)));
}

void	check_interactive_object(t_game *game, t_map **map,
					t_dvector2 fpos)
{
	t_ray	ray;

	ray = get_object_hit((t_launch_ray){'\0', OBJ_INTER,
			get_dist(game->player->f_pos, fpos)},
			map, game->player->f_pos, game->player->angle);
	if (ray.hit.x != -1)
		take_object(game, game->player, &map[(int)ray.hit.y][(int)ray.hit.x],
			game->music_array);
}

t_dvector2	slide_wall_x(t_game *gm, t_dvector2 fpos, t_map **map,
					t_dvector2 dest)
{
	int		x;
	int		dir;
	double	pos;

	x = (int)fpos.x;
	dir = ((dest.x - fpos.x) > 0) * 2 - 1;
	check_interactive_object(gm, map, fpos);
	while ((dir == 1 && x < dest.x) || (dir == -1 && x > dest.x))
	{
		if ((map[(int)fpos.y][x].type & OBJ_INTER) == OBJ_INTER)
			take_object(gm, gm->player, &map[(int)fpos.y][x], gm->music_array);
		if ((map[(int)fpos.y][x].type & WALL) == WALL)
			return ((t_dvector2){x + (dir == -1) + g_dist_wall * dir * -1,
				fpos.y});
		x += dir;
	}
	if (dir == -1 && (map[(int)fpos.y][x].type & OBJ_INTER) == OBJ_INTER)
		take_object(gm, gm->player, &map[(int)fpos.y][x], gm->music_array);
	if (dir == -1 && ((map[(int)fpos.y][x].type & WALL) == WALL))
		return ((t_dvector2){x + (dir == -1) + g_dist_wall * dir * -1, fpos.y});
	pos = dest.x + DIST_WALL * dir;
	if ((map[(int)fpos.y][(int)pos].type & WALL) == WALL)
		return ((t_dvector2){(int)dest.x + (dir == 1) + g_dist_wall
			* dir * -1, fpos.y});
	return ((t_dvector2){dest.x, fpos.y});
}

t_dvector2	slide_wall_y(t_game *gm, t_dvector2 fpos, t_map **map,
				t_dvector2 dest)
{
	int		y;
	int		dir;
	double	pos;

	y = (int)fpos.y;
	dir = ((dest.y - fpos.y) > 0) * 2 - 1;
	check_interactive_object(gm, map, fpos);
	while ((dir == 1 && y < dest.y) || (dir == -1 && y > dest.y))
	{
		if ((map[y][(int)fpos.x].type & OBJ_INTER) == OBJ_INTER)
			take_object(gm, gm->player, &map[y][(int)fpos.x], gm->music_array);
		if ((map[y][(int)fpos.x].type & WALL) == WALL)
			return ((t_dvector2){fpos.x, y + (dir == -1)
				+ g_dist_wall * dir * -1});
		y += dir;
	}
	if (dir == -1 && (map[y][(int)fpos.x].type & OBJ_INTER) == OBJ_INTER)
		take_object(gm, gm->player, &map[y][(int)fpos.x], gm->music_array);
	if (dir == -1 && (map[y][(int)fpos.x].type & WALL) == WALL)
		return ((t_dvector2){fpos.x, y + (dir == -1) + g_dist_wall * dir * -1});
	pos = dest.y + DIST_WALL * dir;
	if ((map[(int)pos][(int)fpos.x].type & WALL) == WALL)
		return ((t_dvector2){fpos.x, (int)dest.y + (dir == 1)
			+ g_dist_wall * dir * -1});
	return ((t_dvector2){fpos.x, dest.y});
}

t_dvector2	check_colliding(t_game *game, t_dvector2 new_pos, t_map **map)
{
	t_vector2		sign;
	t_info_collison	info;
	float			angle;

	if (game->player->f_pos.x - new_pos.x < 0)
		sign.x = 1;
	else
		sign.x = -1;
	if (game->player->f_pos.y - new_pos.y < 0)
		sign.y = 1;
	else
		sign.y = -1;
	info.new_pos = new_pos;
	info.fpos = game->player->f_pos;
	angle = fabs(info.fpos.x - new_pos.x) / fabs(info.fpos.y - new_pos.y);
	if (sign.x == 1 && sign.y == 1)
		return (get_collision_se(game, map, angle, &info));
	else if (sign.x == 1 && sign.y == -1)
		return (get_collision_ne(game, map, angle, &info));
	else if (sign.x == -1 && sign.y == 1)
		return (get_collision_sw(game, map, angle, &info));
	else
		return (get_collision_nw(game, map, angle, &info));
}
