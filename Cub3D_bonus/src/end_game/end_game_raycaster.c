/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_game_raycaster.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 18:22:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/17 16:22:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline float	get_dist(t_dvector2 fpos, t_dvector2 wall)
{
	return (sqrtf((wall.x - fpos.x) * (wall.x - fpos.x) + (wall.y - fpos.y)
			* (wall.y - fpos.y)));
}

/**
 * @brief draw light behind the opening door
 * 
 * @param game 
 * @param ray 
 * @param angle 
 * @return float 
 */
float	draw_light(t_game *game, t_ray *ray, float angle)
{
	if (game->end->orient == e_north && ray->hit.y > game->end->dest.y)
	{
		ray->hit = (t_dvector2){game->end->dest.x, game->end->dest.y};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_pos, ray->hit));
	}
	if (game->end->orient == e_south && ray->hit.y < game->end->dest.y)
	{
		ray->hit = (t_dvector2){game->end->dest.x, game->end->dest.y};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_pos, ray->hit));
	}
	if (game->end->orient == e_east && ray->hit.x < game->end->dest.x)
	{
		ray->hit = (t_dvector2){game->end->dest.x, game->end->dest.y};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_pos, ray->hit));
	}
	if (game->end->orient == e_west && ray->hit.x > game->end->dest.x)
	{
		ray->hit = (t_dvector2){game->end->dest.x, game->end->dest.y};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_pos, ray->hit));
	}
	return (get_dist(game->player->f_pos, ray->hit) * cosf(angle * TO_RADIAN));
}

__attribute__((always_inline))
static inline float	set_dist(enum e_status status, t_game *game, t_ray *ray,
					float angle)
{
	float	dist;

	if (status == e_open_door || status == e_walk
		|| status == e_end)
		dist = draw_light(game, ray, angle);
	else
		dist = get_dist(game->player->f_pos, ray->hit)
			* cosf(angle * TO_RADIAN);
	if (dist == 0)
			dist = 0.01;
	return (dist);
}

/**
 * @brief raycsting to open all the door by pulling them + draw light behind them
 * 
 * @param game 
 */
void	raycasting_end(t_game *game)
{
	int			x;
	float		angle;
	t_ray		ray;
	t_dvector2	fpos;
	float		dist;

	fpos = game->player->f_pos;
	x = -WIN_X / 2 - 1;
	draw_ceiling(game);
	while (++x < WIN_X / 2)
	{
		angle = atanf(x / game->constants[0]) * 180 / M_PI;
		if (game->player->angle + angle >= 360)
			angle = angle - 360;
		else if (game->player->angle + angle < 0)
			angle = angle + 360;
		ray = get_wall_hit_end(fpos, game->map, game->player->angle + angle,
				game->end->status);
		dist = set_dist(game->end->status, game, &ray, angle);
		game->dist_tab[x + WIN_X / 2] = dist;
		draw_vert(game, x + WIN_X / 2, ray, 1 / dist * game->constants[0]);
	}
}
