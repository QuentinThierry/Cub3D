/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/13 18:28:37 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


int	key_press_hook(int key, t_game *game)
{
	bzero(game->image.data, WIN_X * WIN_Y * 4);
	(void)game;
	if (key == 65307 ) // esc
		exit(0);
	if (key == 65362) // up arrow
		game->player.angle -= 1;
	if (key == 65364) // down arrow
		game->player.angle += 1;
	if (key == 'd')
		game->player.pos.x++;
	if (key == 'a' || key == 'q')
		game->player.pos.x--;
	if (key == 'w' || key == 'z')
		game->player.pos.y--;
	if (key == 's')
		game->player.pos.y++;
return (0);
}