/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:30:39 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/26 19:19:49 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

int	ft_close(t_game *game)
{
	int	i;

	close_audio(game->music_array);
	if (game == NULL)
		exit(0);
	if (game->file_music != NULL)
		free_music_file(game->file_music, game->nb_music);
	if (game->end != NULL)
	{
		if (game->end->end_screen != NULL)
		{
			if (game->end->end_screen->img != NULL)
				mlx_destroy_image(game->mlx_ptr, game->end->end_screen->img);
			free(game->end->end_screen);
		}
		free(game->end);
	}
	if (game->object_array != NULL)
		free_tab_object(game->object_array, game->nb_objects);
	if (game->player != NULL)
		free(game->player);
	if (game->map != NULL)
		free_map((void *)game->map, game->map_size);
	if (game->filename != NULL)
		free_filename(game);
	free(game->dist_tab);
	free(game->door_array);
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
		if (game->font != NULL)
		{
			if (game->font->img != NULL)	
				mlx_destroy_image(game->mlx_ptr, game->font->img);
			free(game->font);
		}
		if (game->win != NULL)
			mlx_destroy_window(game->mlx_ptr, game->win);
		free_minimap(game->minimap, game->mlx_ptr);
		if (game->image != NULL)
		{
			if (game->image->img != NULL)
				mlx_destroy_image(game->mlx_ptr, game->image->img);
			free(game->image);
		}
		mlx_destroy_display(game->mlx_ptr);
		free(game->mlx_ptr);
	}
	if (nb_fps != 0)
		printf("Moyenne fps : %ld\n", (long)tot_fps / nb_fps);
	exit(0);
}
