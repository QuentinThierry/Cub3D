/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_image_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:56:51 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 18:37:36 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	load_resize_image(t_game *game, t_image *img, char *filename,
			t_vector2 dst_size);

bool	load_image(t_game *game, t_image *img, char *filename,
			t_animation *anim);

static const double	g_resize_letter = (1880 / 94.) / 1880;

static bool	_load_subtitle(t_game *game)
{
	game->subtitle_font = btmlx_xpm_file_to_image_bilinear_resize(game->mlx_ptr,
			LOADING_FONT, (t_vector2){WIN_X / 3 * 2, WIN_Y / 16 / 3 * 2});
	if (game->subtitle_font == NULL)
		return (false);
	game->subtitle_size.x = game->subtitle_font->size.x * g_resize_letter;
	game->subtitle_size.y = game->subtitle_font->size.y;
	return (true);
}

static bool	_load_animation(t_game *game, t_image *tab_image,
				t_animation *animation, int	*index)
{
	int	i;

	if (!ft_read_config(animation, 0))
		return (false);
	i = 1;
	while (i < animation->nb_sprite)
	{
		if (game->filename[i].orient == e_object_interactive_hand)
		{
			if (!load_resize_image(game, &(tab_image[*index]),
					animation->filename[i], (t_vector2){WIN_X / 3, WIN_X / 3}))
				return (print_error(NULL, 0), false);
			tab_image[*index].time_animation = animation->time_animation;
			tab_image[*index].time_frame = animation->time_sprite;
			tab_image[*index].nb_total_frame = animation->nb_sprite - 1;
		}
		else if (!load_image(game, &(tab_image[*index]), animation->filename[i],
				animation))
			return (print_error(NULL, 0), false);
		(*index)++;
		i++;
	}
	return (true);
}

static bool	_load_directory(t_game *game, t_image *tab_image,
				t_texture *texture, int	*index)
{
	int	i;

	i = 0;
	while (i < texture->nb_file)
	{
		if (texture->orient == e_object_interactive_hand)
		{
			if (!load_resize_image(game, &(tab_image[*index]),
					texture->filename_d[i], (t_vector2){WIN_X / 3, WIN_X / 3}))
				return (print_error(NULL, 0), false);
		}
		else if (!load_image(game, &(tab_image[*index]),
				texture->filename_d[i], NULL))
			return (print_error(NULL, 0), false);
		i++;
		(*index)++;
	}
	i = -1;
	while (++i < texture->nb_animation)
	{
		if (!_load_animation(game, tab_image, &texture->animation[i], index))
			return (false);
	}
	return (true);
}

static bool	_load_filename(t_game *game, t_image *tab_image,
				t_texture *texture, int	*index)
{
	if (texture->orient == e_object_interactive_hand)
	{
		if (!load_resize_image(game, &(tab_image[*index]), texture->filename,
				(t_vector2){WIN_X / 3, WIN_X / 3}))
			return (false);
	}
	else if (!load_image(game, &(tab_image[*index]), texture->filename, NULL))
		return (false);
	(*index)++;
	return (true);
}

bool	load_image_tab(t_game *game)
{
	t_image		*tab_image;
	int			i;
	int			index;

	_load_subtitle(game);
	game->nb_images = get_len_texture(game->filename, game->nb_file);
	game->tab_images = ft_calloc(game->nb_images, sizeof(t_image));
	if (game->tab_images == NULL)
		return (false);
	i = -1;
	index = 0;
	tab_image = game->tab_images;
	while (++i < game->nb_file)
	{
		if (game->filename[i].filename != NULL)
		{
			if (!_load_filename(game, tab_image, &game->filename[i], &index))
				return (print_error(NULL, 0), false);
		}
		else
			if (!_load_directory(game, tab_image, &game->filename[i], &index))
				return (false);
	}
	return (true);
}
