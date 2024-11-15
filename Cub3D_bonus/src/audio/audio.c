/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 17:16:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/12/04 18:59:51 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	init_audio(t_game *game)
{
	InitAudioDevice();
	if (!IsAudioDeviceReady() || NB_MAX_SOUNDS <= 1)
		return (print_error("Audio init failed\n", 1), false);
	game->music_array = ft_calloc(NB_MAX_SOUNDS, sizeof(t_music_game));
	if (game->music_array == NULL)
		return (false);
	SetMasterVolume(DFL_SOUND);
	game->music_array[0].music = LoadMusicStream(BACKGROUND_MUSIC);
	if (!IsMusicReady(game->music_array[0].music))
		return (print_error("Background music init failed\n", 1), false);
	game->music_array[0].is_playing = true;
	return (true);
}

void	close_audio(t_music_game *music_tab)
{
	int	i;

	if (!IsAudioDeviceReady())
		return (free(music_tab));
	if (music_tab == NULL)
	{
		free(music_tab);
		CloseAudioDevice();
		return ;
	}
	i = 0;
	while (i < NB_MAX_SOUNDS)
	{
		if (music_tab[i].is_playing == true)
		{
			if (IsMusicStreamPlaying(music_tab[i].music))
				StopMusicStream(music_tab[i].music);
			UnloadMusicStream(music_tab[i].music);
			if (music_tab[i].music.ctxType == 1)
				free(music_tab[i].music.ctxData);
		}
		i++;
	}
	CloseAudioDevice();
	free(music_tab);
}

static t_music_game	*find_empty_place(t_music_game *music_tab)
{
	int	i;

	i = 2;
	while (i < NB_MAX_SOUNDS)
	{
		if (!music_tab[i].is_playing)
			return (&music_tab[i]);
		i++;
	}
	return (NULL);
}

void	play_music(t_map *map_cell, t_music_game *music_tab, char *filename
	, unsigned int type)
{
	t_music_game	*music;

	if ((map_cell->type & IS_PLAYING_MUSIC) == IS_PLAYING_MUSIC
		|| (map_cell->type & IS_PLAYING_MUSIC_OBJECT) == IS_PLAYING_MUSIC_OBJECT)
		return ;
	music = find_empty_place(music_tab);
	if (music == NULL)
		return ;
	music->music = LoadMusicStream(filename);
	if (!IsMusicReady(music->music))
	{
		print_error("Loading music fail\n", 1);
		return ;
	}
	music->music.looping = false;
	music->map_cell = map_cell;
	music->is_playing = true;
	map_cell->type |= type;
	PlayMusicStream(music->music);
}

void	play_narrator(t_game *game, t_map *map_cell, t_music_game *music_tab)
{
	if (music_tab[1].is_playing == true)
	{
		StopMusicStream(music_tab[1].music);
		UnloadMusicStream(music_tab[1].music);
		if (music_tab[1].music.ctxType == 1)
			free(music_tab[1].music.ctxData);
		music_tab[1].map_cell->type &= ~IS_PLAYING_NARRATOR;
		music_tab[1].map_cell = NULL;
		music_tab[1].is_playing = false;
		music_tab[1].is_subtitle = false;
		map_cell->narrator->offset = 0;
	}
	music_tab[1].music = LoadMusicStream(map_cell->narrator->filename);
	if (!IsMusicReady(music_tab[1].music))
	{
		print_error("Loading music fail\n", 1);
		return ;
	}
	music_tab[1].music.looping = false;
	music_tab[1].is_subtitle = true;
	music_tab[1].map_cell = map_cell;
	music_tab[1].is_playing = true;
	map_cell->type |= IS_PLAYING_NARRATOR;
	map_cell->narrator->time = game->time;
	PlayMusicStream(music_tab[1].music);
}

void	set_next_narrator(t_map *map_cell)
{
	if ((map_cell->type & NARRATOR) == NARRATOR)
	{
		map_cell->type ^= NARRATOR;
		if (map_cell->narrator != NULL)
			map_cell->type |= NARRATOR_RECEPTACLE;
	}
	else
		map_cell->type ^= NARRATOR_RECEPTACLE;
}

void	play_sound_fail(t_game *game, t_map *map_cell, t_music_game *music_tab)
{
	if ((map_cell->type & MUSIC) == MUSIC)
		play_music(map_cell, music_tab, map_cell->music, IS_PLAYING_MUSIC);
	if ((map_cell->type & NARRATOR) == NARRATOR)
	{
		play_narrator(game, map_cell, music_tab);
		set_next_narrator(map_cell);
	}
}

void	update_sounds(t_music_game *music_array)
{
	int	i;

	i = 0;
	while (i < NB_MAX_SOUNDS)
	{
		if (music_array[i].is_playing == true)
		{
			if (IsMusicStreamPlaying(music_array[i].music))
				UpdateMusicStream(music_array[i].music);
			else
			{
				UnloadMusicStream(music_array[i].music);
				if (music_array[i].music.ctxType == 1)
					free(music_array[i].music.ctxData);
				music_array[i].is_playing = false;
				if (i == 1)
					music_array[i].map_cell->type &= ~IS_PLAYING_NARRATOR;
				else
					music_array[i].map_cell->type &= ~IS_PLAYING_MUSIC & ~IS_PLAYING_MUSIC_OBJECT;
			}
		}
		i++;
	}
}

void	update_map_cell_music(t_map *map_cell, t_map *old_map_cell, t_music_game *music_array)
{
	int	i;

	i = 0;
	while (i < NB_MAX_SOUNDS)
	{
		if (music_array[i].map_cell == old_map_cell)
		{
			music_array[i].map_cell = map_cell;
			return ;
		}
		i++;
	}
}

void	clear_sound(t_music_game *music_array)
{
	int	i;

	i = 1;
	while (i < NB_MAX_SOUNDS)
	{
		if (music_array[i].is_playing == true)
		{
			if (IsMusicStreamPlaying(music_array[i].music))
				StopMusicStream(music_array[i].music);
			UnloadMusicStream(music_array[i].music);
			if (music_array[i].music.ctxType == 1)
				free(music_array[i].music.ctxData);
			music_array[i].is_playing = false;
		}
		i++;
	}
}
