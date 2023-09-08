/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:30:39 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/07 20:36:02 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

int	ft_close(t_game *game)
{
	int	i;

	i = 0;
	if (game->map != NULL)
		free_map((void *)game->map, game->map_size);
	if (game->player != NULL)
		free(game->player);
	if (game->filename != NULL)
		free_filename(game);
	free(game->dist_tab);
	if (game->mlx_ptr != NULL)
	{
		mlx_do_key_autorepeaton(game->mlx_ptr);
		i = 0;
		while (i < game->nb_images)
		{
			if (game->tab_images[i].img != NULL)
				mlx_destroy_image(game->mlx_ptr, game->tab_images[i].img);
			i++;
		}
		free(game->tab_images);
		if (game->image != NULL)
		{
			mlx_destroy_image(game->mlx_ptr, game->image->img);
			free(game->image);
		}
		if (game->alphabet != NULL)
		{
			if (game->alphabet->img != NULL)	
				mlx_destroy_image(game->mlx_ptr, game->alphabet->img);
			free(game->alphabet);
		}
		if (game->win != NULL)
			mlx_destroy_window(game->mlx_ptr, game->win);
		if (game->minimap != NULL)
		{
			if (game->minimap->image)
				mlx_destroy_image(game->mlx_ptr, game->minimap->image->img);
			if (game->minimap->back_img)
				mlx_destroy_image(game->mlx_ptr, game->minimap->back_img->img);
			if (game->minimap->buffer_img)
				mlx_destroy_image(game->mlx_ptr, game->minimap->buffer_img->img);
			if (game->minimap->player_img)
				mlx_destroy_image(game->mlx_ptr, game->minimap->player_img->img);
			free(game->minimap->image);
			free(game->minimap->back_img);
			free(game->minimap->buffer_img);
			free(game->minimap->player_img);
			free(game->minimap->bounds);
			free(game->minimap);
		}
		mlx_destroy_display(game->mlx_ptr);
		free(game->mlx_ptr);
	}
	if (nb_fps != 0)
		printf("Moyenne fps : %ld\n", (long)tot_fps / nb_fps);
	exit(0);
}
