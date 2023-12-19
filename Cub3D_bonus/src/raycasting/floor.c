/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:50:23 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/14 17:35:39 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline void	for_each_line_pixel(t_game *game, t_floor_infos *infos,
	int i, int y_screen)
{
	if (game->map[(int)infos->map_point.y]
		[(int)infos->map_point.x].sprite[e_ceiling].index != -1)
	{
		if (infos->last_map_pos.x != (int)infos->map_point.x
			|| infos->last_map_pos.y != (int)infos->map_point.y)
		{
			infos->last_map_pos.x = (int)infos->map_point.x;
			infos->last_map_pos.y = (int)infos->map_point.y;
			infos->img_ceil
				= get_image_non_wall(game, infos->map_point, e_ceiling);
			infos->img_floor
				= get_image_non_wall(game, infos->map_point, e_floor);
		}
		compute_pixel(infos, i, y_screen);
	}
}

__attribute__((always_inline))
static inline bool	is_in_map(t_dvector2 pos, t_map **map, t_vector2 size_map)
{
	if (pos.x < size_map.x && pos.x >= 0 && pos.y < size_map.y && pos.y >= 0
		&& map[(int)pos.y][(int)pos.x].symbol != ' ')
		return (true);
	return (false);
}

__attribute__((always_inline))
static inline void	draw_pixel_line(t_game *game, t_dvector2 map_point,
			t_fvector2 step_dir, int y_screen)
{
	t_floor_infos	infos;
	int				i;

	infos = (t_floor_infos){0};
	infos.map_point = map_point;
	infos.last_map_pos.x = -(int)infos.map_point.x;
	infos.last_map_pos.y = -(int)infos.map_point.y;
	infos.game = game;
	i = 0;
	while (i < WIN_X)
	{
		if ((game->height_tab[i] > y_screen && game->end->status == e_game)
			|| !is_in_map(infos.map_point, game->map, game->map_size))
		{
			infos.map_point.x += step_dir.x;
			infos.map_point.y += step_dir.y;
			i++;
			continue ;
		}
		for_each_line_pixel(game, &infos, i, y_screen);
		infos.map_point.x += step_dir.x;
		infos.map_point.y += step_dir.y;
		i++;
	}
}

__attribute__((always_inline))
static inline void	draw_line_ceiling(t_game *game, t_fvector2 xdist_yscreen,
	t_fvector2 cos_sin, t_fvector2 hit)
{
	float				dist_to_left;
	float				h;
	t_fvector2			a;
	t_dvector2			map_point;
	t_dvector2			left_p;

	dist_to_left = fabsf(game->constants[TAN_HALF_FOV] * xdist_yscreen.x);
	h = xdist_yscreen.x / game->constants[COS_HALF_FOV];
	left_p.x = cos_sin.x * h;
	left_p.y = cos_sin.y * h;
	a.y = ((hit.y - left_p.y) / dist_to_left) * fabsf(dist_to_left * 2) / WIN_X;
	a.x = ((hit.x - left_p.x) / dist_to_left) * fabsf(dist_to_left * 2) / WIN_X;
	map_point.x = game->player->f_pos.x + left_p.x;
	map_point.y = game->player->f_pos.y + left_p.y;
	draw_pixel_line(game, map_point, a, xdist_yscreen.y);
}

// if odd WIN_Y, draw 1 floor pixel more than ceiling
void	draw_ceiling(t_game *game)
{
	t_fvector2	hit;
	t_fvector2	cos_sin1;
	t_fvector2	cos_sin2;
	int			y_screen;
	float		x_dist;

	y_screen = WIN_Y / 2.;
	cos_sin2.x = cosf((game->player->angle - 90 - game->fov / 2.) * TO_RADIAN);
	cos_sin2.y = sinf((game->player->angle - 90 - game->fov / 2.) * TO_RADIAN);
	cos_sin1.x = cosf((game->player->angle - 90) * TO_RADIAN);
	cos_sin1.y = sinf((game->player->angle - 90) * TO_RADIAN);
	while (y_screen > 0)
	{
		x_dist = (0.5 * (game->constants[0] / y_screen));
		hit.x = cos_sin1.x * x_dist;
		hit.y = cos_sin1.y * x_dist;
		draw_line_ceiling(game, (t_fvector2){x_dist, y_screen}, cos_sin2, hit);
		y_screen--;
	}
}
