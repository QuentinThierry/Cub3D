/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:27:51 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 21:41:02 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"
#include "../../includes/audio.h"

void	free_filename(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->nb_file)
	{
		if (game->filename[i].filename != NULL)
		{
			free(game->filename[i].filename);
		}
		else
		{
			if (game->filename[i].filename_d != NULL)
				free_tab((void **)game->filename[i].filename_d,
					game->filename[i].nb_file);
			j = 0;
			while (j < game->filename[i].nb_animation)
			{
				free_tab((void **)game->filename[i].animation[j].filename,
					game->filename[i].animation[j].nb_sprite);
				game->filename[i].animation[j].filename = NULL;
				j++;
			}
			free(game->filename[i].animation);
		}
		i++;
	}
	free(game->filename);
	game->filename = NULL;
}

void	free_tab(void **str, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_minimap(t_minimap *minimap, void *mlx_ptr)
{
	if (minimap)
	{
		free_image(mlx_ptr, minimap->image);
		free_image(mlx_ptr, minimap->back_img);
		free_image(mlx_ptr, minimap->buffer_img);
		free_image(mlx_ptr, minimap->player_img);
		free(minimap->bounds);
		free(minimap);
	}
}

void	free_music_file(t_music_name *music_tab, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (music_tab[i].filename != NULL)
			free(music_tab[i].filename);
		if (music_tab[i].subtitle != NULL)
			free(music_tab[i].subtitle);
		i++;
	}
	free(music_tab);
}

void	free_str(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}
