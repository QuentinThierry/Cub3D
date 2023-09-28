/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drop_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 16:00:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/28 16:35:08 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_dvector2	find_pos(t_player *player);

static void	_set_object_on_cell(t_map *map_cell, t_player *player, t_dvector2 pos, t_music_game *music_tab)
{
	((t_object *)player->item.arg)->map_pos = pos;
	map_cell->symbol = player->item.symbol;
	map_cell->arg = player->item.arg;
	map_cell->type |= player->item.type;
	if ((player->item.type & MUSIC_OBJECT) == MUSIC_OBJECT)
		((t_object *)map_cell->arg)->music = ((t_object *)player->item.arg)->music;
	if ((player->item.type & IS_PLAYING_MUSIC_OBJECT) == IS_PLAYING_MUSIC_OBJECT)
		update_map_cell_music(map_cell, &player->item, music_tab);
	if ((player->item.type & IS_PLAYING_NARRATOR) == IS_PLAYING_NARRATOR)
		music_tab[1].map_cell = map_cell;
	map_cell->sprite[e_object_interactive_image] = player->item.sprite[e_object_interactive_image];
	map_cell->sprite[e_object_interactive_hand_image] = player->item.sprite[e_object_interactive_hand_image];
	player->has_item = false;
}

static void	_unlock_door(t_game *game, t_map *map_cell, t_player *player, t_music_game *music_tab)
{
	map_cell->type &= ~DOOR_LOCK;
	map_cell->type |= DOOR_UNLOCK;
	map_cell->sprite[e_door_image] = map_cell->sprite[e_door_image + 1];
	((t_door*)map_cell->arg)->is_opening_door = 1;
	((t_door*)map_cell->arg)->time = game->time;
	if ((map_cell->type & MUSIC) == MUSIC)
	{
		map_cell->music = get_music(game->file_music, game->nb_music, map_cell->symbol, e_music_receptacle_complete);
		play_music(map_cell, music_tab, map_cell->music, IS_PLAYING_MUSIC);
		map_cell->type &= ~MUSIC;
	}
	if (((map_cell->type & NARRATOR) == NARRATOR)
		|| (map_cell->type & NARRATOR_RECEPTACLE) == NARRATOR_RECEPTACLE)
	{
		map_cell->narrator = get_narrator(game->file_music, game->nb_music
				, map_cell->symbol, e_narrator_receptacle_complete);
		play_narrator(map_cell, music_tab);
		map_cell->type &= ~NARRATOR & ~NARRATOR_RECEPTACLE;
	}
	player->has_item = false;
}

static void	_complete_receptacle(t_game *game, t_map *map_cell, t_player *player, t_map *exit)
{
	t_door		*exit_door;
	float		frame;
	
	if ((map_cell->type & MUSIC) == MUSIC)
	{
		map_cell->music = get_music(game->file_music, game->nb_music, map_cell->symbol, e_music_receptacle_complete);
		play_music(map_cell, game->music_array, map_cell->music, IS_PLAYING_MUSIC);
		map_cell->type &= ~MUSIC;
	}
	if ((map_cell->type & NARRATOR) == NARRATOR
		|| (map_cell->type & NARRATOR_RECEPTACLE) == NARRATOR_RECEPTACLE)
	{
		map_cell->narrator = get_narrator(game->file_music, game->nb_music
				, map_cell->symbol, e_narrator_receptacle_complete);
		play_narrator(map_cell, game->music_array);
		map_cell->type &= ~NARRATOR & ~NARRATOR_RECEPTACLE;
	}
	((t_object *)map_cell->arg)->is_completed = true;
	map_cell->sprite[e_receptacle_empty_image] = map_cell->sprite[e_receptacle_full_image];
	player->has_item = false;
	if (exit == NULL || exit->arg == NULL)
		return ;
	exit_door = exit->arg;
	exit_door->nb_receptacle_completed++;
	frame = (float)exit_door->nb_receptacle_completed *
			(game->tab_images[exit->sprite[e_door_image].index].nb_total_frame - 1) / game->total_receptacle ;
	frame = roundf(frame);
	if (frame != exit->sprite[e_door_image].frame)
		exit->sprite[e_door_image].frame = frame;
	if (frame == game->tab_images[exit->sprite[e_door_image].index].nb_total_frame - 1)
		exit->type &= ~DOOR_LOCK;
}

void	drop_object(t_player *player, t_map **map, t_map *exit, t_game *game)
{
	t_dvector2	pos;
	
	pos = find_pos(player);
	pos.x = (int)pos.x + 0.5;
	pos.y = (int)pos.y + 0.5;
	if ((map[(int)pos.y][(int)pos.x].type & WALL) != WALL
		&& (map[(int)pos.y][(int)pos.x].type & DOOR) != DOOR
		&& (map[(int)pos.y][(int)pos.x].type & OBJECT) != OBJECT
		&& (map[(int)pos.y][(int)pos.x].type & OBJECT_INTERACTIVE) != OBJECT_INTERACTIVE)
		_set_object_on_cell(&map[(int)pos.y][(int)pos.x], player, pos, game->music_array);
	else if ((map[(int)pos.y][(int)pos.x].type & RECEPTACLE) == RECEPTACLE
		&& (map[(int)pos.y][(int)pos.x].type & DOOR_LOCK) == DOOR_LOCK)
	{
		if (((t_door *)map[(int)pos.y][(int)pos.x].arg)->symbol_unlock_door == player->item.symbol)
			_unlock_door(game, &map[(int)pos.y][(int)pos.x], player, game->music_array);
		else
			play_sound_fail(game, &map[(int)pos.y][(int)pos.x], game->music_array);
	}
	else if ((map[(int)pos.y][(int)pos.x].type & RECEPTACLE) == RECEPTACLE
		&& (map[(int)pos.y][(int)pos.x].type & OBJECT) == OBJECT)
	{
		if (((t_object *)map[(int)pos.y][(int)pos.x].arg)->symbol_receptacle == player->item.symbol
			&& ((t_object *)map[(int)pos.y][(int)pos.x].arg)->is_completed == false)
			_complete_receptacle(game, &map[(int)pos.y][(int)pos.x], player, exit);
		else
			play_sound_fail(game, &map[(int)pos.y][(int)pos.x], game->music_array);
	}
}
