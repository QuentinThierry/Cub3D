/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 19:11:50 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/10 00:08:20 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	move_forward(t_player *player)
{
	t_vector2 sign;
	
	sign = get_sign(player->angle);
	player->f_pos.x += sin(player->angle) * MOUV * sign.x;
	player->pos.x = (int)player->f_pos.x;
	player->f_real_pos.x = player->f_pos.x / CHUNK_SIZE;
	// printf("player Y	pixel_f : %f pixel :%d	f : %f\n",
	// 	player->f_pos.y,player->pos.y, player->f_real_pos.y);
	player->f_pos.y += cos(player->angle) * MOUV * sign.y;
	player->pos.y = (int)player->f_pos.y;
	player->f_real_pos.y = player->f_pos.y / CHUNK_SIZE;
	// printf("player X	pixel_f : %f pixel :%d	f : %f\n",
	// 	player->f_pos.x,player->pos.x, player->f_real_pos.x);
}
