/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/19 18:44:10 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	key_press_hook(int key, t_game *game)
{
	bzero(game->image->addr, WIN_X * WIN_Y * 4);
	(void)game;
	if (key == 65307 ) // esc
		ft_close(game);
	if (key == 65361) // left rrow
		game->player->angle -= 5;
	if (key == 65363) // right arrow
		game->player->angle += 5;
	if (key == 'd')
	{
		game->player->f_pos.x += CHUNK_SIZE / 4.0;
		game->player->pos.x = (int)game->player->f_pos.x;
		game->player->f_real_pos.x += 1 / 4.0;
	}
	if (key == 'a' || key == 'q')
	{
		game->player->f_pos.x -= CHUNK_SIZE / 4.0;
		game->player->pos.x = (int)game->player->f_pos.x;
		game->player->f_real_pos.x -= 1 / 4.0;
	}
	if (key == 'w' || key == 'z')
	{
		game->player->f_pos.y -= CHUNK_SIZE / 4.0;
		game->player->pos.y = (int)game->player->f_pos.y;
		game->player->f_real_pos.y -= 1 / 4.0;
	}
	if (key == 's')
	{
		game->player->f_pos.y += CHUNK_SIZE / 4.0;
		game->player->pos.y = (int)game->player->f_pos.y;
		game->player->f_real_pos.y += 1 / 4.0;
	}
return (0);
}

void	ft_close(t_game *game)
{
	if (game->mlx_ptr != NULL)
	{
		if (game->win != NULL)
		{
			if (game->image != NULL)
				mlx_destroy_image(game->mlx_ptr, game->image->img);
			mlx_destroy_window(game->mlx_ptr, game->win);
		}
		mlx_destroy_display(game->mlx_ptr);
	}
	free(game->image);
	exit(0);
}
