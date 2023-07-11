/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/11 03:31:45 by qthierry         ###   ########.fr       */
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
	
	game->image = ft_calloc(1, sizeof(t_image));
	if (game->image == NULL)
		return (-1);
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
	return (0);
}

void	fill_rectangle(char *addr, int size_x, int size_y, unsigned int color)
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

bool	load_floor_ceiling_img(t_image **tab_image, t_game *game)
{
	tab_image[e_ceiling] = ft_calloc(1, sizeof(t_image));
	if (!tab_image[e_ceiling])
		return (false);
	tab_image[e_ceiling]->img = mlx_new_image(game->mlx_ptr, WIN_X, WIN_Y / 2);
	if (!tab_image[e_ceiling]->img)
		return (false);
	tab_image[e_ceiling]->addr = mlx_get_data_addr(tab_image[e_ceiling]->img,
		&tab_image[e_ceiling]->opp, &tab_image[e_ceiling]->size_line, &tab_image[e_ceiling]->endian);
	fill_rectangle(tab_image[e_ceiling]->addr, WIN_X, WIN_Y / 2, game->ceiling);
	tab_image[e_floor] = ft_calloc(1, sizeof(t_image));
	if (!tab_image[e_floor])
		return (false);
	tab_image[e_floor]->img = mlx_new_image(game->mlx_ptr, WIN_X, WIN_Y / 2);
	if (!tab_image[e_floor]->img)
		return (false);
	tab_image[e_floor]->addr = mlx_get_data_addr(tab_image[e_floor]->img,
		&tab_image[e_floor]->opp, &tab_image[e_floor]->size_line, &tab_image[e_floor]->endian);
	fill_rectangle(tab_image[e_floor]->addr, WIN_X, WIN_Y / 2, game->floor);
	return (true);
}


int	load_image(t_game *game)
{
	t_image		**tab_image;
	int			i;

	tab_image = ft_calloc(6, sizeof(t_image *));
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
	if (!load_floor_ceiling_img(tab_image, game))
		return (-1);
	game->tab_images = tab_image;
	return (0);
}
