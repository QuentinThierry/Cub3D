/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_audio.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:07:03 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/11 16:07:51 by jvigny           ###   ########.fr       */
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
