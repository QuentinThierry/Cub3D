/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:10:31 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/05 14:30:11 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	raycasting_end(t_game *game);
void	find_dest(t_end *end, const enum e_orientation orient
		, const t_vector2 pos_exit, const t_player * player);
void	next_dest(t_end *end, const t_dvector2 player_pos);

bool	init_end_screen(t_game *game)
{
	game->end = ft_calloc(1, sizeof(t_end));
	if (game->end == NULL)
		return (false);
	game->end->status = -1;
	game->end->end_screen = btmlx_xpm_file_to_image(game->mlx_ptr
		, END_SCREEN, (t_vector2){1, 1});
	if (game->end->end_screen == NULL)
		return (false);
	return (true);
}

/**
 * @brief init the process of open the exit door and open it until it's totally
 * 			open
 * 
 * @param game 
 * @param door 
 * @param end 
 */
static void	open_exit(t_game *game, t_door *door, t_end *end)
{
	if (door->is_opening_door == 0)
	{
		door->is_opening_door = 1;
		door->time = game->time;
	}
	end_step_door_open(game->delta_time, game->exit, game->map, end);
	if (door->door_percent == 90)
		end->status = e_walk_through_door;
	printf("door : %f %d\n", door->door_percent, door->is_opening_door);
}

/**
 * @brief move the player to end->dest and turn his head at the same time
 * 
 * @param player 
 * @param end 
 * @param delta_time 
 * @return true 
 * @return false 
 */
static bool	move_to_dest(t_player *player, const t_end *end, const double delta_time)
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
 * @brief loop for the end's screen
 * 
 * @param game 
 * @return int 
 */
int	update_loop(t_game *game)
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
	mlx_loop_hook(game->mlx_ptr, update_loop, game);
}
