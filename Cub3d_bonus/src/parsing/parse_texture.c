/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 01:50:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 16:31:35 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"
#include "../../includes/get_next_line.h"

bool	ft_read_dir(DIR *dir, t_texture *texture);
bool	is_existing(t_game *game, int index, char symbol,
			enum e_orientation orient);
bool	multiple_texture(t_game *game, int *index, char *str,
			enum e_orientation orient);
bool	cmp_texture(char *line, t_game *game, int i, bool *is_end);

/**
 * @brief return the index of the next whitespace or the '\0' if none has
 * 	been find
 */
int	find_next_wsp(char *line, int i)
{
	while (line[i] != '\0' && !(line[i] == ' ' || line[i] == '\t'
			|| line[i] == '\v' || line[i] == '\n' || line[i] == '\f'
			|| line[i] == '\r'))
		i++;
	return (i);
}

static void	_set_symbol(t_game *game, int index, char *str)
{
	if (index == e_north || index == e_east || index == e_south
		|| index == e_west)
		game->filename[index].symbol = '1';
	else if (index == e_floor || index == e_ceiling)
		game->filename[index].symbol = '0';
	else
		game->filename[index].symbol = *(str - 1);
}

/**
 * @brief complete the tab of filename with the new texture name,
 *		its orientation, its symbol on the map
 * 		malloc the tab and the filename
 * @param game struct to add the texture
 * @param str name of the texture to add
 * @param index offset on the tab to add the texture
 * @param orient orientation of the texture
 * @return true On success
 * @return false ERROR, error already print
 */
bool	find_texture(t_game *game, char *str, int index,
			enum e_orientation orient)
{
	DIR	*dir;
	int	i;

	if (is_existing(game, index, *(str - 1), orient))
		return (print_error("Multiples definition of a texture\n", 1), false);
	if (orient == e_receptacle_empty || orient == e_door_lock
		|| orient == e_object_interactive)
		return (multiple_texture(game, &index, str, orient));
	if (index >= game->nb_file && !realloc_tab_texture(game, index))
		return (false);
	game->filename[index].orient = orient;
	game->filename[index].nb_file = 1;
	_set_symbol(game, index, str);
	i = 0;
	game->filename[index].filename = get_filename(str, &i);
	if (game->filename[index].filename == NULL)
		return (false);
	dir = opendir(game->filename[index].filename);
	if (dir != NULL)
		return (ft_read_dir(dir, &(game->filename[index])));
	game->filename[index].total++;
	return (true);
}

bool	check_texture(t_texture	*filename, char *line)
{
	int	i;

	i = 0;
	while (i < 6)
	{
		if (filename[i].filename == NULL && filename[i].nb_file <= 0
			&& filename[i].nb_animation <= 0)
			return (print_error("need the mandatory texture\n", 1), free(line),
				false);
		i++;
	}
	return (true);
}

/**
 * @brief Parse the first part of the file that contain the name of the texture
 * 
 * @param fd of the file, not -1
 * @param game struct to add the texture
 * @param nb_line pointer to the number of lines, that will be read
 * @param rest pointer to the last string, that will be read
 * @return true On sucess
 * @return false ERROR, error already print
 */
bool	parse_texture(int fd, t_game *game, int *nb_line, char **line)
{
	int		i;
	bool	is_end;

	is_end = false;
	*nb_line = 0;
	*line = get_next_line(fd);
	while (*line != NULL && !is_end)
	{
		(*nb_line)++;
		if ((*line)[0] == '\n')
		{
			free(*line);
			*line = get_next_line(fd);
			continue ;
		}
		i = skip_whitespace(*line);
		if (ft_strlen(*line + i) < 1)
			return (print_error("Line to short\n", 1), free(*line), false);
		if (!cmp_texture(*line, game, i, &is_end))
			return (free(*line), false);
		free(*line);
		*line = get_next_line(fd);
	}
	return (check_texture(game->filename, *line));
}
