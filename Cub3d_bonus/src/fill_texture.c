/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 16:34:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/01 19:29:58 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

static t_sprite	random_texture(t_texture *texture_tab, int index)
{
	int	nb;
	int	size;
	int	i;
	int	random;

	size = 0;
	i = 0;
	while (i < index)
	{
		size += texture_tab[i].total;
		i++;
	}
	if (texture_tab[index].filename != NULL)
		return ((t_sprite){size, -1, 0});
	nb = texture_tab[index].nb_file + texture_tab[index].nb_animation;
	random = 0;
	if (nb > 1)
		random = rand() % nb;
	if (random < texture_tab[index].nb_file)
		return ((t_sprite){size + random, -1, 0});
	size += random;
	random = rand() % (texture_tab[index].animation->nb_sprite - 1);
	printf("random frame : %d\n", random);
	return ((t_sprite){size, random, 0});
}

/**
 * @brief return the index of the texture in the filename_tab depending on the symbol
 * 		and the orientation
 * 
 * @param tab 
 * @param len 
 * @param symbol 
 * @param orient 
 * @return int 
 */
t_sprite	fill_texture(t_texture *tab, int len, char symbol, enum e_orientation orient)
{
	int	res;
	int	i;
	
	i = 0;
	res = -1;
	while (i < len)
	{
		if (tab[i].symbol == symbol)
		{
			if (tab[i].orient == orient)
				return (random_texture(tab, i));
			else if ((orient >= e_north && orient <= e_west) && tab[i].orient == e_wall)
				res = i;
		}
		i++;
	}
	return (random_texture(tab, res));
}