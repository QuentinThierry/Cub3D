/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_anim.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 14:57:51 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 15:25:13 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	sort_animation(t_animation *anim);
bool	set_add_slash(char *filename);

static bool	_realloc_anim(t_texture *texture)
{
	void	*tmp;

	tmp = ft_realloc(texture->animation,
			sizeof(t_animation) * (texture->nb_animation),
			sizeof(t_animation) * (texture->nb_animation + 1));
	if (tmp == NULL)
		return (print_error(NULL, 0), false);
	texture->animation = tmp;
	return (true);
}

static bool	_realloc_sprite(t_animation *anim)
{
	void	*tmp;

	tmp = ft_realloc(anim->filename,
			sizeof(char *) * (anim->nb_sprite),
			sizeof(char *) * (anim->nb_sprite + 1));
	if (tmp == NULL)
		return (print_error(NULL, 0), false);
	anim->filename = tmp;
	return (true);
}

static bool	_set_info_sprite(char *dirname, struct dirent *buffer,
			t_animation *anim)
{
	char			*filename;
	bool			add_slash;

	add_slash = set_add_slash(dirname);
	filename = ft_strjoin_slash(dirname, buffer->d_name, add_slash);
	if (filename == NULL)
		return (print_error(NULL, 0), free(dirname), false);
	if (!_realloc_sprite(anim))
		return (free(filename), free(dirname), false);
	anim->filename[anim->nb_sprite] = filename;
	anim->nb_sprite++;
	return (true);
}

/**
 * @brief read directory to parse the animation find in the file.
 *		Skip hidden files and directories
 * 		Malloc tab of animation and animation's filename
 *
 * @param dir 
 * @param texture 
 * @param dirname 
 * @return true 
 * @return false 
 */
bool	ft_read_anim(DIR *dir, t_texture *texture, char *dirname)
{
	struct dirent	*buffer;
	bool			has_config;

	if (!_realloc_anim(texture))
		return (closedir(dir), free(dirname), false);
	buffer = (void *)1;
	while (buffer != NULL)
	{
		buffer = readdir(dir);
		if (buffer == NULL || buffer->d_name[0] == '.')
			continue ;
		if (!_set_info_sprite(dirname, buffer,
				&texture->animation[texture->nb_animation]))
			return (closedir(dir), false);
		texture->total++;
	}
	texture->total--;
	(free(dirname), closedir(dir));
	has_config = sort_animation(&(texture->animation[texture->nb_animation]));
	if (has_config == false)
		print_error("Missing the file config.cfg for the animations\n", 1);
	texture->nb_animation++;
	if (texture->animation[texture->nb_animation - 1].nb_sprite <= 1)
		return (print_error("Empty directory for animation\n", 1), false);
	return (has_config);
}
