/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill_wall.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 23:05:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/17 18:28:10 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

/**
 * @brief initialize the map the symbol find and each texture depending on
 * 		the wall orientation and the ceiling and floor, and if its a wall
 *		It also complete the map with space if the line is smaller than lenght
 * 		 of the map
 * 
 * @param game 
 * @param line free the string
 * @param map 
 * @param map_size 
 * @return bool
 */
bool	ft_fill_wall(t_game *game, char *line, t_map *map, t_vector2 map_size)
{
	int	i;
	int	len;
	bool	error;

	i = 0;
	len = strlen(line);
	while (i < len && i < map_size.x)
	{
		if (line[i] == '\n')
			break ;
		map[i].symbol = line[i];
		if (line[i] == ' ')
		{
			i++;
			continue ;
		}
		if (line[i] == 'N' || line[i] == 'S' || line[i] == 'W' || line[i] == 'E')
			line[i] = '0';
		if (is_wall(line[i], game->filename, game->nb_sprite, &error))
		{
			map[i].is_wall = true;
			if (line[i] == 'o')
				map[i].is_wall = false;
			map[i].sprite[e_north].index = fill_texture(game->filename, game->nb_sprite, line[i], e_north);
			map[i].sprite[e_east].index = fill_texture(game->filename, game->nb_sprite, line[i], e_east);
			map[i].sprite[e_south].index = fill_texture(game->filename, game->nb_sprite, line[i], e_south);
			map[i].sprite[e_west].index = fill_texture(game->filename, game->nb_sprite, line[i], e_west);
			map[i].sprite[e_down].index = e_floor;
			map[i].sprite[e_up].index = e_ceiling;
		}
		else
		{
			if (error == true)
				return (printf("Error : invalid caracter in the map %s\n", line), false);
			map[i].is_wall = false;
			map[i].sprite[e_north].index = -1;
			map[i].sprite[e_east].index = -1;
			map[i].sprite[e_south].index = -1;
			map[i].sprite[e_west].index = -1;
			map[i].sprite[e_down].index = fill_texture(game->filename, game->nb_sprite, line[i], e_down);
			map[i].sprite[e_up].index = fill_texture(game->filename, game->nb_sprite, line[i], e_up);
		}
		i++;
	}
	while (i < map_size.x)
	{
		map[i].symbol = ' ';
		i++;
	}
	free(line);
	return (true);
}
