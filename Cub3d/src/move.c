/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 19:11:50 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 17:20:25 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	move_forward(t_player *player)
{
	t_vector2	sign;

	sign = get_sign(player->angle);
	player->f_pos.x += sin(player->angle) * MOUV * sign.x;
	player->pos.x = (int)player->f_pos.x;
	player->f_real_pos.x = player->f_pos.x / CHUNK_SIZE;
	player->f_pos.y += cos(player->angle) * MOUV * sign.y;
	player->pos.y = (int)player->f_pos.y;
	player->f_real_pos.y = player->f_pos.y / CHUNK_SIZE;
}
