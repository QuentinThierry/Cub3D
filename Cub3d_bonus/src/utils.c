/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/17 19:10:51 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

enum e_orientation	get_wall_orientation(t_fvector2 player, t_fvector2 wall)
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

t_image	*get_image(t_game *game, enum e_orientation orient, t_fvector2 wall)
{
	int	index;
	
	if (orient == e_south)
		index = game->map[(int)wall.y - 1][(int)wall.x].sprite[orient].index;
	else if (orient == e_north || orient == e_west)
		index = game->map[(int)wall.y][(int)wall.x].sprite[orient].index;
	if (orient == e_east)
		index = game->map[(int)wall.y][(int)wall.x - 1].sprite[orient].index;
	return (&(game->tab_images[index]));
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

bool	check_symbol(char *str)
{
	int	len;
	int	i;

	i = 0;
	len = strlen(str);
	while (i < len)
	{
		if (i == len - 1 && str[len - 1] == '\n')
			str[len - 1] = '\0';
		else if (!(str[i] == ' ' || str[i] == '0' || str[i] == '1' || str[i] == 'o'
				|| str[i] == 'c' || str[i] == 'N' || str[i] == 'E'
				|| str[i] == 'S' || str[i] == 'W'))
			return (false);
		i++;
	}
	return (true);
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
		// if (!check_symbol(line))
		// 	*error = true;
		else if (len.x < (int)strlen(line))
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

/**
 * @brief return the index of the texture in the filename_tab depending on the symbol
 * 		and the orientation
 * 
 * @param tab 
 * @param len 
 * @param symbol 
 * @param orient 
 * @return int 
 */
int	fill_texture(t_texture *tab, int len, char symbol, enum e_orientation orient)
{
	int i;
	int	res;
	
	i = 0;
	res = -1;
	while (i < len)
	{
		if (tab[i].symbol == symbol)
		{
			if (tab[i].orient == orient)
				return (i);
			else if ((orient >= e_north && orient <= e_west) && tab[i].orient == e_wall)
				res = i;
		}
		i++;
	}
	return (res);
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
	int i;

	i = 0;
	*error = false;
	while (i < len)
	{
		if (tab[i].symbol == symbol)
		{
			if (tab[i].orient == e_wall || tab[i].orient == e_north || tab[i].orient == e_south
					|| tab[i].orient == e_east || tab[i].orient == e_west)
				return (true);
			else
				return (false);
		}
		i++;
	}
	return (*error = true, false);
}

int	get_len_texture(t_texture *texture, int len)
{
	int i;
	int j;
	int x;
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
				res += texture[i].animation[j].nb_sprite;
				j++;
			}
		}
		i++;
	}
	return (res);
}
