/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_existing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 16:27:03 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 16:32:38 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static bool	_is_two_specific_object(enum e_orientation orient,
				enum e_orientation orient1)
{
	return ((orient == e_object_entity || orient == e_object_wall
			|| orient == e_object_interactive || orient == e_door
			|| orient == e_door_lock || orient == e_exit
			|| orient == e_receptacle_empty)
		&& (orient1 == e_object_entity || orient1 == e_object_wall
			|| orient1 == e_object_interactive || orient1 == e_door
			|| orient1 == e_exit || orient1 == e_receptacle_empty
			|| orient1 == e_door_lock));
}

static bool	_is_non_wall(enum e_orientation orient)
{
	return (orient == e_floor || orient == e_ceiling || orient == e_door
		|| orient == e_object_wall || orient == e_object_entity
		|| orient == e_door_lock || orient == e_object_interactive
		|| orient == e_receptacle_empty || orient == e_exit);
}

static bool	_is_wall(enum e_orientation orient)
{
	return (orient == e_north || orient == e_east || orient == e_south
		|| orient == e_west || orient == e_wall);
}

static bool	_set_symbol(int index, char *symbol)
{
	if (index == e_floor || index == e_ceiling)
		*symbol = '0';
	else if (index == e_north || index == e_east || index == e_south
		|| index == e_west)
		*symbol = '1';
	else if (*symbol == '1' || *symbol == '0')
		return (false);
	return (true);
}

bool	is_existing(t_game *game, int index, char symbol,
				enum e_orientation orient)
{
	int		i;
	bool	is_wall;

	if (!_set_symbol(index, &symbol))
		return (true);
	is_wall = _is_wall(orient);
	i = -1;
	while (++i < game->nb_file)
	{
		if (orient == e_exit && game->filename[i].orient == e_exit)
			return (true);
		if (game->filename[i].symbol == symbol)
		{
			if (game->filename[i].orient == orient)
				return (true);
			else if (_is_two_specific_object(orient, game->filename[i].orient))
				return (true);
			else if ((is_wall == false) && _is_wall(game->filename[i].orient))
				return (true);
			else if (is_wall == true && _is_non_wall(game->filename[i].orient))
				return (true);
		}
	}
	return (false);
}
