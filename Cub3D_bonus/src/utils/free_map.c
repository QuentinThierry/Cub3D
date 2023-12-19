/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:40:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/04 15:41:07 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	free_map(t_map **map, t_vector2 size)
{
	int	i;
	int	j;

	i = 0;
	while (i < size.y)
	{
		j = 0;
		while (j < size.x)
		{
			if ((map[i][j].type & OBJECT) != OBJECT && map[i][j].arg != NULL)
				free(map[i][j].arg);
			j++;
		}
		free(map[i]);
		i++;
	}
	free(map);
}

void	free_map_object(t_map **map, t_vector2 size)
{
	int	i;
	int	j;

	i = 0;
	while (i < size.y)
	{
		j = 0;
		while (j < size.x)
		{
			if ((map[i][j].type & OBJECT) == OBJECT && map[i][j].arg != NULL)
				free(map[i][j].arg);
			j++;
		}
		i++;
	}
}

void	free_tab_object(t_object **object, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(object[i]);
		i++;
	}
	free(object);
}
