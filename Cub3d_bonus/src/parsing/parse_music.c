/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_music.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:32:29 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/22 15:36:35 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static bool	_is_existing(t_game *game, char symbol, enum e_orientation orient)
{
	int		i;

	i = 0;
	while (i < game->nb_music)
	{
		if (game->file_music[i].symbol == symbol)
		{
			if (game->file_music[i].orient == orient)
				return (true);
			else if (game->file_music[i].orient == e_music_receptacle && orient == e_music)
				return (true);
			else if (game->file_music[i].orient == e_narrator_receptacle && orient == e_narrator)
				return (true);
		}
		i++;
	}
	return (false);
}

bool	find_music(t_game *game, char *str, enum e_orientation orient, int i)
{
	char	*filename;
	int		len;
	int		index;
	
	index = game->nb_music;
	if (_is_existing(game, *(str - 1), orient))
		return (printf("Error : Multiples definition of a sound\n"), false);
	if (index >= game->nb_music)
	{
		game->file_music = ft_realloc(game->file_music
			, sizeof(t_music_name) * game->nb_music, sizeof(t_music_name) * (index + 1));
		if (game->file_music == NULL)
			return (perror("Error"), false);
		game->nb_music = index + 1;
	}
	game->file_music[index].orient = orient;
	game->file_music[index].symbol = *(str - 1);
	i += skip_whitespace(str + i);
	if (str[i] == '\0')
		return (printf("Error : Empty texture\n"), false);
	len = find_next_wsp(str + i, 0);
	if (len >= 0 && (str[i + len] == ' ' || str[i + len] == '\t'
		|| str[i + len] == '\v' || str[i + len] == '\n' || str[i + len] == '\f'
		|| str[i + len] == '\r'))
		str[i + len] = '\0';
	filename = ft_strdup(str + i);
	i += len + 1;
	if (filename == NULL)
		return (printf("Error : malloc failed\n"), false);
	game->file_music[index].filename = filename;
	if (orient == e_narrator || orient == e_narrator_receptacle || orient == e_narrator_receptacle_complete)
	{
		i += skip_whitespace(str + i);
		if (str[i] == '\0')
			return (printf("Error : Empty texture\n"), false);
		len = find_next_wsp(str + i, 0);
		if (len >= 0 && (str[i + len] == ' ' || str[i + len] == '\t'
			|| str[i + len] == '\v' || str[i + len] == '\n' || str[i + len] == '\f'
			|| str[i + len] == '\r'))
			str[i + len] = '\0';
		filename = ft_strdup(str + i);
		i += len + 1;
		if (filename == NULL)
			return (printf("Error : malloc failed\n"), false);
		game->file_music[index].subtitle = filename;
	}
	if (orient == e_music_receptacle || orient == e_narrator_receptacle)
		return (find_music(game, str, orient + 1, i));
	return (true);
}
