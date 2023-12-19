/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 14:36:15 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 15:09:43 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	ft_read_anim(DIR *dir, t_texture *texture, char *dirname);

static bool	_realloc_tab(t_texture *texture)
{
	void	*tmp;

	tmp = ft_realloc(texture->filename_d,
			sizeof(char *) * (texture->nb_file),
			sizeof(char *) * (texture->nb_file + 1));
	if (tmp == NULL)
		return (free(texture->filename), texture->filename = NULL,
			print_error(NULL, 0), false);
	texture->filename_d = tmp;
	return (true);
}

bool	set_add_slash(char *filename)
{
	return (!(ft_strlen(filename) > 0
			&& filename[ft_strlen(filename) - 1] == '/'));
}

static bool	_set_texture(t_texture *texture, struct dirent *buffer,
			bool add_slash)
{
	DIR		*dir_anim;
	char	*filename;

	filename = ft_strjoin_slash(texture->filename,
			buffer->d_name, add_slash);
	if (filename == NULL)
		return (print_error(NULL, 0), false);
	dir_anim = opendir(filename);
	if (dir_anim != NULL)
	{
		if (!ft_read_anim(dir_anim, texture, filename))
			return (free(texture->filename), texture->filename = NULL, false);
	}
	else
	{
		if (!_realloc_tab(texture))
			return (free(filename), false);
		texture->total++;
		texture->filename_d[texture->nb_file] = filename;
		texture->nb_file++;
	}
	return (true);
}

/**
 * @brief read directory to add each filename find or call an other function for
 *		animation's directory. Skip hidden files and directories
 * 		Malloc filname_d's texture
 *
 * @param dir 
 * @param texture 
 * @return true 
 * @return false 
 */
bool	ft_read_dir(DIR *dir, t_texture *texture)
{
	struct dirent	*buffer;
	bool			add_slash;

	texture->nb_file = 0;
	add_slash = set_add_slash(texture->filename);
	buffer = readdir(dir);
	while (buffer != NULL)
	{
		if (buffer->d_name[0] == '.')
		{
			buffer = readdir(dir);
			continue ;
		}
		if (!_set_texture(texture, buffer, add_slash))
			return (closedir(dir), false);
		buffer = readdir(dir);
	}
	free(texture->filename);
	texture->filename = NULL;
	closedir(dir);
	if (texture->nb_file + texture->nb_animation <= 0)
		return (print_error("Empty directory\n", 1), false);
	return (true);
}
