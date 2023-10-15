/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_music.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:32:29 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 18:31:21 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	load_subtitle(t_music_name *music);

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
			else if (game->file_music[i].orient == e_music_receptacle
				&& orient == e_music)
				return (true);
			else if (game->file_music[i].orient == e_music_receptacle
				&& orient == e_music_object)
				return (true);
			else if (game->file_music[i].orient == e_narrator_receptacle
				&& orient == e_narrator)
				return (true);
		}
		i++;
	}
	return (false);
}

static bool	_get_subtitle(char *str, int *i, t_music_name *music)
{
	int	len;

	len = ft_strlen(str);
	if (*i >= len)
		return (print_error("Empty subtitle\n", 1), false);
	music->subtitle = get_filename(str, i);
	if (music->subtitle == NULL)
		return (false);
	if (!load_subtitle(music))
		return (false);
	return (true);
}

static bool	_realloc_tab(t_game *game, int index)
{
	void	*tmp;

	tmp = ft_realloc(game->file_music,
			sizeof(t_music_name) * game->nb_music,
			sizeof(t_music_name) * (index + 1));
	if (tmp == NULL)
		return (print_error(NULL, 0), false);
	game->file_music = tmp;
	game->nb_music = index + 1;
	return (true);
}

bool	check_sound(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error("Invalid sound\n", 1),false);
	close(fd);
	return (true);
}

bool	find_music(t_game *game, char *str, enum e_orientation orient, int i)
{
	int	index;
	int	len;

	index = game->nb_music;
	if (_is_existing(game, *(str - 1), orient))
		return (print_error("Multiples definition of a sound\n", 1), false);
	if (!_realloc_tab(game, index))
		return (false);
	game->file_music[index].orient = orient;
	game->file_music[index].symbol = *(str - 1);
	len = ft_strlen(str);
	if (i >= len)
		return (print_error("Empty sound\n", 1), false);
	game->file_music[index].filename = get_filename(str, &i);
	if (game->file_music[index].filename == NULL
		|| !check_sound(game->file_music[index].filename))
		return (false);
	if (orient == e_narrator || orient == e_narrator_receptacle
		|| orient == e_narrator_receptacle_complete
		|| orient == e_narrator_receptacle_complete)
		if (!_get_subtitle(str, &i, &game->file_music[index]))
			return (false);
	if (orient == e_music_receptacle || orient == e_narrator_receptacle)
		return (find_music(game, str, orient + 1, i));
	return (true);
}
