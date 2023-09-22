/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blur.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:40:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/22 15:14:43 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	horizontal_blur(t_image *src, int *h_rgb_blur_buffer)
{
	int			x;
	int			y;
	int			size_x_3;
	int			size_x;
	int			size_y;
	t_pixel32	*pix_src;

	size_x = src->size.x;
	size_y = src->size.y;
	size_x_3 = src->size.x * 3;
	pix_src = (t_pixel32 *)src->addr;

	y = 0;
	while (y < size_y)
	{
		h_rgb_blur_buffer[y * size_x_3] = 0;
		h_rgb_blur_buffer[y * size_x_3 + 1] = 0;
		h_rgb_blur_buffer[y * size_x_3 + 2] = 0;
		x = 0;
		while (x <= SIZE_BOX_BLUR / 2)
		{
			h_rgb_blur_buffer[y * size_x_3] +=
				((pix_src[y * size_x + x] >> 16) & 0xff);
			h_rgb_blur_buffer[y * size_x_3 + 1] +=
				((pix_src[y * size_x + x] >> 8) & 0xff);
			h_rgb_blur_buffer[y * size_x_3 + 2] +=
				((pix_src[y * size_x + x]) & 0xff);
			x++;
		}
		y++;
	}
	y = 0;
	while (y < size_y)
	{
		x = 1;
		while (x < size_x)
		{
			h_rgb_blur_buffer[x * 3] =
				h_rgb_blur_buffer[x * 3 - 3];
			if (x + SIZE_BOX_BLUR / 2 < size_x)
				h_rgb_blur_buffer[x * 3] +=
					((pix_src[x + SIZE_BOX_BLUR / 2] >> 16) & 0xff);
			if (x - SIZE_BOX_BLUR / 2 - 1 >= 0)
				h_rgb_blur_buffer[x * 3] -=
					((pix_src[x - SIZE_BOX_BLUR / 2 - 1] >> 16) & 0xff);
			h_rgb_blur_buffer[x * 3 + 1] = 
				h_rgb_blur_buffer[x * 3 - 2];
			if (x + SIZE_BOX_BLUR / 2 < size_x)
				h_rgb_blur_buffer[x * 3 + 1] +=
					((pix_src[x + SIZE_BOX_BLUR / 2] >> 8) & 0xff);
			if (x - SIZE_BOX_BLUR / 2 - 1 >= 0)
				h_rgb_blur_buffer[x * 3 + 1] -=
					((pix_src[x - SIZE_BOX_BLUR / 2 - 1] >> 8) & 0xff);
			h_rgb_blur_buffer[x * 3 + 2] =
				h_rgb_blur_buffer[x * 3 - 1];
			if (x + SIZE_BOX_BLUR / 2 < size_x)
				h_rgb_blur_buffer[x * 3 + 2] +=
					((pix_src[x + SIZE_BOX_BLUR / 2]) & 0xff);
			if (x - SIZE_BOX_BLUR / 2 - 1 >= 0)
				h_rgb_blur_buffer[x * 3 + 2] -=
					((pix_src[x - SIZE_BOX_BLUR / 2 - 1]) & 0xff);
			x++;
		}
		h_rgb_blur_buffer += size_x_3;
		pix_src += size_x;
		y++;
	}
}

