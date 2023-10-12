/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_anim.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 20:34:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 20:55:17 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"
#include "../../includes/get_next_line.h"

static void	_get_config(int fd, t_animation *animation, bool *error)
{
	char	*buffer;

	buffer = get_next_line(fd);
	if (buffer != NULL)
		animation->time_sprite = ft_atoi(buffer);
	free(buffer);
	if (animation->time_sprite <= 0)
		*error = true;
	buffer = get_next_line(fd);
	if (buffer != NULL)
		animation->time_animation = ft_atoi(buffer);
	free(buffer);
	if (animation->time_animation <= 0)
		*error = true;
}

/**
 * @brief read the config file and stock the usefull information
 * 
 * @param animation 
 * @param index 
 * @return true 
 * @return false 
 */
bool	ft_read_config(t_animation *animation, int index)
{
	int		fd;
	char	*buffer;
	bool	error;

	error = false;
	fd = open(animation->filename[index], O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), false);
	_get_config(fd, animation, &error);
	buffer = get_next_line(fd);
	while (buffer != NULL)
	{
		if (!(buffer[0] == '\n' || buffer[0] == '\0'))
			error = true;
		free(buffer);
		buffer = get_next_line(fd);
	}
	close(fd);
	if (error == true)
		print_error("Wrong format of config file\n", 1);
	return (!error);
}

void	swap(char **str, int a, int b)
{
	char	*tmp;

	tmp = str[a];
	str[a] = str[b];
	str[b] = tmp;
}

static bool	_move_config(t_animation *anim)
{
	bool	has_config;
	int		i;

	i = 0;
	has_config = false;
	while (i < anim->nb_sprite)
	{
		if (ft_strlen(anim->filename[i]) >= 10 && ft_strcmp(anim->filename[i]
				+ (ft_strlen(anim->filename[i]) - 10), "config.cfg") == 0)
		{
			swap(anim->filename, i, 0);
			has_config = true;
			break ;
		}
		i++;
	}
	return (has_config);
}

/**
 * @brief sort the filenstrncmpames of animation by alphbetic order and return if
 *		"config.cfg" is present. "config.cfg" will have the first position
 * 
 * @param anim 
 * @return true 
 * @return false 
 */
bool	sort_animation(t_animation *anim)
{
	int		i;
	int		j;
	bool	has_config;

	has_config = _move_config(anim);
	i = 1;
	while (i + 1 < anim->nb_sprite)
	{
		j = 1;
		while (j + 1 < anim->nb_sprite)
		{
			if (ft_strcmp(anim->filename[j], anim->filename[j + 1]) > 0)
				swap(anim->filename, j, j + 1);
			j++;
		}
		i++;
	}
	return (has_config);
}
