/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 16:00:18 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/04 16:00:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

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
			if (tab[i].orient == e_wall || tab[i].orient == e_north
				|| tab[i].orient == e_south || tab[i].orient == e_east
				|| tab[i].orient == e_west || tab[i].orient == e_door
				|| tab[i].orient == e_object_wall
				|| tab[i].orient == e_receptacle_empty
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
	int	i;

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
	int	i;

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
	int	i;

	i = 0;
	while (i < len)
	{
		if (tab[i].symbol == symbol
			&& (tab[i].orient == e_object_interactive_after
				|| tab[i].orient == e_object_interactive_before
				|| tab[i].orient == e_object_interactive
				|| tab[i].orient == e_object_interactive_hand))
			return (true);
		i++;
	}
	return (false);
}

bool	is_receptacle(char symbol, t_texture *tab, int len, char *c)
{
	int	i;

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