/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/29 18:25:16 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

enum e_orientation	get_wall_orientation(t_player player, t_fvector2 wall)
{
	if ((wall.x - (int)wall.x) != 0)
	{
		if (player.f_real_pos.y > wall.y)
			return (e_north);
		else
		 	return (e_south);
	}
	else
	{
		if (player.f_real_pos.x > wall.x)
			return (e_west);
		else
		 	return (e_east);
	}
}

t_image	*get_image(t_game	*game, enum e_orientation orient)
{
	return (game->tab_images[orient]);
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