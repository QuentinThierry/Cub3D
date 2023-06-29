/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/29 17:55:37 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
	return (0);
}

void	ft_mouv(t_player *player, float delta_time)
{
	// t_vector2 sign;

	// sign = get_sign(player->angle);
	if (player->dir.y != 0)
	{
		// player->f_pos.x += fabs(sin(player->angle * TO_RADIAN)) * SPEED * delta_time * sign.x;
		// player->f_pos.y += fabs(cos(player->angle * TO_RADIAN)) * SPEED * delta_time * sign.y;
		// // printf("angle : %d",1);
		// // player->f_pos.y += SPEED * delta_time * player->dir.y;
		// player->pos.y = (int)player->f_pos.y;
		// player->pos.x = (int)player->f_pos.x;
		// player->f_real_pos.y = player->f_pos.y / CHUNK_SIZE;
		// player->f_real_pos.x = player->f_pos.x / CHUNK_SIZE;
		player->f_pos.y += SPEED * delta_time * player->dir.y;
		player->pos.y = (int)player->f_pos.y;
		player->f_real_pos.y = player->f_pos.y / CHUNK_SIZE;
	}
	if (player->dir.x != 0)
	{
		// player->f_pos.x += fabs(sin(player->angle * TO_RADIAN)) * SPEED * delta_time * sign.x;
		// player->f_pos.y += fabs(cos(player->angle * TO_RADIAN)) * SPEED * delta_time * sign.y;
		// // printf("angle : %d",1);
		// // player->f_pos.y += SPEED * delta_time * player->dir.y;
		// player->pos.y = (int)player->f_pos.y;
		// player->pos.x = (int)player->f_pos.x;
		// player->f_real_pos.y = player->f_pos.y / CHUNK_SIZE;
		// player->f_real_pos.x = player->f_pos.x / CHUNK_SIZE;
		player->f_pos.x += SPEED * delta_time * player->dir.x;
		player->pos.x = (int)player->f_pos.x;
		player->f_real_pos.x = player->f_pos.x / CHUNK_SIZE;
	}
	if (player->view != 0)
		player->angle += ROTATION * delta_time * player->view;
}

void	ft_close(t_game *game)
{
	if (game->mlx_ptr != NULL)
	{
		mlx_do_key_autorepeaton(game->mlx_ptr);
		if (game->win != NULL)
		{
			if (game->image != NULL)
				mlx_destroy_image(game->mlx_ptr, game->image->img);
			mlx_destroy_window(game->mlx_ptr, game->win);
		}
		mlx_destroy_display(game->mlx_ptr);
	}
	printf("Moyenne fps : %ld\n", (long)tot_fps / nb_fps);
	free(game->image);
	exit(0);
}
