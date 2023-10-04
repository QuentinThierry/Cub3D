/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:56:51 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/04 18:31:54 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	load_image(t_game *game, t_image *img, char *filename,
		t_animation *anim)
{
	img->img = mlx_xpm_file_to_image(game->mlx_ptr, filename,
			&(img->size.x), &(img->size.y));
	if (img->img == NULL)
		return (false);
	img->addr = mlx_get_data_addr(img->img,
			&img->opp, &img->size_line, &img->endian);
	if (img->opp != 32)
		return (false);
	img->opp /= 8;
	if (!update_loading_screen(game, game->loading_screen))
		return (false);
	if (anim == NULL)
		return (true);
	img->time_animation = anim->time_animation;
	img->time_frame = anim->time_sprite;
	img->nb_total_frame = anim->nb_sprite - 1;
	return (true);
}

static t_image	*_resize_img(t_image *src,
					t_vector2 dst_size, t_image *dst)
{
	t_dvector2	ratio;
	int			x;
	int			y;

	ratio.x = (double)(src->size.x) / (double)(dst_size.x);
	ratio.y = (double)(src->size.y) / (double)(dst_size.y);
	y = -1;
	while (++y < dst_size.y)
	{
		x = -1;
		while (++x < dst_size.x)
		{
			*(unsigned int *)(dst->addr + (int)(y) *dst->size_line
					+ (int)(x) *(dst->opp))
				= *(unsigned int *)(src->addr + (int)(y * ratio.y)
					*src->size_line
					+ (int)(x * ratio.x) *(src->opp));
		}
	}
	return (dst);
}

bool	load_resize_image(t_game *game, t_image *img, char *filename,
		t_vector2 dst_size)
{
	t_image	dst;

	img->img = mlx_xpm_file_to_image(game->mlx_ptr, filename,
			&(img->size.x), &(img->size.y));
	if (img->img == NULL)
		return (false);
	img->addr = mlx_get_data_addr(img->img,
			&img->opp, &img->size_line, &img->endian);
	if (img->opp != 32)
		return (false);
	img->opp /= 8;
	if ((img->size.x == dst_size.x && img->size.y == dst_size.y)
		|| (dst_size.x == 0 || dst_size.y == 0))
		return (true);
	dst.img = mlx_new_image(game->mlx_ptr, dst_size.x, dst_size.y);
	if (!dst.img)
		return (false);
	dst.addr = mlx_get_data_addr(dst.img, &dst.opp, &dst.size_line,
			&dst.endian);
	if (dst.opp != 32)
		return (mlx_destroy_image(game->mlx_ptr, dst.img), false);
	dst.opp /= 8;
	dst.size = dst_size;
	dst = *_resize_img(img, dst_size, &dst);
	mlx_destroy_image(game->mlx_ptr, img->img);
	*img = dst;
	if (!update_loading_screen(game, game->loading_screen))
		return (false);
	return (true);
}

bool	load_image_tab(t_game *game, bool *print_error)
{
	t_image		*tab_image;
	int			i;
	int			j;
	int			h;
	int			index;

	*print_error = true;
	game->subtitle_font = btmlx_xpm_file_to_image_bilinear_resize(game->mlx_ptr,
			LOADING_FONT, (t_vector2){WIN_X / 3 * 2, WIN_Y / 16 / 3 * 2});
	if (game->subtitle_font == NULL)
		return (false);
	game->subtitle_size_letter.x = game->subtitle_font->size.x * WIDTH_LETTER / WIDTH_ALPHA;
	game->subtitle_size_letter.y = game->subtitle_font->size.y;
	game->nb_images = get_len_texture(game->filename, game->nb_file);
	game->tab_images = ft_calloc(game->nb_images, sizeof(t_image));
	if (game->tab_images == NULL)
		return (false);
	i = 0;
	index = 0;
	tab_image = game->tab_images;
	while (i < game->nb_file)
	{
		if (game->filename[i].filename != NULL)
		{
			if (game->filename[i].orient == e_object_interactive_hand)
			{
				if (!load_resize_image(game, &(tab_image[index]), game->filename[i].filename, (t_vector2){WIN_X / 3, WIN_X / 3}))
					return (false);
			}
			else if (!load_image(game, &(tab_image[index]), game->filename[i].filename, NULL))
				return (false);
			index++;
		}
		else
		{
			j = 0;
			while (j < game->filename[i].nb_file)
			{
				if (game->filename[i].orient == e_object_interactive_hand)
				{
					if (!load_resize_image(game, &(tab_image[index]), game->filename[i].filename_d[j], (t_vector2){WIN_X / 3, WIN_X / 3}))
						return (false);
				}
				else if (!load_image(game, &(tab_image[index]), game->filename[i].filename_d[j], NULL))
					return (false);
				j++;
				index++;
			}
			j = 0;
			while (j < game->filename[i].nb_animation)
			{
				h = 1;
				if (!ft_read_config(&(game->filename[i].animation[j]), 0))
					return (*print_error = false, false);
				while (h < game->filename[i].animation[j].nb_sprite)
				{
					if (game->filename[i].orient == e_object_interactive_hand)
					{
						if (!load_resize_image(game, &(tab_image[index]), game->filename[i].animation[j].filename[h], (t_vector2){WIN_X / 3, WIN_X / 3}))
							return (false);
						tab_image[index].time_animation = game->filename[i].animation[j].time_animation;
						tab_image[index].time_frame = game->filename[i].animation[j].time_sprite;
						tab_image[index].nb_total_frame = game->filename[i].animation[j].nb_sprite - 1;
					}
					else if (!load_image(game, &(tab_image[index]), game->filename[i].animation[j].filename[h], &(game->filename[i].animation[j])))
						return (false);
					index++;
					h++;
				}
				j++;
			}
		}
		i++;
	}
	return (true);
}
