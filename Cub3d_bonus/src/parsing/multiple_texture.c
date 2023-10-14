/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 14:23:28 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 20:30:41 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	ft_read_dir(DIR *dir, t_texture *texture);

bool	realloc_tab_texture(t_game *game, int index)
{
	void	*tmp;

	tmp = ft_realloc(game->filename,
			sizeof(t_texture) * game->nb_file, sizeof(t_texture) * (index + 1));
	if (tmp == NULL)
		return (print_error(NULL, 0), false);
	game->filename = tmp;
	game->nb_file = index + 1;
	return (true);
}

static bool	_try_open_directory(t_texture *texture)
{
	DIR		*dir;

	dir = opendir(texture->filename);
	if (dir == NULL)
		texture->total++;
	else if (!ft_read_dir(dir, texture))
		return (false);
	return (true);
}

static bool	_set_info(enum e_orientation orient, int cpt, char *str,
			t_texture *texture)
{
	if (orient == e_receptacle_empty)
	{
		texture->orient = e_receptacle_empty + cpt;
		texture->sym_rcp = *(str - 3);
	}
	else if (orient == e_door_lock)
	{
		texture->orient = e_door_lock + cpt;
		texture->sym_rcp = *(str - 3);
	}
	else
		texture->orient = e_object_interactive + cpt;
	texture->symbol = *(str - 1);
	texture->nb_file = 1;
	return (_try_open_directory(texture));
}

static char	*_get_filename(char *str, int *i, t_texture *texture)
{
	int		len;
	char	tmp;

	tmp = -1;
	len = find_next_wsp(str + *i, 0);
	if (len >= 0 && (str[*i + len] == ' ' || str[*i + len] == '\t'
			|| str[*i + len] == '\v' || str[*i + len] == '\n'
			|| str[*i + len] == '\f' || str[*i + len] == '\r'))
	{
		tmp = str[*i + len];
		str[*i + len] = '\0';
	}
	texture->filename = ft_strdup(str + *i);
	if (texture->filename == NULL)
		return (print_error("malloc failed\n", 1), NULL);
	if (tmp != -1)
		str[*i + len] = tmp;
	*i += len + 1;
	return (texture->filename);
}

bool	multiple_texture(t_game *game, int *index, char *str,
		enum e_orientation orient)
{
	int	cpt;
	int	nb_file;
	int	i;

	i = 0;
	cpt = -1;
	nb_file = 2 + 2 * !(orient == e_receptacle_empty || orient == e_door_lock);
	while (++cpt < nb_file)
	{
		if (i >= ft_strlen(str) && nb_file == 4 && cpt == 2)
			return (true);
		if (i >= ft_strlen(str))
			return (print_error("Empty texture\n", 1), false);
		i += skip_whitespace(str + i);
		if (str[i] == '\0' && nb_file == 4 && cpt == 2)
			return (true);
		if (str[i] == '\0')
			return (print_error("Empty texture\n", 1), false);
		if (*index >= game->nb_file && !realloc_tab_texture(game, *index))
			return (false);
		if (_get_filename(str, &i, &game->filename[*index]) == NULL
			|| !_set_info(orient, cpt, str, &game->filename[(*index)++]))
			return (false);
	}
	return (true);
}