static void	vertical_blur(t_image *dest, int *h_rgb_blur_buffer, int *v_rgb_blur_buffer)
{
	int			x;
	int			y;
	const int	size_x_3 = dest->size.x * 3;
	const int	size_y = dest->size.y;
	const int	size_x = dest->size.x;
	t_pixel32	*pix_dest;
	int			nb_pixel_div;

	pix_dest = (t_pixel32 *)dest->addr;

	ft_bzero(v_rgb_blur_buffer, WIN_X * 3 * sizeof(int));
	x = 0;
	while (x < size_x)
	{
		y = 0;
		while (y <= SIZE_BOX_BLUR / 2)
		{
			v_rgb_blur_buffer[x * 3] += 
				h_rgb_blur_buffer[x * 3 + y * size_x_3];
			v_rgb_blur_buffer[x * 3 + 1] +=
				h_rgb_blur_buffer[x * 3 + y * size_x_3 + 1];
			v_rgb_blur_buffer[x * 3 + 2] +=
				h_rgb_blur_buffer[x * 3 + y * size_x_3 + 2];
			y++;
		}
		nb_pixel_div = SIZE_BOX_BLUR - (x - SIZE_BOX_BLUR / 2 - 1 < 0) * (-1 * (x - SIZE_BOX_BLUR / 2 - 1) - 1)
				- (x + SIZE_BOX_BLUR / 2 >= size_x) * (x + SIZE_BOX_BLUR / 2 - size_x + 1);
		pix_dest[x] =
			((v_rgb_blur_buffer[x * 3] / (nb_pixel_div * (SIZE_BOX_BLUR / 2 + 1))) << 16) |
			((v_rgb_blur_buffer[x * 3 + 1] / (nb_pixel_div * (SIZE_BOX_BLUR / 2 + 1))) << 8) |
			(v_rgb_blur_buffer[x * 3 + 2] / (nb_pixel_div * (SIZE_BOX_BLUR / 2 + 1)));
		x++;
	}
	x = 0;
	while (x < size_x)
	{
		y = 1;
		while (y < size_y)
		{
			v_rgb_blur_buffer[y * size_x_3] = 
				v_rgb_blur_buffer[(y - 1) * size_x_3];
			if (y + SIZE_BOX_BLUR / 2 < size_y)
				v_rgb_blur_buffer[y * size_x_3] +=
					h_rgb_blur_buffer[(y + SIZE_BOX_BLUR / 2) * size_x_3];
			if (y - SIZE_BOX_BLUR / 2 - 1 >= 0)
				v_rgb_blur_buffer[y * size_x_3] -=
					h_rgb_blur_buffer[(y - SIZE_BOX_BLUR / 2 - 1) * size_x_3];
			v_rgb_blur_buffer[y * size_x_3 + 1] = 
				v_rgb_blur_buffer[(y - 1) * size_x_3 + 1];
			if (y + SIZE_BOX_BLUR / 2 < size_y)
				v_rgb_blur_buffer[y * size_x_3 + 1] +=
					h_rgb_blur_buffer[(y + SIZE_BOX_BLUR / 2) * size_x_3 + 1];
			if (y - SIZE_BOX_BLUR / 2 - 1 >= 0)
				v_rgb_blur_buffer[y * size_x_3 + 1] -=
					h_rgb_blur_buffer[(y - SIZE_BOX_BLUR / 2 - 1) * size_x_3 + 1];
			v_rgb_blur_buffer[y * size_x_3 + 2] = 
				v_rgb_blur_buffer[(y - 1) * size_x_3 + 2];
			if (y + SIZE_BOX_BLUR / 2 < size_y)
				v_rgb_blur_buffer[y * size_x_3 + 2] +=
					h_rgb_blur_buffer[(y + SIZE_BOX_BLUR / 2) * size_x_3 + 2];
			if (y - SIZE_BOX_BLUR / 2 - 1 >= 0)
				v_rgb_blur_buffer[y * size_x_3 + 2] -=
					h_rgb_blur_buffer[(y - SIZE_BOX_BLUR / 2 - 1) * size_x_3 + 2];
			nb_pixel_div = (SIZE_BOX_BLUR - (y + SIZE_BOX_BLUR / 2 >= size_y) * (y + SIZE_BOX_BLUR / 2 - size_y + 1)
				- (y - SIZE_BOX_BLUR / 2 - 1 < 0) * (-1 * (y - SIZE_BOX_BLUR / 2 - 1) - 1))
				* (SIZE_BOX_BLUR - (x - SIZE_BOX_BLUR / 2 - 1 < 0) * (-1 * (x - SIZE_BOX_BLUR / 2 - 1) - 1)
				- (x + SIZE_BOX_BLUR / 2 >= size_x) * (x + SIZE_BOX_BLUR / 2 - size_x + 1)
			);
			pix_dest[y * size_x + x] =
				((v_rgb_blur_buffer[y * size_x_3] / nb_pixel_div) << 16) |
				((v_rgb_blur_buffer[y * size_x_3 + 1] / nb_pixel_div) << 8) |
				(v_rgb_blur_buffer[y * size_x_3 + 2] / nb_pixel_div);
			y++;
		}
		v_rgb_blur_buffer += 3;
		h_rgb_blur_buffer += 3;
		x++;
	}
}

// apply 2 times a box_blur to approximate a gaussian blur
void	blur_image(t_image *dest, t_image *src,
			int *h_rgb_blur_buffer, int *v_rgb_blur_buffer)
{
	horizontal_blur(src, h_rgb_blur_buffer);
	vertical_blur(dest, h_rgb_blur_buffer, v_rgb_blur_buffer);
	horizontal_blur(dest, h_rgb_blur_buffer);
	vertical_blur(src, h_rgb_blur_buffer, v_rgb_blur_buffer);
	ft_memcpy(dest->addr, src->addr, WIN_X * WIN_Y * 4);
}
