/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:30:39 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/21 16:24:54 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	ft_close(t_game *game)
{
	int	i;

	i = 0;
	if (game->map != NULL)
		free_tab((void *)game->map, game->map_size.y);
	if (game->player != NULL)
		free(game->player);
	if (game->filename != NULL)
		free_filename(game);
	if (game->mlx_ptr != NULL)
	{
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
		if (game->win != NULL)
			mlx_destroy_window(game->mlx_ptr, game->win);
		mlx_destroy_display(game->mlx_ptr);
		free(game->mlx_ptr);
	}

	printf("Moyenne fps : %ld\n", (long)tot_fps / nb_fps);
	exit(0);
}
