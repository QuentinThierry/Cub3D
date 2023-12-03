/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:55:15 by jvigny            #+#    #+#             */
/*   Updated: 2023/12/03 19:08:10 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	is_sound(t_music_name *filename, int nb_music, char symbol)
{
	int	i;

	i = 0;
	while (i < nb_music)
	{
		if (filename[i].symbol == symbol)
			return (true);
		i++;
	}
	return (false);
}

char	*get_music(t_music_name *filename, int nb_music, char symbol,
			enum e_orientation orient)
{
	int	i;

	i = 0;
	while (i < nb_music)
	{
		if (filename[i].symbol == symbol)
		{
			if (filename[i].orient == orient
				|| (orient == e_music
					&& filename[i].orient == e_music_receptacle))
				return (filename[i].filename);
		}
		i++;
	}
	return (NULL);
}

t_music_name	*get_narrator(t_music_name *filename, int nb_music, char symbol,
				enum e_orientation orient)
{
	int	i;

	i = 0;
	while (i < nb_music)
	{
		if (filename[i].symbol == symbol)
		{
			if (filename[i].orient == orient
				|| (orient == e_narrator
					&& filename[i].orient == e_narrator_receptacle))
				return (&filename[i]);
		}
		i++;
	}
	return (NULL);
}

void	play_first_music(t_game *game)
{
	if ((game->map[(int)game->player->f_pos.y][(int)game->player->f_pos.x].type
		& MUSIC) == MUSIC)
		play_music(&game->map[(int)game->player->f_pos.y]
		[(int)game->player->f_pos.x], game->music_array,
			game->map[(int)game->player->f_pos.y]
		[(int)game->player->f_pos.x].music, IS_PLAYING_MUSIC);
	if ((game->map[(int)game->player->f_pos.y][(int)game->player->f_pos.x].type
		& NARRATOR) == NARRATOR)
	{
		play_narrator(game, &game->map[(int)game->player->f_pos.y]
		[(int)game->player->f_pos.x], game->music_array);
		game->map[(int)game->player->f_pos.y]
		[(int)game->player->f_pos.x].type &= ~NARRATOR;
	}
}
