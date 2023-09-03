/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/03 19:34:35 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

int	init_mlx(t_game *game)
{
	game->image = ft_calloc(1, sizeof(t_image));
	if (game->image == NULL)
		return (-1);
	game->mlx_ptr = mlx_init();
	if (game->mlx_ptr == NULL)
		return (-1);
	game->win = mlx_new_window(game->mlx_ptr, WIN_X, WIN_Y, "cub3d");
	if (game->win == NULL)
		return (-1);
	game->image->img = mlx_new_image(game->mlx_ptr, WIN_X, WIN_Y);
	if (game->win == NULL)
		return (-1);
	game->image->addr = mlx_get_data_addr(game->image->img,
		&game->image->opp, &game->image->size_line, &game->image->endian);
	if (game->image->opp != 32)
		return (-1); // If mlx returns a number of plane different that 4, stop the program
	game->image->size = (t_vector2){WIN_X, WIN_Y};
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

bool	load_image(void *mlx_ptr, t_image *img, char *filename, t_animation *anim)
{
	// printf("name : %s\n", filename);
	img->img = mlx_xpm_file_to_image(mlx_ptr, filename,
			&(img->size.x), &(img->size.y));
	if (img->img == NULL)
		return (false);
	img->addr = mlx_get_data_addr(img->img,
		&img->opp, &img->size_line, &img->endian);
	if (img->addr == NULL)
		return (false);
	img->opp /= 8;
	if (anim == NULL)
		return (true);
	img->time_animation = anim->time_animation;
	img->time_frame = anim->time_sprite;
	img->nb_total_frame = anim->nb_sprite - 1;
	return (true);
}

bool	load_image_tab(t_game *game)
{
	t_image		*tab_image;
	int			i;
	int			j;
	int			h;
	int			index;

	game->nb_images = get_len_texture(game->filename, game->nb_file);
	game->tab_images = ft_calloc(game->nb_images, sizeof(t_image));
	if (game->tab_images == NULL)
		return (-1);
	i = 0;
	index = 0;
	tab_image = game->tab_images;
	while (i < game->nb_file)
	{
		if (game->filename[i].filename != NULL)
		{
			if (!load_image(game->mlx_ptr, &(tab_image[index]), game->filename[i].filename, NULL))
				return (false);
			index++;
		}
		else
		{
			j = 0;
			while (j < game->filename[i].nb_file)
			{
				if (!load_image(game->mlx_ptr, &(tab_image[index]), game->filename[i].filename_d[j], NULL))
					return (false);
				j++;
				index++;
			}
			j = 0;
			while (j < game->filename[i].nb_animation)
			{
				h = 1;
				if (!ft_read_config(&(game->filename[i].animation[j]), 0))
					return (false);
				while (h < game->filename[i].animation[j].nb_sprite)
				{
					if (!load_image(game->mlx_ptr, &(tab_image[index]), game->filename[i].animation[j].filename[h], &(game->filename[i].animation[j])))
						return (false);
					index++;
					h++;
				}
				j++;
			}
		}
		i++;
	}
	return (true);
}
