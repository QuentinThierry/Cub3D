/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:30:39 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/05 15:35:25 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	free_image(void *mlx_ptr, t_image *image)
{
	if (image == NULL)
		return ;
	if (image->img != NULL && mlx_ptr != NULL)
		mlx_destroy_image(mlx_ptr, image->img);
	free(image);
}

void	free_menu(void *mlx_ptr, t_menu *menu)
{
	if (!menu)
		return ;
	free_image(mlx_ptr, menu->background_image);
	free_image(mlx_ptr, menu->exit_option_image);
	free_image(mlx_ptr, menu->image);
	free(menu->h_rgb_blur_buffer);
	free(menu->v_rgb_blur_buffer);
	free_image(mlx_ptr, menu->option_menu.buttons[0].base_image);
	free_image(mlx_ptr, menu->option_menu.buttons[0].hovered_image);
	free_image(mlx_ptr, menu->option_menu.exit_opt_button.base_image);
	free_image(mlx_ptr, menu->option_menu.slider_fov.hor_image);
	free_image(mlx_ptr, menu->option_menu.slider_fov.vert_image);
	free_image(mlx_ptr, menu->option_menu.slider_sound.hor_image);
	free_image(mlx_ptr, menu->option_menu.slider_sound.vert_image);
	free(menu);
}

int	ft_close(t_game *game)
{
	int	i;

	close_audio(game->music_array);
	if (game->file_music != NULL)
		free_music_file(game->file_music, game->nb_music);
	if (game->object_array != NULL)
		free_tab_object(game->object_array, game->nb_objects);
	if (game->player != NULL)
		free(game->player);
	if (game->map != NULL)
		free_map((void *)game->map, game->map_size);
	if (game->filename != NULL)
		free_filename(game);
	free(game->dist_tab);
	free(game->height_tab);
	free(game->door_array);
	free(game->keybinds);
	if (game->filename)
		free_filename(game);
	free_minimap(game->minimap, game->mlx_ptr);
	free_menu(game->mlx_ptr, game->menu);
	free_loading_screen(game);
	free_image(game->mlx_ptr, game->font);
	free_image(game->mlx_ptr, game->subtitle_font);
	free_image(game->mlx_ptr, game->image);
	if (game->end != NULL)
		free_image(game->mlx_ptr, game->end->end_screen);
	free(game->end);
	if (game->mlx_ptr)
	{
		mlx_do_key_autorepeaton(game->mlx_ptr);
		i = 0;
		while (game->tab_images != NULL && i < game->nb_images)
		{
			if (game->tab_images[i].img != NULL)
				mlx_destroy_image(game->mlx_ptr, game->tab_images[i].img);
			i++;
		}
		free(game->tab_images);
		if (game->win != NULL)
			mlx_destroy_window(game->mlx_ptr, game->win);
		mlx_destroy_display(game->mlx_ptr);
		free(game->mlx_ptr);
	}
	if (nb_fps != 0)
		printf("Moyenne fps : %ld\n", (long)tot_fps / nb_fps);
	exit(0);
}
