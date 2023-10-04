/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_of_the_game.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 18:22:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/04 14:31:07 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	raycasting_end(t_game *game);

/**
 * @brief move the player to end->dest and turn his head atthe same time
 * 
 * @param player 
 * @param end 
 * @param delta_time 
 * @return true 
 * @return false 
 */
bool	move_to_dest(t_player *player, const t_end *end, const double delta_time)
{
	player->f_real_pos.x += end->dir.x * SPEED / 3. * delta_time;
	player->f_real_pos.y += end->dir.y * SPEED / 3. * delta_time;
	player->angle += end->dir_angle / 3. * delta_time;
	if ((end->dir.x < 0 && player->f_real_pos.x < end->dest.x)
		|| (end->dir.x > 0 && player->f_real_pos.x > end->dest.x))
		player->f_real_pos.x = end->dest.x;
	if ((end->dir.y < 0 && player->f_real_pos.y < end->dest.y)
		|| (end->dir.y > 0 && player->f_real_pos.y > end->dest.y))
		player->f_real_pos.y = end->dest.y;
	if ((end->dir_angle < 0 && player->angle < end->dest_angle)
		|| (end->dir_angle > 0 && player->angle > end->dest_angle))
		player->angle = end->dest_angle;
	if (player->f_real_pos.x == end->dest.x && player->f_real_pos.y == end->dest.y)
	{
		if (player->angle == end->dest_angle || (end->dest_angle == 0 && player->angle == 360) ||
			(end->dest_angle == 360 && player->angle == 0))
			return (true);
	}
	return (false);
}

/**
 * @brief init hte process of open the exit door and open it until it's totally
 * 			open
 * 
 * @param game 
 * @param door 
 * @param end 
 */
