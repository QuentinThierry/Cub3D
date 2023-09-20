/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_interactive.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:54:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/20 19:23:53 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	take_object(t_player *player, t_map *cell_map)
{
	if (player->has_item == false && (cell_map->type & WALL) != WALL)
	{
		((t_object *)cell_map->arg)->map_pos = (t_dvector2){-1, -1};
		player->item = *cell_map;
		cell_map->symbol = '0';
		cell_map->arg = NULL;
		cell_map->type ^= OBJECT_INTERACTIVE;
		cell_map->type ^= OBJECT;
		player->has_item = true;
	}
}

void	take_object_click(t_game *game, t_player *player, t_map **map)
{
	t_dvector2	pos;
	
	pos.x = sin(player->angle * TO_RADIAN) * M_SQRT2;
	pos.y = -cos(player->angle * TO_RADIAN) * M_SQRT2;
	if (pos.x > 1)
		pos.x = 1;
	else if (pos.x < -1)
		pos.x = -1;
	if (pos.y > 1)
		pos.y = 1;
	else if (pos.y < -1)
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

void	drop_object(t_player *player, t_map **map, t_map *exit, t_game *game)
{
	t_dvector2	pos;
	t_door		*door;
	float		frame;
	
	if (player->has_item == true)
	{
		pos.x = sin(player->angle * TO_RADIAN) * M_SQRT2;
		pos.y = -cos(player->angle * TO_RADIAN) * M_SQRT2;
		if (pos.x > 1)
			pos.x = 1;
		else if (pos.x < -1)
			pos.x = -1;
		if (pos.y > 1)
			pos.y = 1;
		else if (pos.y < -1)
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
			player->has_item = false;
		}
		else if ((map[(int)pos.y][(int)pos.x].type & RECEPTACLE) == RECEPTACLE
			&& ((t_object *)map[(int)pos.y][(int)pos.x].arg)->symbol_receptacle == player->item.symbol
			&& ((t_object *)map[(int)pos.y][(int)pos.x].arg)->is_completed != true)
		{
			((t_object *)map[(int)pos.y][(int)pos.x].arg)->is_completed = true;
			map[(int)pos.y][(int)pos.x].sprite[e_receptacle_empty_image] = map[(int)pos.y][(int)pos.x].sprite[e_receptacle_full_image];
			
			player->has_item = false;
			if (exit != NULL && exit->arg != NULL)
			{
				door = exit->arg;
				door->nb_receptacle_completed++;
				frame  = (float)door->nb_receptacle_completed * (game->tab_images[exit->sprite[e_door_image].index].nb_total_frame - 1) / game->total_receptacle ;
				frame = roundf(frame);
				if (frame != exit->sprite[e_door_image].frame)
					exit->sprite[e_door_image].frame = frame;
				if (frame == game->tab_images[exit->sprite[e_door_image].index].nb_total_frame - 1)
					exit->type ^= DOOR_LOCK;
			}
		}
	}
}

#define SIZE_OBJECT_HAND 3

void	draw_hand_item(t_game *game, t_player *player)
{
	t_image	*image;
	int		begin;
	
	image = &game->tab_images[player->item.sprite[e_object_interactive_hand_image].index];
	begin = (WIN_Y - image->size.y) * game->image->size_line + (WIN_X - image->size.x) * 4;
	draw_image_with_transparence(game->image->addr + begin, image, (t_vector2){0}, image->size);
}
