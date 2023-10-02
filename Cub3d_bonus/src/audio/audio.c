/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 17:16:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/02 14:01:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	init_audio(t_game *game, t_music_name *music_file, int nb_music)
{
	int	i;
	
	InitAudioDevice();
	if (!IsAudioDeviceReady() || NB_MAX_SOUNDS <= 0)
		return (printf("Error : Audio init failed\n"), false);
	game->music_array = ft_calloc(NB_MAX_SOUNDS, sizeof(t_music_game));
	if (!game->music_array)
		return (false);
	i = 0;
	game->music_array[0].music = LoadMusicStream(BACKGROUND_MUSIC);
	if (!IsMusicReady(game->music_array[0].music))
		return (printf("Error : Background music init failed\n"), false);
	game->music_array->is_playing = true;
	return (true);
}

void	close_audio(t_music_game *music_tab)
{
	int	i;

	if (!IsAudioDeviceReady())
		return (free(music_tab));
	i = 0;
	while (i < NB_MAX_SOUNDS)
	{
		if (music_tab[i].is_playing == true && IsMusicStreamPlaying(music_tab[i].music))
		{
			StopMusicStream(music_tab[i].music);
			UnloadMusicStream(music_tab[i].music);
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
			return (printf("%d\n", i), &music_tab[i]);
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
	printf("play %s\n", filename);
	music->music = LoadMusicStream(filename);
	if (!IsMusicReady(music->music))
		return ;
	music->music.looping = false;
	music->map_cell = map_cell;
	music->is_playing = true;
	map_cell->type |= type;
	PlayMusicStream(music->music);
}

void	play_narrator(t_game *game, t_map *map_cell, t_music_game *music_tab)
{
	t_music_game	*music;

	music = &music_tab[1];
	if (music->is_playing == true)
	{
		StopMusicStream(music->music);
		UnloadMusicStream(music->music);
		music->map_cell->type &= ~IS_PLAYING_NARRATOR;
		music->map_cell = NULL;
		music->is_playing = false;
		music->is_subtitle = false;
		map_cell->narrator->offset = 0;
	}
	printf("play narrator %s\n", map_cell->narrator->filename);
	music->music = LoadMusicStream(map_cell->narrator->filename);
	if (!IsMusicReady(music->music))
		return ;
	music->music.looping = false;
	music->is_subtitle = true;
	music->map_cell = map_cell;
	music->is_playing = true;
	map_cell->type |= IS_PLAYING_NARRATOR;
	map_cell->narrator->time = game->time;
	PlayMusicStream(music->music);
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
	else if ((map_cell->type & NARRATOR) == NARRATOR)
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
				if (i == 1)
					music_array[i].map_cell->type &= ~IS_PLAYING_NARRATOR;
				else
					music_array[i].map_cell->type &= ~IS_PLAYING_MUSIC & ~IS_PLAYING_MUSIC_OBJECT;
				// music_array[i].map_cell = NULL;
				music_array[i].is_playing = false;
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
			// music_array[i].map_cell = NULL;
			music_array[i].is_playing = false;
		}
		i++;
	}
}
