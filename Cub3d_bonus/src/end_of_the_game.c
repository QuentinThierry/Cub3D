/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_of_the_game.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 18:22:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/18 13:10:42 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	move_to_dest(t_player *player, t_end *end, const double delta_time)
{
	player->f_real_pos.x += end->dir.x * SPEED * delta_time;
	player->f_real_pos.y += end->dir.y * SPEED * delta_time;
	player->angle += end->dir_angle * delta_time;
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
		end->is_moving = false;
}

int	update_end(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	struct timespec			time;
	long					fps;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	clock_gettime(CLOCK_REALTIME, &time);
	game->time = time_to_long(&time);
	
	if (game->end->is_moving)
		move_to_dest(game->player, game->end, game->delta_time);
	if (game->player->angle + game->player->angle >= 360)
		game->player->angle = game->player->angle - 360;
	if (game->player->angle + game->player->angle < 0)
		game->player->angle = game->player->angle + 360;
	// update_doors(game->door_array, game->nb_doors, game->time, game->map);
	raycasting(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);

	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	// fps = (long)(1.0 / game->delta_time);
	// tot_fps += fps;
	// nb_fps++;
	// if ((nb_fps % 50) == 0)
	// 	printf("fps : %ld\n", fps);
	last_time = cur_time;
	return (0);
}

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
	
	game->end->is_moving = true;
	mlx_loop_hook(game->mlx_ptr, update_end, game);
}