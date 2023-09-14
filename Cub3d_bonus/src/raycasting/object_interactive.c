/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_interactive.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:54:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/14 23:12:34 by jvigny           ###   ########.fr       */
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

void	take_object_click(t_game *game, t_player *player, t_map **map)
{
	t_dvector2	pos;
	t_vector2	dir;
	
	pos.x = sin(player->angle * TO_RADIAN) * M_SQRT2;
	pos.y = -cos(player->angle * TO_RADIAN) * M_SQRT2;
	if (pos.x > 1)
		pos.x = 1;
	if (pos.x < -1)
		pos.x = -1;
	if (pos.y > 1)
		pos.y = 1;
	if (pos.y < -1)
		pos.y = -1;
	pos.x += player->f_real_pos.x;
	pos.y += player->f_real_pos.y;
	if ((map[(int)pos.y][(int)pos.x].type & WALL) == WALL
		&& (map[(int)pos.y][(int)pos.x].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE
		&& (map[(int)pos.y][(int)pos.x].type & OBJECT) == OBJECT)
	{
		player->item.symbol = map[(int)pos.y][(int)pos.x].symbol;
		player->item.type = map[(int)pos.y][(int)pos.x].type;
		player->item.type ^= WALL;
		player->item.sprite[e_object_interactive_image] = map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_image];
		player->item.sprite[e_object_interactive_hand_image] = map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_hand_image];
		player->item.arg = find_empty_object(game);
		((t_object *)player->item.arg)->map_pos = pos;
		player->has_item = true;
		map[(int)pos.y][(int)pos.x].type ^= OBJECT_INTERACTIVE;
		map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_image] = map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_after_image];
	}
}

void	drop_object(t_player *player, t_map **map)
{
	t_dvector2	pos;
	t_vector2	dir;
	
	if (player->has_item == true)
	{
		pos.x = sin(player->angle * TO_RADIAN) * M_SQRT2;
		pos.y = -cos(player->angle * TO_RADIAN) * M_SQRT2;
		if (pos.x > 1)
			pos.x = 1;
		if (pos.x < -1)
			pos.x = -1;
		if (pos.y > 1)
			pos.y = 1;
		if (pos.y < -1)
			pos.y = -1;
		pos.x += player->f_real_pos.x;
		pos.y += player->f_real_pos.y;
		pos.x = (int)pos.x + 0.5;
		pos.y = (int)pos.y + 0.5;
		if ((map[(int)pos.y][(int)pos.x].type & WALL) != WALL
			&& (map[(int)pos.y][(int)pos.x].type & DOOR) != DOOR
			&& (map[(int)pos.y][(int)pos.x].type & OBJECT) != OBJECT
			&& (map[(int)pos.y][(int)pos.x].type & OBJECT_INTERACTIVE) != OBJECT_INTERACTIVE)
		{
			((t_object *)player->item.arg)->map_pos = pos;
			map[(int)pos.y][(int)pos.x].symbol = player->item.symbol;
			map[(int)pos.y][(int)pos.x].arg = player->item.arg;
			map[(int)pos.y][(int)pos.x].type = player->item.type;
			map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_image] = player->item.sprite[e_object_interactive_image];
			map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_hand_image] = player->item.sprite[e_object_interactive_hand_image];
			printf("drop\n");
			player->has_item = false;
		}
		else if ((map[(int)pos.y][(int)pos.x].type & RECEPTACLE) == RECEPTACLE
			&& ((t_object *)map[(int)pos.y][(int)pos.x].arg)->symbol_receptacle == player->item.symbol)
		{
			((t_object *)map[(int)pos.y][(int)pos.x].arg)->is_completed = true;
			player->has_item = false;
			printf("okayyyy\n");
		}
	}
}