/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:53:09 by qthierry          #+#    #+#             */
/*   Updated: 2023/11/16 18:15:43 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_minimap_pos
	= (t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y)
	- (((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE)
	- ((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD
};

__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line,
	t_vector2 pos, t_pixel32 color)
{
	*(int *)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

/**
 * @brief return true if old_pos should be put on screen
 * 
 * @return t_vector2 
 */
bool	get_old_pos(t_vector2 *old_pos, t_vector2 coo,
				t_dvector2 cos_sin, t_image *img_src)
{
	if ((((cos_sin.x >= -0.00001 && cos_sin.x <= 0.00001)
				|| (cos_sin.x >= 0.99999 && cos_sin.x <= 1.00001)))
		&& ((cos_sin.y >= -0.00001 && cos_sin.y <= 0.00001)
			|| (cos_sin.y >= 0.99999 && cos_sin.y <= 1.00001)))
	{
		old_pos->x = (coo.x - img_src->size.x / 2.) * (int)(cos_sin.x + 0.5)
			- (coo.y - img_src->size.y / 2.) * (int)(cos_sin.y + 0.5);
		old_pos->y = (coo.x - img_src->size.x / 2.) * (int)(cos_sin.y + 0.5)
			+ (coo.y - img_src->size.y / 2.) * (int)(cos_sin.x + 0.5);
	}
	else
	{
		old_pos->x = (coo.x - img_src->size.x / 2.) * cos_sin.x
			- (coo.y - img_src->size.y / 2.) * cos_sin.y;
		old_pos->y = (coo.x - img_src->size.x / 2.) * cos_sin.y
			+ (coo.y - img_src->size.y / 2.) * cos_sin.x;
	}
	if (old_pos->x + img_src->size.x / 2. < img_src->size.x
		&& old_pos->x + img_src->size.x / 2. >= 0
		&& old_pos->y + img_src->size.y / 2. < img_src->size.y
		&& old_pos->y + img_src->size.y / 2. >= 0)
		return (true);
	return (false);
}

void	draw_rotated_image(t_image *img_dest,
			t_image *img_src, t_vector2 pos, float angle)
{
	t_vector2	coo;
	t_vector2	old_pos;
	double		cos_angle;
	double		sin_angle;

	cos_angle = cos(angle);
	sin_angle = sin(angle);
	coo.y = 0;
	while (coo.y < img_src->size.y)
	{
		coo.x = 0;
		while (coo.x < img_src->size.x)
		{
			if (get_old_pos(&old_pos, coo,
					(t_dvector2){cos_angle, sin_angle}, img_src))
				my_mlx_pixel_put(
					img_dest->addr, img_dest->size_line,
					(t_vector2){coo.x + pos.x, coo.y + pos.y},
					*((uint *)img_src->addr + ((old_pos.y + img_src->size.y / 2)
							* img_src->size.x
							+ old_pos.x + img_src->size.x / 2)));
			coo.x++;
		}
		coo.y++;
	}
}

void	draw_minimap_buf_on_main_image(t_minimap *mmap, t_image *image)
{
	int			y;
	int			i;

	y = 0;
	i = mmap->image->size.y / 2 - 1;
	while (y < mmap->buffer_img->size.y / 2)
	{
		ft_memcpy(
			image->addr + (g_minimap_pos.y + y)
			* image->size_line + g_minimap_pos.x * 4
			+ (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[y] * 4,
			mmap->buffer_img->addr + y * mmap->buffer_img->size_line
			+ (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[y] * 4,
			mmap->bounds[y] * 4 * 2);
		ft_memcpy(
			image->addr + (g_minimap_pos.y + y + mmap->buffer_img->size.y / 2)
			* image->size_line + g_minimap_pos.x * 4
			+ (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[i] * 4,
			mmap->buffer_img->addr + (y + mmap->buffer_img->size.y / 2)
			* mmap->buffer_img->size_line + (mmap->buffer_img->size.x / 2 * 4)
			- mmap->bounds[i] * 4, mmap->bounds[i] * 4 * 2);
		i--;
		y++;
	}
}
