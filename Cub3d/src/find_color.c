/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_color.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:58:50 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/11 17:48:48 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	_get_color(char *str, int *i, bool *error)
{
	int	tmp;

	tmp = 0;
	*error = false;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		tmp = tmp * 10 + str[*i] - '0';
		if (tmp > 255)
			return (print_error("Color not between 0 and 255\n", 1),
				*error = true, 0);
		(*i)++;
	}
	return (tmp);
}

static void	set_color(t_game *game, char texture, unsigned int color)
{
	if (texture == 'F')
		game->floor_color = color;
	else
		game->ceiling_color = color;
}

bool	find_color(char *str, t_game *game, char texture)
{
	int				nb_color;
	unsigned int	color;
	int				i;
	bool			error;

	nb_color = 0;
	color = 0;
	i = skip_whitespace(str);
	while (str[i] != 0)
	{
		color = (color << 8) + _get_color(str, &i, &error);
		if (error == true)
			return (false);
		nb_color++;
		i += skip_whitespace(str + i);
		if (str[i] == '\0' || nb_color == 3)
			break ;
		if (str[i] != ',')
			return (print_error("wrong format of color\n", 1), false);
		i += 1 + skip_whitespace(str + i + 1);
	}
	if (nb_color != 3 || str[i] != '\0')
		return (print_error("Wrong format of color\n", 1), false);
	set_color(game, texture, color);
	return (true);
}
