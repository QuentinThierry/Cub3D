/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/22 14:30:36 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

enum e_orientation	get_wall_orientation(t_dvector2 player, t_dvector2 wall)
{
	if ((wall.x - (int)wall.x) != 0)
	{
		if (player.y > wall.y)
		 	return (e_south);
		else
			return (e_north);
	}
	else
	{
		if (player.x > wall.x)
		 	return (e_east);
		else
			return (e_west);
	}
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

void remove_end_whitespace(char *str)
{
	int	i;

	i = ft_strlen(str) - 1;
	if (i <= 0)
		return ;
	while (i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' || str[i] == '\n'
			|| str[i] == '\f' || str[i] == '\r'))
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
 * @param error set on true if error occurs during the function NOT USE FOR THE MOMENT
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
			break;
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



/**
 * @brief return true if the symbol is a wall on the table of texture
 * 
 * @param symbol 
 * @param tab 
 * @param len 
 * @param error
 */
bool	is_wall(char symbol, t_texture *tab, int len, bool *error)
{
	int		i;
	bool	find_floor_ceiling;

	i = 0;
	find_floor_ceiling = false;
	*error = false;
	while (i < len)
	{
		if (tab[i].symbol == symbol)
		{
			if (tab[i].orient == e_wall || tab[i].orient == e_north || tab[i].orient == e_south
					|| tab[i].orient == e_east || tab[i].orient == e_west || tab[i].orient == e_door
					|| tab[i].orient == e_object_wall || tab[i].orient == e_receptacle_empty
					|| tab[i].orient == e_receptacle_full || tab[i].orient == e_exit
					|| tab[i].orient == e_object_interactive_after 
					|| tab[i].orient == e_object_interactive_before
					|| tab[i].orient == e_door_lock)
				return (true);
			else
				find_floor_ceiling = true;
		}
		i++;
	}
	if (find_floor_ceiling)
		return (false);
	return (*error = true, false);
}

bool	is_door(char symbol, t_texture *tab, int len, t_texture *type_door)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (tab[i].symbol == symbol && (tab[i].orient == e_door
			|| tab[i].orient == e_exit || tab[i].orient == e_door_lock))
		{
			*type_door = tab[i];
			return (true);
		}
		i++;
	}
	return (false);
}

bool	is_object(char symbol, t_texture *tab, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (tab[i].symbol == symbol && (tab[i].orient == e_object_entity 
			|| tab[i].orient == e_object_wall))
			return (true);
		i++;
	}
	return (false);
}

bool	is_object_interactive(char symbol, t_texture *tab, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (tab[i].symbol == symbol && (tab[i].orient == e_object_interactive_after 
			|| tab[i].orient == e_object_interactive_before || tab[i].orient == e_object_interactive
			|| tab[i].orient == e_object_interactive_hand))
			return (true);
		i++;
	}
	return (false);
}

bool	is_receptacle(char symbol, t_texture *tab, int len, char *c)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (tab[i].symbol == symbol && (tab[i].orient == e_receptacle_empty 
			|| tab[i].orient == e_receptacle_full))
		{
			*c = tab[i].symbol_receptacle;
			return (true);
		}
		i++;
	}
	return (false);
}

bool	is_in_map(t_dvector2 pos, t_map **map, t_vector2 size_map)
{
	if (pos.x < size_map.x && pos.x >= 0 && pos.y < size_map.y && pos.y >= 0
		&& map[(int)pos.y][(int)pos.x].symbol != ' ')
		return (true);
	return (false);
}

/**
 * @brief Get the number of different texture without the "config.cfg"
 * 
 * @param texture 
 * @param len 
 * @return int 
 */
int	get_len_texture(t_texture *texture, int len)
{
	int i;
	int j;
	int	res;

	res = 0;
	i = 0;
	while (i < len)
	{
		if (texture[i].filename != NULL)
			res++;
		else
		{
			res += texture[i].nb_file;
			j = 0;
			while (j < texture[i].nb_animation)
			{
				res += texture[i].animation[j].nb_sprite - 1;
				j++;
			}
		}
		i++;
	}
	return (res);
}

int	ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

t_object	*find_empty_object(t_game *game)
{
	int i;

	i = 0;
	while (i < game->nb_objects)
	{
		if (game->object_array[i]->map_pos.x == -1 && game->object_array[i]->map_pos.x == -1)
			return (game->object_array[i]);
		i++;
	}
	return (NULL);
}