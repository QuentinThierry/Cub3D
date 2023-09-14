/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_interactive.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:54:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/14 18:52:25 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	take_object(t_player *player, t_map *cell_map)
{
	if (player->has_item == false && (cell_map->type & WALL) != WALL)
	{
		player->item = *cell_map;
		cell_map->symbol = '0';
		cell_map->arg = NULL;
		cell_map->type ^= OBJECT_INTERACTIVE;
		cell_map->type ^= OBJECT;
		player->has_item = true;
		// printf("has item : %c %d %p\n", player->item.symbol, player->item.type, player->item.arg);
		// printf("map : %c %d %p\n", cell_map->symbol, cell_map->type, cell_map->arg);
	}
}

void	drop_object(t_player *player, t_map **map)
{
	t_dvector2	pos;
	t_vector2	dir;
	
	if (player->has_item == true)
	{
		pos.x = sin(player->angle * TO_RADIAN) * sqrt(2.);
		pos.y = -cos(player->angle * TO_RADIAN) * sqrt(2.);
		pos.x += player->f_real_pos.x;
		pos.y += player->f_real_pos.y;
		pos.x = (int)pos.x + 0.5;
		pos.y = (int)pos.y + 0.5;
		if ((map[(int)pos.y][(int)pos.x].type & WALL) != WALL
			&& (map[(int)pos.y][(int)pos.x].type & DOOR_CLOSE) != DOOR_CLOSE
			&& (map[(int)pos.y][(int)pos.x].type & OBJECT) != OBJECT
			&& (map[(int)pos.y][(int)pos.x].type & OBJECT_INTERACTIVE) != OBJECT_INTERACTIVE)
		{
			((t_object *)player->item.arg)->map_pos = pos;
			map[(int)pos.y][(int)pos.x].symbol = player->item.symbol;
			map[(int)pos.y][(int)pos.x].arg = player->item.arg;
			map[(int)pos.y][(int)pos.x].type = player->item.type;
			map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_image] = player->item.sprite[e_object_interactive_image];
			map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_hand_image] = player->item.sprite[e_object_interactive_hand_image];
			player->has_item = false;
		}
		else if ((map[(int)pos.y][(int)pos.x].type & RECEPTACLE) == RECEPTACLE
			&& ((t_object *)map[(int)pos.y][(int)pos.x].arg)->symbol_receptacle == player->item.symbol)
		{
			((t_object *)player->item.arg)->is_full = true;
			player->has_item = false;
			printf("okayyyy\n");
		}
	}
}