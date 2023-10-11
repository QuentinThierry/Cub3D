/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_interactive.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:54:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 21:42:21 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"
#include "../../includes/audio.h"

void	take_object(t_game *game, t_player *player, t_map *cell_map, t_music_game *music_tab)
{
	if (!(player->has_item == false && (cell_map->type & WALL) != WALL))
		return ;
	player->item = *cell_map;
	((t_object *)player->item.arg)->map_pos = (t_dvector2){-1, -1};
	player->item.type &= ~MUSIC & ~IS_PLAYING_MUSIC;
	if ((cell_map->type & NARRATOR) == NARRATOR)
	{
		play_narrator(game, cell_map, music_tab);
		player->item.type &= ~NARRATOR;
		cell_map->type &= ~NARRATOR;
	}
	if ((cell_map->type & MUSIC_OBJECT) == MUSIC_OBJECT)
		play_music(&player->item, music_tab,
			((t_object *)player->item.arg)->music, IS_PLAYING_MUSIC_OBJECT);
	if ((cell_map->type & IS_PLAYING_MUSIC_OBJECT) == IS_PLAYING_MUSIC_OBJECT)
		update_map_cell_music(&player->item, cell_map, music_tab);
	cell_map->type &= ~MUSIC_OBJECT & ~IS_PLAYING_MUSIC_OBJECT;
	cell_map->symbol = '0';
	cell_map->arg = NULL;
	cell_map->type &= ~OBJECT_INTERACTIVE;
	cell_map->type &= ~OBJECT;
	player->has_item = true;
}

t_dvector2	find_pos(t_player *player)
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
	return (pos);
}

void	take_object_click(t_game *game, t_player *player, t_map **map)
{
	t_dvector2	pos;

	pos = find_pos(player);
	if ((map[(int)pos.y][(int)pos.x].type & RECEPTACLE) == RECEPTACLE
		&& ((map[(int)pos.y][(int)pos.x].type & DOOR_LOCK) == DOOR_LOCK
		|| (map[(int)pos.y][(int)pos.x].type & OBJECT) == OBJECT))
		play_sound_fail(game, &map[(int)pos.y][(int)pos.x], game->music_array);
	if (!((map[(int)pos.y][(int)pos.x].type & WALL) == WALL
		&& (map[(int)pos.y][(int)pos.x].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE
		&& (map[(int)pos.y][(int)pos.x].type & OBJECT) == OBJECT))
		return ;
	player->item.symbol = map[(int)pos.y][(int)pos.x].symbol;
	player->item.type = map[(int)pos.y][(int)pos.x].type;
	player->item.type &= ~WALL;
	player->item.type &= ~MUSIC & ~IS_PLAYING_MUSIC;
	map[(int)pos.y][(int)pos.x].type &= ~MUSIC_OBJECT & ~IS_PLAYING_MUSIC_OBJECT;
	player->item.sprite[e_object_interactive_image] = map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_image];
	player->item.sprite[e_object_interactive_hand_image] = map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_hand_image];
	player->item.arg = find_empty_object(game);
	((t_object *)player->item.arg)->music = ((t_object *)map[(int)pos.y][(int)pos.x].arg)->music;
	((t_object *)player->item.arg)->map_pos = pos;
	player->has_item = true;
	if ((player->item.type & MUSIC_OBJECT) == MUSIC_OBJECT)
		play_music(&player->item, game->music_array, ((t_object *)player->item.arg)->music, IS_PLAYING_MUSIC_OBJECT);
	if ((map[(int)pos.y][(int)pos.x].type & NARRATOR) == NARRATOR)
	{
		play_narrator(game, &map[(int)pos.y][(int)pos.x], game->music_array);
		player->item.type &= ~NARRATOR;
		map[(int)pos.y][(int)pos.x].type &= ~NARRATOR;
	}
	map[(int)pos.y][(int)pos.x].type &= ~OBJECT_INTERACTIVE;
	map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_image] = map[(int)pos.y][(int)pos.x].sprite[e_object_interactive_after_image];
}

void	update_anim(long int time, t_sprite *sprite, t_image *img);

void	draw_hand_item(t_game *game, t_player *player)
{
	t_image		*image;
	t_sprite	*sprite;
	t_vector2	size;
	int			begin;

	sprite = &player->item.sprite[e_object_interactive_hand_image];
	image = &game->tab_images[sprite->index];
	if (sprite->frame != -1)
	{
		if (sprite->time == 0)
			sprite->time = game->time;
		if (sprite->frame < image->nb_total_frame
			&& game->time - sprite->time >= image->time_frame)
			update_anim(game->time, sprite, image);
		else if (sprite->frame == image->nb_total_frame
			&& game->time - sprite->time >= image->time_animation)
			update_anim(game->time, sprite, image);
		if (sprite->frame != image->nb_total_frame)
			image = &(game->tab_images[sprite->index + sprite->frame]);
	}
	size = image->size;
	if ((WIN_Y - image->size.y) < 0)
		size.y = WIN_Y;
	if ((WIN_X - image->size.x) < 0)
		size.x = WIN_X;
	begin = (WIN_Y - size.y) * game->image->size_line + (WIN_X - size.x) * 4;
	draw_image_with_green_sreen(game->image->addr + begin, image, (t_vector2){0}, size);
}
