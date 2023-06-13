/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/13 17:26:25 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


int	key_press_hook(int key, t_game *game)
{
	(void)game;
	if (key == 65307 ) // esc
		exit(0);
	if (key == 65362) // up arrow
		game->player.angle -= 1;
	if (key == 65364) // down arrow
		game->player.angle += 1;
	return (0);
}