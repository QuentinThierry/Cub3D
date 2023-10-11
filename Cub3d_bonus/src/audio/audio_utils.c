/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:55:15 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/11 15:34:22 by jvigny           ###   ########.fr       */
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
