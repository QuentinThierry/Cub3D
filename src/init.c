/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/27 01:45:58 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	init_mlx(t_game *game)
{
	t_vector2	size;
	t_image		*img;
	
	if (THREED)
		size = (t_vector2){WIN_X, WIN_Y};
	else
		size = (t_vector2){game->map_size.x * CHUNK_SIZE, game->map_size.y * CHUNK_SIZE};
	img = ft_calloc(1, sizeof(t_image));
	if (img == NULL)
		return (-1);
	game->image = img;
	game->mlx_ptr = mlx_init();
	if (game->mlx_ptr == NULL)
		return (-1);
	game->win = mlx_new_window(game->mlx_ptr, size.x, size.y, "cub3d");
	if (game->win == NULL)
		return (-1);
	game->image->img = mlx_new_image(game->mlx_ptr, size.x, size.y);
	if (game->win == NULL)
		return (-1);
	game->image->addr = mlx_get_data_addr(game->image->img,
		&game->image->opp, &game->image->size_line, &game->image->endian);
	game->image->opp /= 8;
	if (game->win == NULL)
		return (-1);
	// mlx_do_key_autorepeatoff(game->mlx_ptr);
	return (0);
}

int	load_image(t_game *game)
{
	t_image		**tab_image;
	int			i;

	tab_image = ft_calloc(4, sizeof(t_image *));
	if (tab_image == NULL)
		return (-1);
	i = 0;
	while (i < 4)
	{
		tab_image[i] = ft_calloc(1, sizeof(t_image));
		if (tab_image[i] == NULL)
			return (-1);
		tab_image[i]->img = mlx_xpm_file_to_image(game->mlx_ptr, game->filename[i],
			 &(tab_image[i]->size.x), &(tab_image[i]->size.y));
		if (tab_image[i]->img == NULL)
			return (-1);
		tab_image[i]->addr = mlx_get_data_addr(tab_image[i]->img,
			&tab_image[i]->opp, &tab_image[i]->size_line, &tab_image[i]->endian);
		if (tab_image[i]->addr == NULL)
			return (-1);
		tab_image[i]->opp /= 8;
		i++;
	}
	game->tab_images = tab_image;
	return (0);
}
