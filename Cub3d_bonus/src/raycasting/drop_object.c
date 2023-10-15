/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drop_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 16:00:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 18:58:17 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_vector2	find_pos(t_player *player);

static void	_set_object_on_cell(t_map *map_cell, t_player *player,
			t_vector2 pos, t_music_game *music_tab)
{
	((t_object *)player->item.arg)->map_pos = (t_dvector2){pos.x + 0.5,
		pos.y + 0.5};
	map_cell->symbol = player->item.symbol;
	map_cell->arg = player->item.arg;
	map_cell->type |= player->item.type;
	if ((player->item.type & MUSIC_OBJECT) == MUSIC_OBJECT)
		((t_object *)map_cell->arg)->music
			= ((t_object *)player->item.arg)->music;
	if ((player->item.type & IS_PLAYING_OBJECT) == IS_PLAYING_OBJECT)
		update_map_cell_music(map_cell, &player->item, music_tab);
	map_cell->sprite[e_obj_int_img]
		= player->item.sprite[e_obj_int_img];
	map_cell->sprite[e_obj_int_hand_img]
		= player->item.sprite[e_obj_int_hand_img];
	player->has_item = false;
}

static void	_unlock_door(t_game *game, t_map *map_cell,
			t_player *player, t_music_game *music_tab)
{
	map_cell->type &= ~DOOR_LOCK & ~RECEPTACLE;
	map_cell->type |= DOOR_UNLOCK;
	map_cell->sprite[e_door_img] = map_cell->sprite[e_door_img + 1];
	((t_door *)map_cell->arg)->is_opening_door = 1;
	if ((map_cell->type & MUSIC) == MUSIC)
	{
		map_cell->music = get_music(game->file_music,
				game->nb_music, map_cell->symbol, e_music_receptacle_complete);
		play_music(map_cell, music_tab, map_cell->music, IS_PLAYING_MUSIC);
		map_cell->type &= ~MUSIC;
	}
	if (((map_cell->type & NARRATOR) == NARRATOR)
		|| (map_cell->type & NARRATOR_RECEPTACLE) == NARRATOR_RECEPTACLE)
	{
		map_cell->narrator = get_narrator(game->file_music, game->nb_music,
				map_cell->symbol, e_narrator_receptacle_complete);
		play_narrator(game, map_cell, music_tab);
		map_cell->type &= ~NARRATOR & ~NARRATOR_RECEPTACLE;
	}
	player->has_item = false;
}

static void	_play_sound_receptacle(t_game *game, t_map *map_cell)
{
	if ((map_cell->type & MUSIC) == MUSIC)
	{
		map_cell->music = get_music(game->file_music, game->nb_music,
				map_cell->symbol, e_music_receptacle_complete);
		play_music(map_cell, game->music_array, map_cell->music,
			IS_PLAYING_MUSIC);
		map_cell->type &= ~MUSIC;
	}
	if ((map_cell->type & NARRATOR) == NARRATOR
		|| (map_cell->type & NARRATOR_RECEPTACLE) == NARRATOR_RECEPTACLE)
	{
		map_cell->narrator = get_narrator(game->file_music, game->nb_music,
				map_cell->symbol, e_narrator_receptacle_complete);
		play_narrator(game, map_cell, game->music_array);
		map_cell->type &= ~NARRATOR & ~NARRATOR_RECEPTACLE;
	}
}

static void	_complete_receptacle(t_game *game, t_map *map_cell,
			t_player *player, t_map *exit)
{
	t_door		*exit_door;
	float		frame;

	_play_sound_receptacle(game, map_cell);
	((t_object *)map_cell->arg)->is_completed = true;
	map_cell->sprite[e_receptacle_empty_img]
		= map_cell->sprite[e_receptacle_full_img];
	player->has_item = false;
	if (exit == NULL || exit->arg == NULL)
		return ;
	exit_door = exit->arg;
	exit_door->nb_receptacle_completed++;
	frame = (float)exit_door->nb_receptacle_completed
		* (game->tab_images[exit->sprite[e_door_img].index].nb_total_frame
			- 1) / game->total_receptacle ;
	frame = roundf(frame);
	if (frame
		== game->tab_images[exit->sprite[e_door_img].index].nb_total_frame - 1
		&& exit_door->nb_receptacle_completed != game->total_receptacle)
		return ;
	if (frame != exit->sprite[e_door_img].frame)
		exit->sprite[e_door_img].frame = frame;
	if (exit_door->nb_receptacle_completed == game->total_receptacle)
		exit->type &= ~DOOR_LOCK;
}

void	drop_object(t_player *player, t_map **map, t_map *exit, t_game *game)
{
	t_vector2	pos;
	t_type		type;

	pos = find_pos(player);
	type = map[pos.y][pos.x].type;
	if ((type & WALL) != WALL && (type & DOOR) != DOOR
		&& (type & OBJECT) != OBJECT
		&& (type & OBJ_INTER) != OBJ_INTER)
		_set_object_on_cell(&map[pos.y][pos.x], player, pos, game->music_array);
	if ((type & RECEPTACLE) == RECEPTACLE && (type & DOOR_LOCK) == DOOR_LOCK)
	{
		if (((t_door *)map[pos.y][pos.x].arg)->open_door == player->item.symbol)
			_unlock_door(game, &map[pos.y][pos.x], player, game->music_array);
		else
			play_sound_fail(game, &map[pos.y][pos.x], game->music_array);
	}
	else if ((type & RECEPTACLE) == RECEPTACLE && (type & OBJECT) == OBJECT)
	{
		if (((t_object *)map[pos.y][pos.x].arg)->sym_rcp == player->item.symbol
			&& ((t_object *)map[pos.y][pos.x].arg)->is_completed == false)
			_complete_receptacle(game, &map[pos.y][pos.x], player, exit);
		else
			play_sound_fail(game, &map[pos.y][pos.x], game->music_array);
	}
}
