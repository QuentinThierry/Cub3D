/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:30:39 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/06 01:09:14 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	ft_close(t_game *game)
{
	// if (game->tab_image != NULL)
		
	if (game->maps != NULL)
		free_tab(game->maps, game->map_size);
	if (game->player != NULL)
		free(game->player);
	if (game->filename)
		free_tab(game->filename, (t_vector2){4, 4});
	if (game->mlx_ptr != NULL)
	{
		mlx_do_key_autorepeaton(game->mlx_ptr);
		if (game->win != NULL)
		{
			if (game->image != NULL)
			{
				mlx_destroy_image(game->mlx_ptr, game->image->img);
				free(game->image);
			}
			mlx_destroy_window(game->mlx_ptr, game->win);
		}
		mlx_destroy_display(game->mlx_ptr);
	}
	printf("Moyenne fps : %ld\n", (long)tot_fps / nb_fps);
	exit(0);
}