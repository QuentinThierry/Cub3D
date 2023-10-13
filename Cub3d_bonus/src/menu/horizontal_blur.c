/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   horizontal_blur.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:16:34 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/13 21:18:53 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	init_left_column(int size_x, int size_y, int *h_buffer,
		t_pixel32 *pix_src)
{
	int			x;
	int			y;
	const int	size_x_3 = size_x * 3;

	y = 0;
	while (y < size_y)
	{
		h_buffer[y * size_x_3] = 0;
		h_buffer[y * size_x_3 + 1] = 0;
		h_buffer[y * size_x_3 + 2] = 0;
		x = 0;
		while (x <= SIZE_BOX_BLUR / 2)
		{
			h_buffer[y * size_x_3]
				+= ((pix_src[y * size_x + x] >> 16) & 0xff);
			h_buffer[y * size_x_3 + 1]
				+= ((pix_src[y * size_x + x] >> 8) & 0xff);
			h_buffer[y * size_x_3 + 2]
				+= ((pix_src[y * size_x + x]) & 0xff);
			x++;
		}
		y++;
	}
}

static void	compute_blur_pix(int x, int *h_buffer,
	t_pixel32 *pix_src, int size_x)
{
	h_buffer[x * 3]
		= h_buffer[x * 3 - 3];
	if (x + SIZE_BOX_BLUR / 2 < size_x)
		h_buffer[x * 3]
			+= ((pix_src[x + SIZE_BOX_BLUR / 2] >> 16) & 0xff);
	if (x - SIZE_BOX_BLUR / 2 - 1 >= 0)
		h_buffer[x * 3]
			-= ((pix_src[x - SIZE_BOX_BLUR / 2 - 1] >> 16) & 0xff);
	h_buffer[x * 3 + 1]
		= h_buffer[x * 3 - 2];
	if (x + SIZE_BOX_BLUR / 2 < size_x)
		h_buffer[x * 3 + 1]
			+= ((pix_src[x + SIZE_BOX_BLUR / 2] >> 8) & 0xff);
	if (x - SIZE_BOX_BLUR / 2 - 1 >= 0)
		h_buffer[x * 3 + 1]
			-= ((pix_src[x - SIZE_BOX_BLUR / 2 - 1] >> 8) & 0xff);
	h_buffer[x * 3 + 2]
		= h_buffer[x * 3 - 1];
	if (x + SIZE_BOX_BLUR / 2 < size_x)
		h_buffer[x * 3 + 2]
			+= ((pix_src[x + SIZE_BOX_BLUR / 2]) & 0xff);
	if (x - SIZE_BOX_BLUR / 2 - 1 >= 0)
		h_buffer[x * 3 + 2]
			-= ((pix_src[x - SIZE_BOX_BLUR / 2 - 1]) & 0xff);
}

void	horizontal_blur(t_image *src, int *h_buffer)
{
	int			x;
	int			y;
	int			size_x;
	int			size_y;
	t_pixel32	*pix_src;

	size_x = src->size.x;
	size_y = src->size.y;
	pix_src = (t_pixel32 *)src->addr;
	init_left_column(size_x, size_y, h_buffer, pix_src);
	y = 0;
	while (y < size_y)
	{
		x = 1;
		while (x < size_x)
		{
			compute_blur_pix(x, h_buffer, pix_src, size_x);
			x++;
		}
		h_buffer += src->size.x * 3;
		pix_src += size_x;
		y++;
	}
}
