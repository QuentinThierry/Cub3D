/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 19:45:31 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	key_press_hook(int key, t_game *game)
{
	if (key == ESC)
		ft_close(game);
	if (key == LEFT_ARROW)
		game->player->view -= 1;
	if (key == RIGHT_ARROW)
		game->player->view += 1;
	if (key == 'd')
		game->player->dir.x += 1;
	if (key == 'a' || key == 'q')
		game->player->dir.x -= 1;
	if (key == 'w' || key == 'z')
		game->player->dir.y -= 1;
	if (key == 's')
		game->player->dir.y += 1;
	return (0);
}

int	key_release_hook(int key, t_player *player)
{
	if (key == LEFT_ARROW)
		player->view += 1;
	if (key == RIGHT_ARROW)
		player->view -= 1;
	if (key == 'd')
		player->dir.x -= 1;
	if (key == 'a' || key == 'q')
		player->dir.x += 1;
	if (key == 'w' || key == 'z')
		player->dir.y += 1;
	if (key == 's')
		player->dir.y -= 1;
	return (0);
}

static t_fvector2	set_move_value(t_player *player)
{
	t_fvector2	move_value;

	move_value.x = 0;
	move_value.y = 0;
	if (player->dir.y != 0)
	{
		move_value.x -= sinf(player->angle * TO_RADIAN) * SPEED
			* player->dir.y;
		move_value.y += cosf(player->angle * TO_RADIAN) * SPEED
			* player->dir.y;
	}
	if (player->dir.x != 0)
	{
		move_value.x += cosf(player->angle * TO_RADIAN) * SPEED
			* player->dir.x;
		move_value.y += sinf(player->angle * TO_RADIAN) * SPEED
			* player->dir.x;
	}
	if (player->dir.x != 0 && player->dir.y != 0)
	{
		move_value.x *= 0.707;
		move_value.y *= 0.707;
	}
	return (move_value);
}

void	player_move(t_player *player)
{
	t_fvector2	move_value;

	if (player->view != 0)
		player->angle += ROTATION * player->view;
	move_value = set_move_value(player);
	if (player->dir.x != 0 || player->dir.y != 0)
	{
		player->f_pos.x += move_value.x;
		player->f_pos.y += move_value.y;
		player->pos.y = (int)player->f_pos.y;
		player->pos.x = (int)player->f_pos.x;
		player->f_real_pos.y = player->f_pos.y / CHUNK_SIZE;
		player->f_real_pos.x = player->f_pos.x / CHUNK_SIZE;
	}
}
