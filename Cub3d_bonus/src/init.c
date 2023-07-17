/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/17 21:36:14 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	init_mlx(t_game *game)
{
	t_vector2	size;
	
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
	game->image->size = size;
	game->image->opp /= 8;
	if (game->win == NULL)
		return (-1);
	return (0);
}

void	init_mouse(t_game *game)
{
	mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
	game->player->mouse_pos.x = WIN_X / 2;
	game->player->mouse_pos.y = WIN_Y / 2;
	mlx_mouse_hide(game->mlx_ptr, game->win);
}

bool	load_image(void *mlx_ptr, t_image *tab_img, int	i, char *filename)
{
	printf("name : %s\n", filename);
	tab_img[i].img = mlx_xpm_file_to_image(mlx_ptr, filename,
			&(tab_img[i].size.x), &(tab_img[i].size.y));
	if (tab_img[i].img == NULL)
		return (false);
	tab_img[i].addr = mlx_get_data_addr(tab_img[i].img,
		&tab_img[i].opp, &tab_img[i].size_line, &tab_img[i].endian);
	if (tab_img[i].addr == NULL)
		return (false);
	tab_img[i].opp /= 8;
	return (true);
}

bool	load_image_tab(t_game *game)
{
	t_image		*tab_image;
	int			i;
	int			j;
	int			h;
	int			index;

	h = get_len_texture(game->filename, game->nb_sprite);
	game->tab_images = ft_calloc(h, sizeof(t_image));
	if (game->tab_images == NULL)
		return (-1);
	i = 0;
	index = 0;
	tab_image = game->tab_images;
	while (i < game->nb_sprite)
	{
		if (game->filename[i].filename != NULL)
		{
			if (!load_image(game->mlx_ptr, tab_image, index, game->filename[i].filename))
				return (false);
		}
		else
		{
			j = 0;
			while (j < game->filename[i].nb_file)
			{
				if (!load_image(game->mlx_ptr, tab_image, index, game->filename[i].filename_d[j]))
					return (false);
				j++;
				index++;
			}
			j = 0;
			while (j < game->filename[i].nb_animation)
			{
				h = 0;
				while (h < game->filename[i].animation[j].nb_sprite - 1)
				{
					if (!load_image(game->mlx_ptr, tab_image, index, game->filename[i].animation[j].filename[h]))
						return (false);
					index++;
					h++;
				}
				ft_read_config(&(game->filename[i].animation[j]), h);
				j++;
			}
		}
		i++;
	}
	return (true);
}
