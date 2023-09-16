/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_of_the_game.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 18:22:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/16 20:01:31 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	move_to_dest(t_player *player, t_end *end, double delta_time)
{
	t_dvector2	dir;
	float		dir_angle;

	if (end->is_moving)
	{
		dir_angle = end->dest_angle - player->angle;
		// if (dir_angle > 180)
		// 	dir_angle = dir_angle - 360;
		dir.x = end->dest.x - player->f_real_pos.x;
		dir.y = end->dest.y - player->f_real_pos.y;
		printf("dir : %f %f angle :%f\n", dir.x, dir.y, dir_angle);
		player->f_real_pos.x += dir.x * SPEED * delta_time;
		player->f_real_pos.y += dir.y * SPEED * delta_time;
		// player->angle += dir_angle * delta_time;
		if (player->f_real_pos.x == end->dest.x && player->f_real_pos.y == end->dest.y
			&& player->angle == end->dest_angle)
			end->is_moving = false;
	}
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
	// usleep(1000000);
	printf("player : %f %f angle : %f\n", game->player->f_real_pos.x, game->player->f_real_pos.y, game->player->angle);
	move_to_dest(game->player, game->end, game->delta_time);
	printf("player : %f %f angle : %f\n\n\n", game->player->f_real_pos.x, game->player->f_real_pos.y, game->player->angle);
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

void	end_of_the_game(t_game *game, enum e_orientation orient)
{
	t_vector2 pos_door;

	pos_door = ((t_door *)game->exit->arg)->map_pos;
	if (orient == e_north)
	{
		game->end->dest = (t_fvector2){pos_door.x + .5, pos_door.y - .5};
		game->end->dest_angle = 180;
	}
	else if (orient == e_south)
	{
		game->end->dest = (t_fvector2){pos_door.x + .5, pos_door.y + 1.5};
		game->end->dest_angle = 0;
	}
	else if (orient == e_west)
	{
		game->end->dest = (t_fvector2){pos_door.x + 1.5, pos_door.y + .5};
		game->end->dest_angle = 270;
	}
	else
	{
		game->end->dest = (t_fvector2){pos_door.x - .5, pos_door.y + .5};
		game->end->dest_angle = 90;
	}
	printf("exit : %d %d	orient : %d\n", pos_door.x, pos_door.y, orient);
	printf("dest : %f %f	angle :%d\n", game->end->dest.x, game->end->dest.y, game->end->dest_angle);
	game->end->is_moving = true;
	mlx_loop_hook(game->mlx_ptr, update_end, game);
}