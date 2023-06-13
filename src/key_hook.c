/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/14 00:13:23 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


int	key_press_hook(int key, t_game *game)
{
	bzero(game->image.data, WIN_X * WIN_Y * 4);
	(void)game;
	if (key == 65307 ) // esc
		exit(0);
	if (key == 65361) // left rrow
		game->player.angle -= 5;
	if (key == 65363) // right arrow
		game->player.angle += 5;
	if (key == 'd')
	{
		game->player.pos.x += CHUNK_SIZE / 4.0;
		game->player.f_real_pos.x += 0.25;
	}
	if (key == 'a' || key == 'q')
	{
		game->player.pos.x -= CHUNK_SIZE / 4.0;
		game->player.f_real_pos.x -= 0.25;
	}
	if (key == 'w' || key == 'z')
	{
		game->player.pos.y -= CHUNK_SIZE / 4.0;
		game->player.f_real_pos.y -= 0.25;
	}
	if (key == 's')
	{
		game->player.pos.y += CHUNK_SIZE / 4.0;
		game->player.f_real_pos.y += 0.25;
	}
return (0);
}