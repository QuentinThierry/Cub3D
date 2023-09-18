/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_of_the_game.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 18:22:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/18 19:32:52 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	raycasting_end(t_game *game);

bool	move_to_dest(t_player *player, t_end *end, const double delta_time)
{
	player->f_real_pos.x += end->dir.x * SPEED / 2. * delta_time;
	player->f_real_pos.y += end->dir.y * SPEED / 2. * delta_time;
	player->angle += end->dir_angle / 2. * delta_time;
	if ((end->dir.x < 0 && player->f_real_pos.x < end->dest.x)
		|| (end->dir.x > 0 && player->f_real_pos.x > end->dest.x))
		player->f_real_pos.x = end->dest.x;
	if ((end->dir.y < 0 && player->f_real_pos.y < end->dest.y)
		|| (end->dir.y > 0 && player->f_real_pos.y > end->dest.y))
		player->f_real_pos.y = end->dest.y;
	if ((end->dir_angle < 0 && player->angle < end->dest_angle)
		|| (end->dir_angle > 0 && player->angle > end->dest_angle))
		player->angle = end->dest_angle;
	if (player->f_real_pos.x == end->dest.x && player->f_real_pos.y == end->dest.y
		&& player->angle == end->dest_angle)
		return (true);
	return (false);
}
void	open_exit(t_game *game, t_door *door, t_end *end)
{
	if (door->is_opening_door == 0)
	{
		door->is_opening_door = 1;
		door->time = game->time;
	}
	door->time += (game->time - door->time) / 2.;
	step_door_open(door, game->time, game->exit, game->map);
	if (door->door_percent == 90)
	{
		end->status = e_walk_through_door;
		if (end->orient == e_south)
			end->dest.y -= 1.5;
		else if (end->orient == e_north)
			end->dest.y += 1.5;
		else if (end->orient == e_east)
			end->dest.x -= 1.5;
		else
			end->dest.x += 1.5;
		end->dir.x = end->dest.x - game->player->f_real_pos.x;
		end->dir.y = end->dest.y - game->player->f_real_pos.y;
		end->dir_angle = 0;
	}
}

int	update_end(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	struct timespec			time;
	// long					fps;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	clock_gettime(CLOCK_REALTIME, &time);
	game->time = time_to_long(&time);
	
	if (game->end->status == e_go_in_font_of_door)
	{
		if (move_to_dest(game->player, game->end, game->delta_time))
			game->end->status = e_open_door;
	}
	else if (game->end->status == e_open_door)
		open_exit(game, game->exit->arg, game->end);
	else if (game->end->status == e_walk_through_door)
	{
		// if (move_to_dest(game->player, game->end, game->delta_time))
		// 	game->end->status = e_end;
	}
	raycasting_end(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);

	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	last_time = cur_time;
	return (0);
}
	// fps = (long)(1.0 / game->delta_time);
	// tot_fps += fps;
	// nb_fps++;
	// if ((nb_fps % 50) == 0)
	// 	printf("end fps : %ld\n", fps);

void	find_dest(t_end *end, const enum e_orientation orient
		, const t_vector2 pos_exit, const t_player * const player)
{
	if (orient == e_north)
	{
		end->dest = (t_fvector2){pos_exit.x + .5, pos_exit.y - .5};
		end->dest_angle = 180;
	}
	else if (orient == e_south)
	{
		end->dest = (t_fvector2){pos_exit.x + .5, pos_exit.y + 1.5};
		end->dest_angle = 360;
	}
	else if (orient == e_east)
	{
		end->dest = (t_fvector2){pos_exit.x + 1.5, pos_exit.y + .5};
		end->dest_angle = 270;
	}
	else
	{
		end->dest = (t_fvector2){pos_exit.x - .5, pos_exit.y + .5};
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

void	end_of_the_game(t_game *game, enum e_orientation orient)
{
	find_dest(game->end, orient, ((t_door *)game->exit->arg)->map_pos, game->player);
	game->end->orient = orient;
	game->end->status = e_go_in_font_of_door;
	mlx_hook(game->win, 3, 0L, NULL, game);
	mlx_hook(game->win, 6, (1L << 6), mouse_stay_in_window_hook, game);
	mlx_hook(game->win, 4, 0L, NULL, game);
	mlx_hook(game->win, 2, (1L << 0), exit_hook, &game);
	mlx_loop_hook(game->mlx_ptr, update_end, game);
}

__attribute__((always_inline))
static inline float	get_dist(t_dvector2 fpos, t_dvector2 wall)
{
	return (sqrtf((wall.x - fpos.x) * (wall.x - fpos.x) + (wall.y - fpos.y) * (wall.y - fpos.y)));
}

void	raycasting_end(t_game *game)
{
	int			x;
	double		height;
	float		angle;
	t_ray		ray;
	t_dvector2	fpos;
	float		dist; 
	
	draw_ceiling(game);
	fpos = game->player->f_real_pos;
	x = -WIN_X / 2;
	while (x < WIN_X / 2)
	{
		angle = atanf(x / game->constants[0]) * 180 / M_PI;
		if (game->player->angle + angle >= 360)
			angle = angle - 360;
		if (game->player->angle + angle < 0)
			angle = angle + 360;
		ray = get_wall_hit_end(fpos, game->map, game->player->angle + angle, game->end->status);
		dist = get_dist(game->player->f_real_pos, ray.hit);
		game->dist_tab[x + WIN_X / 2] = dist;
		dist *= cosf(angle * TO_RADIAN);
		if (dist == 0)
			dist = 0.01;
		height = 1 / dist * game->constants[0];
		draw_vert(game, x + WIN_X / 2, ray, height);
		x++;
	}
}

bool	init_end_screen(t_game *game)
{
	game->end = ft_calloc(1, sizeof(t_end));
	if (game->end == NULL)
		return (false);
	game->end->end_screen = btmlx_xpm_file_to_image(game->mlx_ptr
		, "./assets/end.xpm", (t_vector2){100, 100});
	if (game->end->end_screen == NULL)
		return (false);
	return (true);
}
