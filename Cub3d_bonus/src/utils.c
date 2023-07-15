/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/15 23:23:32 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

enum e_orientation	get_wall_orientation(t_player player, t_fvector2 wall)
{
	if ((wall.x - (int)wall.x) != 0)
	{
		if (player.f_real_pos.y > wall.y)
		 	return (e_south);
		else
			return (e_north);
	}
	else
	{
		if (player.f_real_pos.x > wall.x)
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
	// if (index == -1)
	// {
	// 	printf("Error : get image wall %f	%f\n", wall.x, wall.y);
	// 	exit(1);
	// }
	return (game->tab_images[index]);
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

bool	is_symbol_map(char c)
{
	return (c == ' ' || c == '0' || c == '1' || c == 'o' || c == 'c'
		|| c == 'N' || c == 'E' || c == 'S' || c == 'W');
}

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
			else if ((orient == e_down || orient == e_up) && tab[i].orient == e_none)
				res = i;
			else if ((orient >= e_north && orient <= e_west) && tab[i].orient == e_wall)
				res = i;
		}
		i++;
	}
	if (res == -1)
		return (orient);
	return (res);
}

bool	is_wall(char symbol, t_texture *tab, int len)
{
	int i;

	i = 0;
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
	return (false);
}
