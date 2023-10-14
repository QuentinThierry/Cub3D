/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 18:39:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/14 15:58:39 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	fill_object_dist(t_game *game)
{
	int			i;
	t_dvector2	relative_pos;

	i = 0;
	while (i < game->nb_objects)
	{
		if (game->object_array[i]->visited)
		{
			relative_pos.x
				= (game->object_array[i]->map_pos.x - game->player->f_pos.x);
			relative_pos.y
				= (game->object_array[i]->map_pos.y - game->player->f_pos.y);
			game->object_array[i]->dist
				= sqrtf(relative_pos.x * relative_pos.x
					+ relative_pos.y * relative_pos.y);
		}
		else
			game->object_array[i]->dist = -1;
		i++;
	}
}

static void	sort_objects_distance(t_object **objects, int nb_object)
{
	int			i;
	int			j;
	t_object	*key;

	i = 1;
	while (i < nb_object)
	{
		j = i - 1;
		key = objects[i];
		while (j >= 0 && key->dist > objects[j]->dist && key->dist != -1)
		{
			objects[j + 1] = objects[j];
			j--;
		}
		objects[j + 1] = key;
		i++;
	}
}

void	draw_objects(t_game *game)
{
	int	i;

	i = 0;
	if (game->nb_objects != 0)
	{
		fill_object_dist(game);
		sort_objects_distance(game->object_array, game->nb_objects);
		while (i < game->nb_objects)
		{
			if (game->object_array[i]->visited)
				find_object_projection(game,
					game->object_array[i], game->player);
			game->object_array[i]->visited = false;
			i++;
		}
	}
}
