/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:30:39 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/11 03:34:27 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	ft_close(t_game *game)
{
	int	i;

	i = 0;
	if (game->map != NULL)
		free_tab(game->map, game->map_size);
	if (game->player != NULL)
		free(game->player);
	if (game->filename)
		free_tab(game->filename, (t_vector2){4, 4});
	if (game->mlx_ptr != NULL)
	{
		while (i < 6)
		{
			mlx_destroy_image(game->mlx_ptr, game->tab_images[i]->img);
			free(game->tab_images[i]);
			i++;
		}
		if (game->image != NULL)
		{
			mlx_destroy_image(game->mlx_ptr, game->image->img);
			free(game->image);
		}
		free(game->tab_images);
		if (game->win)
			mlx_destroy_window(game->mlx_ptr, game->win);
		mlx_destroy_display(game->mlx_ptr);
		free(game->mlx_ptr);
	}
	printf("Moyenne fps : %ld\n", (long)tot_fps / nb_fps);
	exit(0);
}