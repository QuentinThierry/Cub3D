/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 16:00:18 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 13:04:51 by qthierry         ###   ########.fr       */
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
	int					i;
	bool				find_floor_ceiling;
	enum e_orientation	or;

	i = 0;
	find_floor_ceiling = false;
	*error = false;
	while (i < len)
	{
		if (tab[i].symbol == symbol)
		{
			or = tab[i].orient;
			if (or == e_wall || or == e_north || or == e_south || or == e_east
				|| or == e_west || or == e_door || or == e_object_wall
				|| or == e_receptacle_empty || or == e_receptacle_full
				|| or == e_exit || or == e_object_interactive_after
				|| or == e_object_interactive_before || or == e_door_lock)
				return (true);
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
