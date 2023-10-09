/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 15:17:36 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

void	remove_new_line(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len >= 1 && str[len - 1] == '\n')
		str[len - 1] = '\0';
}

void	remove_end_whitespace(char *str)
{
	int	i;

	i = ft_strlen(str) - 1;
	if (i <= 0)
		return ;
	while (i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\v'
			|| str[i] == '\n' || str[i] == '\f' || str[i] == '\r'))
		i--;
	if (i + 1 < ft_strlen(str))
		str[i + 1] = '\0';
	return ;
}

/**
 * @brief Get the dimension of the map in the file
 * 
 * @param fd fd to read the map and until the end of the file
 * @param line last line read in the file with gnl
 * @param error set on true if error occurs during the function NOT USE FOR
 * 		THE MOMENT
 * @return t_vector2 width and lenght of the map
 */
t_vector2	get_dimension_maps(int fd, char *line, bool *error)
{
	t_vector2	len;

	*error = false;
	len.y = 0;
	len.x = 0;
	while (line != NULL)
	{
		if (line[0] == '\n')
			break ;
		remove_end_whitespace(line);
		if (len.x < (int)ft_strlen(line))
			len.x = ft_strlen(line);
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

void	free_tab(char **str, int sizey)
{
	int	i;

	i = 0;
	while (i < sizey)
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
