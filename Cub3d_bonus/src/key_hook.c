/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/14 20:46:01 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	key_press_hook(int key, t_game *game)
{
	if (key == 65307 ) // esc
		ft_close(game);
	if (key == 65361) // left rrow
		game->player->view -= 1;
	if (key == 65363) // right arrow
		game->player->view += 1;
	if (key == 'd')
		game->player->dir.x += 1;
	if (key == 'a' || key == 'q')
		game->player->dir.x -= 1;
	if (key == 'w' || key == 'z')
		game->player->dir.y -= 1;
	if (key == 's')
		game->player->dir.y += 1;
	if (key == 65505) // shift
		game->player->speed += SPRINT_BOOST;
	return (0);
}

int	key_release_hook(int key, t_player *player)
{
	if (key == 65361) // left rrow
		player->view += 1;
	if (key == 65363) // right arrow
		player->view -= 1;
	if (key == 'd')
		player->dir.x -= 1;
	if (key == 'a' || key == 'q')
		player->dir.x += 1;
	if (key == 'w' || key == 'z')
		player->dir.y += 1;
	if (key == 's')
		player->dir.y -= 1;
	if (key == 65505) // shift
		player->speed -= SPRINT_BOOST;
	return (0);
}

void	player_move(t_player *player, double delta_time, char **map, t_vector2 map_size)
{
	t_fvector2 move_value;

	if (player->view != 0)
		player->angle += ROTATION_KEYBOARD * delta_time * player->view;
	move_value.x = 0;
	move_value.y = 0;
	if (player->dir.y != 0)
	{
		move_value.x -= sinf(player->angle * TO_RADIAN) * player->speed * player->dir.y;
		move_value.y += cosf(player->angle * TO_RADIAN) * player->speed * player->dir.y;
	}
	if (player->dir.x != 0)
	{
		move_value.x += cosf(player->angle * TO_RADIAN) * player->speed * player->dir.x;
		move_value.y += sinf(player->angle * TO_RADIAN) * player->speed * player->dir.x;
	}
	if (player->dir.x != 0 || player->dir.y != 0)
	{
		move_value.x *= 0.707;
		move_value.y *= 0.707;
	}
	if (player->dir.x != 0 || player->dir.y != 0)
	{
		move_value.x = player->f_pos.x + move_value.x * delta_time;
		move_value.y = player->f_pos.y + move_value.y * delta_time;
		check_colliding(player, move_value, map);
	}
}

// 		player->f_pos.x += move_value.x * delta_time;
// 		player->f_pos.y += move_value.y * delta_time;
// 		player->pos.y = (int)player->f_pos.y;
// 		player->pos.x = (int)player->f_pos.x;
// 		player->f_real_pos.y = player->f_pos.y / CHUNK_SIZE;
// 		player->f_real_pos.x = player->f_pos.x / CHUNK_SIZE;


int mouse_hook(int x,int y, t_game *game)
{
	game->player->angle -= (double)(game->player->mouse_pos.x - x) / ROTATION_MOUSE;
	// if (x >= WIN_X || x <= 0)
	// {
		// mlx_mouse_move(game->mlx_ptr, game->win, WIN_X/2.0, WIN_Y/2.0);
	// 	game->player->mouse_pos.x = WIN_X / 2.0;
	// 	game->player->mouse_pos.y = WIN_Y / 2.0;
	// }
	// else if (y >= WIN_Y || y <= 0)
	// {
	// 	mlx_mouse_move(game->mlx_ptr, game->win, WIN_X/2.0, WIN_Y/2.0);
	// 	game->player->mouse_pos.x = WIN_X / 2.0;
	// 	game->player->mouse_pos.y = WIN_Y / 2.0;
	// }
	// else
	// {
		game->player->mouse_pos.x = x;
		game->player->mouse_pos.y = y;
	// }
	return (0);
}