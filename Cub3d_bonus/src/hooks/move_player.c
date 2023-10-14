/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 17:17:55 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 17:19:39 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static t_dvector2	_get_move_value(t_player *player, double delta_time)
{
	t_dvector2	move;

	move = (t_dvector2){0};
	if (player->dir.y != 0)
	{
		move.x -= sinf(player->angle * TO_RADIAN) * player->speed
			* player->dir.y;
		move.y += cosf(player->angle * TO_RADIAN) * player->speed
			* player->dir.y;
	}
	if (player->dir.x != 0)
	{
		move.x += cosf(player->angle * TO_RADIAN) * player->speed
			* player->dir.x;
		move.y += sinf(player->angle * TO_RADIAN) * player->speed
			* player->dir.x;
	}
	if (player->dir.x != 0 && player->dir.y != 0)
	{
		move.x *= 0.707;
		move.y *= 0.707;
	}
	move.x = player->f_pos.x + move.x * delta_time;
	move.y = player->f_pos.y + move.y * delta_time;
	return (move);
}

void	player_move(t_game *game, t_player *player, double delta_time,
		t_map **map)
{
	t_dvector2	move;

	if (player->view != 0)
		player->angle += ROTATION_KEYBOARD * delta_time * player->view;
	move = _get_move_value(player, delta_time);
	if (player->dir.x != 0 || player->dir.y != 0)
	{
		move = check_colliding(game, move, map);
		if (((int)move.x != (int)player->f_pos.x
				|| (int)move.y != (int)player->f_pos.y)
			&& (map[(int)move.y][(int)move.x].type & MUSIC) == MUSIC)
			play_music(&map[(int)move.y][(int)move.x],
				game->music_array, map[(int)move.y][(int)move.x].music,
				IS_PLAYING_MUSIC);
		if (((int)move.x != (int)player->f_pos.x
				|| (int)move.y != (int)player->f_pos.y)
			&& (map[(int)move.y][(int)move.x].type & NARRATOR) == NARRATOR)
		{
			play_narrator(game, &map[(int)move.y][(int)move.x],
				game->music_array);
			map[(int)move.y][(int)move.x].type ^= NARRATOR;
		}
		game->player->f_pos = move;
	}
}
