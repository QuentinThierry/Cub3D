/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/27 00:57:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

enum e_orientation	get_wall_orientation(t_player player, t_fvector2 wall)
{
	if (fmodf(wall.x, CHUNK_SIZE) != 0)
	{
		if (player.pos.y > wall.y)
			return (e_north);
		else
		 	return (e_south);
	}
	else
	{
		if (player.pos.x > wall.x)
			return (e_west);
		else
		 	return (e_east);
	}
}

t_image	*get_image(t_game	*game, enum e_orientation orient)
{
	return (game->tab_images[orient]);
}
