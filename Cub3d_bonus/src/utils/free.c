/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:27:51 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/25 20:37:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	free_filename(t_game *game)
{
	int i;
	int j;

	i = 0;
	while (i < game->nb_file)
	{
		if (game->filename[i].filename != NULL)
		{
			free(game->filename[i].filename);
			// game->filename[i].filename = NULL;
		}
		else
		{
			free_tab((void **)game->filename[i].filename_d, game->filename[i].nb_file);
			// game->filename[i].filename_d = NULL;
			j = 0;
			while (j < game->filename[i].nb_animation)
			{
				free_tab((void **)game->filename[i].animation[j].filename
					, game->filename[i].animation[j].nb_sprite);
				game->filename[i].animation[j].filename = NULL;
				j++;
			}
			free(game->filename[i].animation);
			// game->filename[i].animation = NULL;
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
