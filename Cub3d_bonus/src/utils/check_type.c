/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 16:00:18 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 13:07:58 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

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
