/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_object_door.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 16:51:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 17:09:07 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static bool	_init_tab(t_game *game)
{
	if (game->nb_objects != 0)
	{
		game->object_array = ft_calloc(game->nb_objects, sizeof(t_object *));
		if (!game->object_array)
			return (free_map_object(game->map, game->map_size), false);
	}
	if (game->nb_doors != 0)
	{
		game->door_array = ft_calloc(game->nb_doors, sizeof(t_map *));
		if (!game->door_array)
			return (free_map_object(game->map, game->map_size), false);
	}
	return (true);
}

static bool	_complete_object_tab(t_game *game, int cpt)
{
	while (cpt < game->nb_objects)
	{
		game->object_array[cpt] = ft_calloc(1, sizeof(t_object));
		if (game->object_array[cpt] == NULL)
			return (print_error(NULL, 0), false);
		game->object_array[cpt]->dist = -1;
		game->object_array[cpt]->map_pos = (t_dvector2){-1, -1};
		cpt++;
	}
	return (true);
}

static void	_set_object(t_object **object, t_vector2 pos, int *cpt, void *arg)
{
	*object = arg;
	(*object)->map_pos
		= (t_dvector2){pos.x + 0.5f, pos.y + 0.5f};
	(*cpt)++;
}

static void	_set_door(t_map **door, t_vector2 pos, int *cpt, t_game *game)
{
	*door = &game->map[pos.y][pos.x];
	((t_door *)(*door)->arg)->map_pos = pos;
	(*cpt)++;
}

bool	fill_object_and_doors(t_game *game)
{
	t_vector2	pos;
	int			cpt_objects;
	int			cpt_doors;

	if (!_init_tab(game))
		return (false);
	cpt_objects = 0;
	cpt_doors = 0;
	pos.y = -1;
	while (++pos.y < game->map_size.y)
	{
		pos.x = -1;
		while (++pos.x < game->map_size.x)
		{
			if ((game->map[pos.y][pos.x].type & OBJECT) == OBJECT)
				_set_object(&game->object_array[cpt_objects], pos, &cpt_objects,
					game->map[pos.y][pos.x].arg);
			else if ((game->map[pos.y][pos.x].type & DOOR) == DOOR)
				_set_door(&game->door_array[cpt_doors], pos, &cpt_doors, game);
		}
	}
	return (_complete_object_tab(game, cpt_objects));
}
