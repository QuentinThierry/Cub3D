/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 16:34:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/29 12:24:40 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static int	get_nb_sprite_animation(int nb_animation, t_texture *texture)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (i < nb_animation)
	{
		res += texture->animation[i].nb_sprite - 1;
		i++;
	}
	return (res);
}

static int	_get_size(int index, t_texture *texture_tab)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (i < index)
	{
		size += texture_tab[i].total;
		i++;
	}
	return (size);
}

static t_sprite	random_texture(t_texture *texture_tab, int index,
				bool is_random)
{
	int	nb;
	int	size;
	int	random;

	if (index == -1)
		return ((t_sprite){-1, -1, 0});
	size = _get_size(index, texture_tab);
	if (texture_tab[index].filename != NULL)
		return ((t_sprite){size, -1, 0});
	nb = texture_tab[index].nb_file + texture_tab[index].nb_animation;
	random = 0;
	if (is_random == true)
	{
		if (nb > 1)
			random = rand() % nb;
		if (random < texture_tab[index].nb_file)
			return ((t_sprite){size + random, -1, 0});
		size += texture_tab[index].nb_file + get_nb_sprite_animation(random
				- texture_tab[index].nb_file, &texture_tab[index]);
		return ((t_sprite){size, 0, 0});
	}
	return ((t_sprite){size, 0, 0});
}

static void	_set_orient_ref(t_texture *tab, enum e_orientation orient, int i,
				int *orient_ref)
{
	if (tab->orient == orient && (orient == e_north || orient == e_east
			|| orient == e_south || orient == e_west) && tab->symbol == '1')
		*orient_ref = i;
	else if (tab->orient == orient && (orient == e_floor || orient == e_ceiling)
		&& tab->symbol == '0')
		*orient_ref = i;
}

/**
 * @brief return the index of the texture in the filename_tab depending
 * 		on the symbol and the orientation
 * 
 * @param tab 
 * @param len 
 * @param symbol 
 * @param orient 
 * @return int 
 */
t_sprite	fill_texture(t_texture *tab, int len, char symbol,
			enum e_orientation orient)
{
	int	symbol_ref;
	int	orient_ref;
	int	i;

	i = 0;
	symbol_ref = -1;
	orient_ref = -1;
	while (i < len)
	{
		if (tab[i].symbol == symbol)
		{
			if (tab[i].orient == orient && orient == e_exit)
				return (random_texture(tab, i, false));
			else if (tab[i].orient == orient)
				return (random_texture(tab, i, true));
			else if ((orient == e_north || orient == e_east || orient == e_south
					|| orient == e_west) && tab[i].orient == e_wall)
				symbol_ref = i;
		}
		_set_orient_ref(&tab[i], orient, i, &orient_ref);
		i++;
	}
	if (symbol_ref == -1)
		symbol_ref = orient_ref;
	return (random_texture(tab, symbol_ref, true));
}
