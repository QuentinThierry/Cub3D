/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 18:35:21 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static bool	_init_mlx(t_game *game)
{
	game->mlx_ptr = mlx_init();
	if (!game->mlx_ptr)
		return (false);
	game->win = mlx_new_window(game->mlx_ptr, WIN_X, WIN_Y, "cub3D");
	if (!game->win)
		return (false);
	game->image = ft_calloc(1, sizeof(t_image));
	if (game->image == NULL)
		return (false);
	game->image->img = mlx_new_image(game->mlx_ptr, WIN_X, WIN_Y);
	if (game->image->img == NULL)
		return (false);
	bettermlx_get_data_addr(game->image, (t_vector2){WIN_X, WIN_Y});
	return (true);
}

static void	fill_rectangle(char *addr, int size_x, int size_y,
		unsigned int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < size_y)
	{
		x = 0;
		while (x < size_x)
		{
			*(unsigned int *)addr = color;
			addr += 4;
			x++;
		}
		y++;
	}
}

static bool	load_floor_ceiling_img(t_image **tab_image, t_game *game)
{
	tab_image[e_ceiling] = ft_calloc(1, sizeof(t_image));
	if (!tab_image[e_ceiling])
		return (false);
	tab_image[e_ceiling]->img = mlx_new_image(game->mlx_ptr, WIN_X, WIN_Y / 2);
	if (!tab_image[e_ceiling]->img)
		return (false);
	bettermlx_get_data_addr(tab_image[e_ceiling],
		(t_vector2){WIN_X, WIN_Y / 2});
	fill_rectangle(tab_image[e_ceiling]->addr, WIN_X, WIN_Y / 2,
		game->ceiling_color);
	tab_image[e_floor] = ft_calloc(1, sizeof(t_image));
	if (!tab_image[e_floor])
		return (false);
	tab_image[e_floor]->img = mlx_new_image(game->mlx_ptr, WIN_X, WIN_Y / 2);
	if (!tab_image[e_floor]->img)
		return (false);
	bettermlx_get_data_addr(tab_image[e_floor], (t_vector2){WIN_X, WIN_Y / 2});
	fill_rectangle(tab_image[e_floor]->addr, WIN_X, WIN_Y / 2,
		game->floor_color);
	return (true);
}

static bool	_load_image(t_game *game)
{
	int	i;

	game->tab_images = ft_calloc(6, sizeof(t_image *));
	if (!game->tab_images)
		return (false);
	i = 0;
	while (i < 4)
	{
		game->tab_images[i] = ft_calloc(1, sizeof(t_image));
		if (!game->tab_images[i])
			return (false);
		game->tab_images[i]->img = mlx_xpm_file_to_image(game->mlx_ptr,
				game->filename[i], &(game->tab_images[i]->size.x),
				&(game->tab_images[i]->size.y));
		if (!game->tab_images[i]->img)
			return (false);
		bettermlx_get_data_addr(game->tab_images[i], game->tab_images[i]->size);
		i++;
	}
	if (!load_floor_ceiling_img(game->tab_images, game))
		return (false);
	return (true);
}

bool	init_all(t_game *game)
{
	if (!_init_mlx(game))
		return (print_error(NULL, 0), false);
	if (!_load_image(game))
		return (print_error(NULL, 0), false);
	return (true);
}
