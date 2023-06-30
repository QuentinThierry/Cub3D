/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/30 23:53:31 by jvigny           ###   ########.fr       */
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

t_image	*get_image(t_game	*game, enum e_orientation orient)
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

t_vector2	get_dimension_maps(int fd, int nb_line, char *line)
{
	// int	fd;
	int	i;
	// char	*line;
	t_vector2	len;

	// i = 0;
	// fd = open(filename, O_RDONLY);
	// if (fd == -1)
	// 	return ((t_vector2){-1, -1});
	// line = get_next_line(fd);
	// while (line != NULL && i <= nb_line)
	// {
	// 	free(line);
	// 	line = get_next_line(fd);
	// 	i++;
	// }
	len.y = 0;
	len.x = 0;
	while (line != NULL)
	{
		i = skip_whitespace(line);
		if (line[0] == '\n' || line[0] == '\0' || line[i] == '\0')
			break;
		remove_new_line(line);
		if (len.x < (int)strlen(line))
			len.x = strlen(line);
		len.y++;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (len);
}
