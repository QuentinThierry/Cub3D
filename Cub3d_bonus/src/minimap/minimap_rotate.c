/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:53:09 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/04 17:13:17 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_minimap_pos = 
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) - 
	(((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE) -
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD
};

__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, t_pixel32 color)
{
	*(int *)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

void	draw_rotated_image(t_image *img_dest, t_image *img_src, t_vector2 pos, float angle)
{
	t_vector2	coo;
	t_vector2	old_pos;
	t_vector2	image_size_div2;
	double		cos_angle;
	double		sin_angle;

	image_size_div2.x = img_src->size.x / 2;
	image_size_div2.y = img_src->size.y / 2;
	cos_angle = cos(angle);
	sin_angle = sin(angle);
	coo.y = 0;
	while (coo.y < img_src->size.y)
	{
		coo.x = 0;
		while (coo.x < img_src->size.x)
		{
			old_pos.x = (coo.x - image_size_div2.x) * cos_angle - (coo.y - image_size_div2.y) * sin_angle;
			old_pos.y = (coo.x - image_size_div2.x) * sin_angle + (coo.y - image_size_div2.y) * cos_angle;
			if (old_pos.x + image_size_div2.x < img_src->size.x && old_pos.x + image_size_div2.x >= 0
				&& old_pos.y + image_size_div2.y < img_src->size.y && old_pos.y + image_size_div2.y >= 0)
			{
				my_mlx_pixel_put(
					img_dest->addr, img_dest->size_line,
					(t_vector2){coo.x + pos.x, coo.y + pos.y},
					*(int *)(img_src->addr + ((old_pos.y + image_size_div2.y) * img_src->size_line + (old_pos.x + image_size_div2.x) * 4)));
			}
			coo.x++;
		}
		coo.y++;
	}
}

void	draw_minimap_buf_on_main_image(t_minimap *mmap, t_image *image)
{
	t_vector2	dest_pos;
	int			y;
	int			i;

	y = 0;
	i = mmap->image->size.y / 2 - 1;
	dest_pos = (t_vector2){g_minimap_pos.x, g_minimap_pos.y};
	while (y < mmap->buffer_img->size.y / 2)
	{
		ft_memcpy(
			image->addr + (dest_pos.y + y)
			* image->size_line + dest_pos.x * 4 + (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[y] * 4,
			mmap->buffer_img->addr + y * mmap->buffer_img->size_line + (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[y] * 4,
			mmap->bounds[y] * 4 * 2
			);
		ft_memcpy(
			image->addr + (dest_pos.y + y + mmap->buffer_img->size.y / 2)
			* image->size_line + dest_pos.x * 4 + (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[i] * 4,
			mmap->buffer_img->addr + (y + mmap->buffer_img->size.y / 2) * mmap->buffer_img->size_line + (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[i] * 4,
			mmap->bounds[i] * 4 * 2
			);
		i--;
		y++;
	}
}
