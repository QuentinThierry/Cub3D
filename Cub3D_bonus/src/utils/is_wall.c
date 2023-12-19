/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_wall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:07:38 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/15 15:18:07 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

/**
 * @brief return true if the type is considered has a real wall with collision
 * 
 * @param type 
 * @return true 
 * @return false 
 */
bool	is_only_wall(unsigned int type)
{
	return ((type & WALL) == WALL && (type & OBJECT) != OBJECT
		&& (type & OBJ_INTER) != OBJ_INTER
		&& (type & RECEPTACLE) != RECEPTACLE);
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
