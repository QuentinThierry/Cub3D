/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_game_raycaster.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 18:22:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/11 18:22:56 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	fix_angle(t_end *end, enum e_orientation orient)
{
	if (end->dir_angle > 180)
		end->dir_angle = end->dir_angle - 360;
	if (end->dir_angle < -180)
		end->dir_angle = 360 + end->dir_angle;
	if (orient == e_south && end->dir_angle < 0)
		end->dest_angle = 0;
}

/**
 * @brief init the struct end with the destination where the player need to go
 * 	and its view angle
 * 
 * @param end 
 * @param orient 
 * @param pos_exit 
 * @param player 
 */
void	find_dest(t_end *end, const enum e_orientation orient,
		const t_vector2 exit, const t_player *player)
{
	if (orient == e_north)
	{
		end->dest = (t_fvector2){exit.x + .5, exit.y - DIST_TO_WALL};
		end->dest_angle = 180;
	}
	else if (orient == e_south)
	{
		end->dest = (t_fvector2){exit.x + .5, exit.y + 1 + DIST_TO_WALL};
		end->dest_angle = 360;
	}
	else if (orient == e_east)
	{
		end->dest = (t_fvector2){exit.x + 1 + DIST_TO_WALL, exit.y + .5};
		end->dest_angle = 270;
	}
	else
	{
		end->dest = (t_fvector2){exit.x - DIST_TO_WALL, exit.y + .5};
		end->dest_angle = 90;
	}
	end->dir.x = end->dest.x - player->f_pos.x;
	end->dir.y = end->dest.y - player->f_pos.y;
	end->dir_angle = end->dest_angle - player->angle;
	fix_angle(end, orient);
}

/**
 * @brief init the end->dest to the coordonne behind the door that has been open
 * 
 * @param end 
 * @param player_pos 
 */
void	next_dest(t_end *end, const t_dvector2 player_pos)
{
	if (end->orient == e_south)
		end->dest.y -= 1 + DIST_TO_WALL;
	else if (end->orient == e_north)
		end->dest.y += 1 + DIST_TO_WALL;
	else if (end->orient == e_east)
		end->dest.x -= 1 + DIST_TO_WALL;
	else
		end->dest.x += 1 + DIST_TO_WALL;
	end->dir.x = end->dest.x - player_pos.x;
	end->dir.y = end->dest.y - player_pos.y;
	end->dir_angle = 0;
	end->status = e_open_door;
}

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
	if (game->end->orient == e_north && ray->hit.y > game->end->dest.y - .5)
	{
		ray->hit = (t_dvector2){game->end->dest.x, game->end->dest.y - .5};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_pos, ray->hit));
	}
	if (game->end->orient == e_south && ray->hit.y < game->end->dest.y + .5)
	{
		ray->hit = (t_dvector2){game->end->dest.x, game->end->dest.y + .5};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_pos, ray->hit));
	}
	if (game->end->orient == e_east && ray->hit.x < game->end->dest.x + .5)
	{
		ray->hit = (t_dvector2){game->end->dest.x + .5, game->end->dest.y};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_pos, ray->hit));
	}
	if (game->end->orient == e_west && ray->hit.x > game->end->dest.x - .5)
	{
		ray->hit = (t_dvector2){game->end->dest.x - .5, game->end->dest.y};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_pos, ray->hit));
	}
	return (get_dist(game->player->f_pos, ray->hit)
		* cosf(angle * TO_RADIAN));
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
	x = -WIN_X / 2;
	draw_ceiling(game);
	while (x < WIN_X / 2)
	{
		angle = atanf(x / game->constants[0]) * 180 / M_PI;
		if (game->player->angle + angle >= 360)
			angle = angle - 360;
		else if (game->player->angle + angle < 0)
			angle = angle + 360;
		ray = get_wall_hit_end(fpos, game->map, game->player->angle
			+ angle, game->end->status);
		if (game->end->status == e_open_door
			|| game->end->status == e_walk || game->end->status == e_end)
			dist = draw_light(game, &ray, angle);
		else
			dist = get_dist(fpos, ray.hit) * cosf(angle * TO_RADIAN);
		game->dist_tab[x + WIN_X / 2] = dist;
		if (dist == 0)
			dist = 0.01;
		draw_vert(game, x + WIN_X / 2, ray, 1 / dist * game->constants[0]);
		x++;
	}
}