void	open_exit(t_game *game, t_door *door, t_end *end)
{
	if (door->is_opening_door == 0)
	{
		door->is_opening_door = 1;
		door->time = game->time;
	}
	end_step_door_open(game->delta_time, game->exit, game->map, end);
	if (door->door_percent == 90)
		end->status = e_walk_through_door;
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
void	find_dest(t_end *end, const enum e_orientation orient
		, const t_vector2 pos_exit, const t_player * player)
{
	if (orient == e_north)
	{
		end->dest = (t_fvector2){pos_exit.x + .5, pos_exit.y - DIST_TO_WALL};
		end->dest_angle = 180;
	}
	else if (orient == e_south)
	{
		end->dest = (t_fvector2){pos_exit.x + .5, pos_exit.y + 1 + DIST_TO_WALL};
		end->dest_angle = 360;
	}
	else if (orient == e_east)
	{
		end->dest = (t_fvector2){pos_exit.x + 1 + DIST_TO_WALL, pos_exit.y + .5};
		end->dest_angle = 270;
	}
	else
	{
		end->dest = (t_fvector2){pos_exit.x - DIST_TO_WALL, pos_exit.y + .5};
		end->dest_angle = 90;
	}
	end->dir.x = end->dest.x - player->f_real_pos.x;
	end->dir.y = end->dest.y - player->f_real_pos.y;
	end->dir_angle = end->dest_angle - player->angle;
	if (end->dir_angle > 180)
		end->dir_angle = end->dir_angle - 360;
	if (end->dir_angle < -180)
		end->dir_angle = 360 + end->dir_angle;
	if (orient == e_south && end->dir_angle < 0)
		end->dest_angle = 0;
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

/**
 * @brief loop for the end's screen
 * 
 * @param game 
 * @return int 
 */
int	update_end(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	struct timespec			time;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	clock_gettime(CLOCK_REALTIME, &time);
	update_sounds(game->music_array);
	game->time = time_to_long(&time);
	if (game->end->status == e_go_in_font_of_door)
	{
		if (move_to_dest(game->player, game->end, game->delta_time))
			next_dest(game->end, game->player->f_real_pos);
	}
	else if (game->end->status == e_open_door)
		open_exit(game, game->exit->arg, game->end);
	else if (game->end->status == e_walk_through_door)
		if (move_to_dest(game->player, game->end, game->delta_time))
			game->end->status = e_end;
	raycasting_end(game);
	print_subtitle(game, game->music_array[1].map_cell);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	last_time = cur_time;
	return (0);
}

/**
 * @brief launch the end's animation => go in front of the door, then open it,
 *		and go through it
 * 
 * @param game 
 * @param orient 
 */
void	end_of_the_game(t_game *game, const enum e_orientation orient)
{
	clear_sound(game->music_array);
	if ((game->exit->type & MUSIC) == MUSIC)
	{
		game->exit->music = get_music(game->file_music, game->nb_music
				, game->exit->symbol, e_music_receptacle_complete);
		play_music(game->exit, game->music_array, game->exit->music, IS_PLAYING_MUSIC);
		game->exit->type &= ~MUSIC;
	}
	if ((game->exit->type & NARRATOR) == NARRATOR
		|| (game->exit->type & NARRATOR_RECEPTACLE) == NARRATOR_RECEPTACLE)
	{
		game->exit->narrator = get_narrator(game->file_music, game->nb_music
				, game->exit->symbol, e_narrator_receptacle_complete);
		play_narrator(game, game->exit, game->music_array);
		game->exit->type &= ~NARRATOR & ~NARRATOR_RECEPTACLE;
	}
	find_dest(game->end, orient, ((t_door *)game->exit->arg)->map_pos, game->player);
	game->end->orient = orient;
	game->end->status = e_go_in_font_of_door;
	mlx_hook(game->win, 2, (1L << 0), exit_hook, game);
	mlx_hook(game->win, 6, (1L << 6), mouse_stay_in_window_hook, game);
	mlx_hook(game->win, 3, 0L, NULL, NULL);
	mlx_hook(game->win, 4, 0L, NULL, NULL);
	mlx_loop_hook(game->mlx_ptr, update_end, game);
}

__attribute__((always_inline))
static inline float	get_dist(t_dvector2 fpos, t_dvector2 wall)
{
	return (sqrtf((wall.x - fpos.x) * (wall.x - fpos.x) + (wall.y - fpos.y) * (wall.y - fpos.y)));
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
		return (get_dist(game->player->f_real_pos, ray->hit));
	}
	else if (game->end->orient == e_south && ray->hit.y < game->end->dest.y + .5)
	{
		ray->hit = (t_dvector2){game->end->dest.x, game->end->dest.y + .5};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_real_pos, ray->hit));
	}
	else if (game->end->orient == e_east && ray->hit.x < game->end->dest.x + .5)
	{
		ray->hit = (t_dvector2){game->end->dest.x + .5, game->end->dest.y};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_real_pos, ray->hit));
	}
	else if (game->end->orient == e_west && ray->hit.x > game->end->dest.x - .5)
	{
		ray->hit = (t_dvector2){game->end->dest.x - .5, game->end->dest.y};
		ray->orient = e_end_screen;
		return (get_dist(game->player->f_real_pos, ray->hit));
	}
	return (get_dist(game->player->f_real_pos, ray->hit) * cosf(angle * TO_RADIAN));
}

#include <float.h>

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

	fpos = game->player->f_real_pos;
	x = -WIN_X / 2;
	// for (int i = 0; i < WIN_X; i++) {
	// 	game->dist_tab[i] = FLT_MAX;
	// }
	draw_ceiling(game);
	while (x < WIN_X / 2)
	{
		angle = atanf(x / game->constants[0]) * 180 / M_PI;
		if (game->player->angle + angle >= 360)
			angle = angle - 360;
		else if (game->player->angle + angle < 0)
			angle = angle + 360;
		ray = get_wall_hit_end(fpos, game->map, game->player->angle + angle, game->end->status);
		if (game->end->status == e_open_door || game->end->status == e_walk_through_door || game->end->status == e_end)
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

bool	init_end_screen(t_game *game)
{
	game->end = ft_calloc(1, sizeof(t_end));
	if (game->end == NULL)
		return (false);
	game->end->status = -1;
	game->end->end_screen = btmlx_xpm_file_to_image(game->mlx_ptr
		, END_SCREEN, (t_vector2){100, 100});
	if (game->end->end_screen == NULL)
		return (false);
	return (true);
}
