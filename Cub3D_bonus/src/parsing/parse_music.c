/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_music.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:32:29 by jvigny            #+#    #+#             */
/*   Updated: 2023/11/11 14:44:40 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

#define READ_SIZE 100

static bool	check_format(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (print_error("Wrong name of subtitle\n", 1), false);
	if (ft_strncmp(filename + (len - 4), ".txt", 4) != 0)
		return (print_error("Wrong format of subtitle\n", 1), false);
	return (true);
}

static bool	load_subtitle(t_music_name *music)
{
	int		fd;
	int		ret;
	char	buffer[READ_SIZE + 1];
	char	*res;
	char	*tmp;

	if (!check_format(music->subtitle))
		return (false);
	fd = open(music->subtitle, O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), false);
	res = NULL;
	buffer[READ_SIZE] = 0;
	ret = read(fd, buffer, READ_SIZE);
	while (ret > 0)
	{
		if (ret != READ_SIZE)
			buffer[ret] = 0;
		tmp = ft_strjoin(res, buffer);
		free(res);
		if (tmp == NULL)
			return (print_error("Malloc failed\n", 1), close(fd), false);
		res = tmp;
		if (ret != READ_SIZE)
			break ;
		ret = read(fd, buffer, READ_SIZE);
	}
	free(music->subtitle);
	music->subtitle = res;
	close(fd);
	if (ret == -1)
		return (print_error(NULL, 0), false);
	return (true);
}

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

bool	check_sound(char *filename)
{
	int	fd;
	DIR	*dir;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error("Invalid sound\n", 1),false);
	close(fd);
	dir = opendir(filename);
	if (dir != NULL)
		return (closedir(dir), print_error("Invalid sound\n", 1), false);
	return (true);
}

bool	find_music(t_game *game, char *str, enum e_orientation orient, int i)
{
	char	*filename;
	void	*tmp;
	int		len;
	int		index;
	char	tmp1 = -1;

	index = game->nb_music;
	if (_is_existing(game, *(str - 1), orient))
		return (print_error("Multiples definition of a sound\n", 1), false);
	if (index >= game->nb_music)
	{
		tmp = ft_realloc(game->file_music,
				sizeof(t_music_name) * game->nb_music,
				sizeof(t_music_name) * (index + 1));
		if (tmp == NULL)
			return (print_error(NULL, 0), false);
		game->file_music = tmp;
		game->nb_music = index + 1;
	}
	game->file_music[index].orient = orient;
	game->file_music[index].symbol = *(str - 1);
	if (i >= ft_strlen(str))
		return (print_error("Empty texture\n", 1), false);
	i += skip_whitespace(str + i);
	if (str[i] == '\0')
		return (print_error("Empty texture\n", 1), false);
	len = find_next_wsp(str + i, 0);
	if (len >= 0 && (str[i + len] == ' ' || str[i + len] == '\t'
			|| str[i + len] == '\v' || str[i + len] == '\n'
			|| str[i + len] == '\f' || str[i + len] == '\r'))
	{
		tmp1 = str[i + len];
		str[i + len] = '\0';
	}
	filename = ft_strdup(str + i);
	if (filename == NULL)
		return (print_error("malloc failed\n", 1), false);
	if (!check_sound(filename))
		return (free(filename), false);
	if (tmp1 != -1)
		str[i + len] = tmp1;
	i += len + 1;
	game->file_music[index].filename = filename;
	if (orient == e_narrator || orient == e_narrator_receptacle
		|| orient == e_narrator_receptacle_complete
		|| orient == e_narrator_receptacle_complete)
	{
		tmp1 = -1;
		if (i >= ft_strlen(str))
			return (print_error("Empty texture\n", 1), false);
		i += skip_whitespace(str + i);
		if (str[i] == '\0')
			return (print_error("Empty texture\n", 1), false);
		len = find_next_wsp(str + i, 0);
		if (len >= 0 && (str[i + len] == ' ' || str[i + len] == '\t'
				|| str[i + len] == '\v' || str[i + len] == '\n'
				|| str[i + len] == '\f' || str[i + len] == '\r'))
		{
			tmp1 = str[i + len];
			str[i + len] = '\0';
		}
		filename = ft_strdup(str + i);
		if (filename == NULL)
			return (print_error("malloc failed\n", 1), false);
		if (tmp1 != -1)
			str[i + len] = tmp1;
		i += len + 1;
		game->file_music[index].subtitle = filename;
		if (!load_subtitle(&game->file_music[index]))
			return (false);
	}
	if (orient == e_music_receptacle || orient == e_narrator_receptacle)
		return (find_music(game, str, orient + 1, i));
	return (true);
}
