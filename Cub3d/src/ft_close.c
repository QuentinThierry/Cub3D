/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:30:39 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 19:56:08 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	destroy_all_images(t_game *game)
{
	int	i;

	if (game->image)
	{
		if (game->mlx_ptr && game->image->img)
			mlx_destroy_image(game->mlx_ptr, game->image->img);
		free(game->image);
	}
	if (game->tab_images)
	{
		i = 0;
		while (i < 6)
		{
			if (game->mlx_ptr && game->tab_images[i]
				&& game->tab_images[i]->img)
				mlx_destroy_image(game->mlx_ptr, game->tab_images[i]->img);
			free(game->tab_images[i]);
			i++;
		}
		free(game->tab_images);
	}
}

void	free_tab(char **str, int sizey)
{
	int	i;

	i = 0;
	while (i < sizey)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	ft_close(t_game *game)
{
	int	i;

	if (game->mlx_ptr && game->win)
		mlx_destroy_window(game->mlx_ptr, game->win);
	destroy_all_images(game);
	if (game->map)
	{
		i = 0;
		while (i < game->map_size.y)
			free(game->map[i++]);
		free(game->map);
	}
	free(game->player);
	if (game->filename)
		free_tab(game->filename, 4);
	if (game->mlx_ptr)
		mlx_destroy_display(game->mlx_ptr);
	free(game->mlx_ptr);
	exit(0);
}
