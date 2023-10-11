/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_audio.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:09:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/11 16:10:49 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

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
					music_array[i].map_cell->type &= ~IS_PLAYING_MUSIC & ~IS_PLAYING_OBJECT;
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
