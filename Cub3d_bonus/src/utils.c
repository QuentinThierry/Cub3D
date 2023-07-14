/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/14 23:24:59 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

enum e_orientation	get_wall_orientation(t_player player, t_fvector2 wall)
{
	if ((wall.x - (int)wall.x) != 0)
	{
		if (player.f_real_pos.y > wall.y)
			return (e_north);
		else
		 	return (e_south);
	}
	else
	{
		if (player.f_real_pos.x > wall.x)
			return (e_west);
		else
		 	return (e_east);
	}
}

t_image	*get_image(t_game *game, enum e_orientation orient)
{
	return (game->tab_images[orient]);
}


int skip_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' || str[i] == '\n'
			|| str[i] == '\f' || str[i] == '\r')
		i++;
	return (i);
}

void	remove_new_line(char *str)
{
	int	len;

	len = strlen(str);
	if (len >= 1 && str[len - 1] == '\n')
		str[len - 1] = '\0';
}

t_vector2	get_dimension_maps(int fd, char *line, bool *error)
{
	t_vector2	len;

	*error = false;
	len.y = 0;
	len.x = 0;
	while (line != NULL)
	{
		remove_new_line(line);
		if (len.x < (int)strlen(line))
			len.x = strlen(line);
		len.y++;
		free(line);
		line = get_next_line(fd);
	}
	while (line != NULL)
	{
		if (line[0] != '\n')
			*error = true;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (len);
}

void	free_tab(void **str, t_vector2 size)
{
	int	i;

	i = 0;
	while (i < size.y)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_str(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}
