/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 01:05:58 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/10 01:59:27 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	check_colliding(t_player *player, t_fvector2 new_pos, char **map)
{
	if (fabs(player->f_pos.x - new_pos.x) > CHUNK_SIZE)
		;
	if (fabs(player->f_pos.y - new_pos.y) > CHUNK_SIZE)
		;

	if (map[(int)(player->f_pos.y / CHUNK_SIZE)][(int)(new_pos.x / CHUNK_SIZE)] == '1')
		player->f_pos.x = ((int)(new_pos.x / CHUNK_SIZE)+ player->dir.y == -1) * CHUNK_SIZE+ player->dir.x;
	else
		player->f_pos.x = new_pos.x;
	if (map[(int)(new_pos.y / CHUNK_SIZE)][(int)(player->f_pos.x / CHUNK_SIZE)] == '1')
		player->f_pos.y = ((int)(new_pos.y / CHUNK_SIZE) + player->dir.y == -1) * CHUNK_SIZE+ player->dir.y;
	else
		player->f_pos.y = new_pos.y;

	player->pos.x = (int)player->f_pos.x;
	player->f_real_pos.x = player->f_pos.x / CHUNK_SIZE;
	player->pos.y = (int)player->f_pos.y;
	player->f_real_pos.y = player->f_pos.y / CHUNK_SIZE;

}

// void	check_colliding(t_player *player, t_fvector2 new_pos, char **map)
// {
// 	t_fvector2 tmp;
// 	int			i = 0;

// 	tmp.x = new_pos.x / CHUNK_SIZE;
// 	tmp.y = new_pos.y / CHUNK_SIZE;
// 	if (fabs(player->f_real_pos.x - tmp.x) > 1)
// 		;
// 	if (fabs(player->f_real_pos.y - tmp.y) > 1)
// 		;
	
// 	if (map[(int)player->f_real_pos.y][(int)tmp.x] == '1')
// 	{
// 		player->f_real_pos.x = (int)tmp.x;
// 		player->f_pos.x = player->f_real_pos.x * CHUNK_SIZE;
// 		player->pos.x = player->f_pos.x;
// 		i++;
// 	}
// 	if (map[(int)tmp.y][(int)player->f_real_pos.x] == '1')
// 	{
// 		player->f_real_pos.y = (int)tmp.y;
// 		player->f_pos.y = player->f_real_pos.y * CHUNK_SIZE;
// 		player->pos.y = player->f_pos.y;
// 		i++;
// 	}
// 	if (i == 0)
// 	{
// 		player->f_real_pos.y = tmp.y;
// 		player->f_real_pos.x = tmp.x;
// 		player->f_pos.x = new_pos.x;
// 		player->pos.x = (int)player->f_pos.x;
// 		player->f_pos.y = new_pos.y;
// 		player->pos.y = (int)player->f_pos.y;
// 	}
// }