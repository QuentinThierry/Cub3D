/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill_wall.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 23:05:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/14 23:10:15 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	ft_fill_wall(char *line, t_wall *map, t_vector2 map_size)
{
	int	i;
	int	len;

	i = 0;
	len = strlen(line);
	while (i < len && i < map_size.x)
	{
		if (line[i] == '\n')
			break ;
		map[i].symbol = line[i];
		i++;
	}
	while (i < map_size.x)
	{
		map[i].symbol = ' ';
		i++;
	}
	free(line);
}
