/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/20 14:26:18 by jvigny           ###   ########.fr       */
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
		&game->image->bpp, &game->image->size_line, &game->image->endian);
	if (game->win == NULL)
		return (-1);
	// mlx_do_key_autorepeatoff(game->mlx_ptr);
	return (0);
}
