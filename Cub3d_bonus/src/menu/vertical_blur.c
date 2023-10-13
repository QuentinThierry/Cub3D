/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertical_blur.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:40:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/13 21:18:51 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	init_first_row(int size_x, int *v_buffer,
	int *h_buffer, t_pixel32 *pix_dest)
{
	int			x;
	int			y;
	int			nb_pix;
	const int	size_x_3 = size_x * 3;

	x = -1;
	while (++x < size_x)
	{
		y = -1;
		while (++y <= SIZE_BOX_BLUR / 2)
		{
			v_buffer[x * 3]
				+= h_buffer[x * 3 + y * size_x_3];
			v_buffer[x * 3 + 1]
				+= h_buffer[x * 3 + y * size_x_3 + 1];
			v_buffer[x * 3 + 2]
				+= h_buffer[x * 3 + y * size_x_3 + 2];
		}
		nb_pix = SIZE_BOX_BLUR - (x - SIZE_BOX_BLUR / 2 - 1 < 0)
			* (-1 * (x - SIZE_BOX_BLUR / 2 - 1) - 1) \
	- (x + SIZE_BOX_BLUR / 2 >= size_x) * (x + SIZE_BOX_BLUR / 2 - size_x + 1);
		pix_dest[x] = ((v_buffer[x * 3] / (nb_pix * (SIZE_BOX_BLUR / 2 + 1))) \
	<< 16) | ((v_buffer[x * 3 + 1] / (nb_pix * (SIZE_BOX_BLUR / 2 + 1))) << 8)
			| (v_buffer[x * 3 + 2] / (nb_pix * (SIZE_BOX_BLUR / 2 + 1)));
	}
}

static void	_compute_blur_pix_red_green(t_vector2 xy, int *h_buffer,
	int *v_buffer, t_vector2 size_xy)
{
	const int	size_x_3 = size_xy.x * 3;

	v_buffer[xy.y * size_x_3]
		= v_buffer[(xy.y - 1) * size_x_3];
	if (xy.y + SIZE_BOX_BLUR / 2 < size_xy.y)
		v_buffer[xy.y * size_x_3]
			+= h_buffer[(xy.y + SIZE_BOX_BLUR / 2) * size_x_3];
	if (xy.y - SIZE_BOX_BLUR / 2 - 1 >= 0)
		v_buffer[xy.y * size_x_3]
			-= h_buffer[(xy.y - SIZE_BOX_BLUR / 2 - 1) * size_x_3];
	v_buffer[xy.y * size_x_3 + 1]
		= v_buffer[(xy.y - 1) * size_x_3 + 1];
	if (xy.y + SIZE_BOX_BLUR / 2 < size_xy.y)
		v_buffer[xy.y * size_x_3 + 1]
			+= h_buffer[(xy.y + SIZE_BOX_BLUR / 2) * size_x_3 + 1];
	if (xy.y - SIZE_BOX_BLUR / 2 - 1 >= 0)
		v_buffer[xy.y * size_x_3 + 1]
			-= h_buffer[(xy.y - SIZE_BOX_BLUR / 2 - 1) * size_x_3 + 1];
}

static t_pixel32	_compute_blur_pix(t_vector2 xy, int *h_buffer,
	int *v_buffer, t_vector2 size_xy)
{
	const int	size_x_3 = size_xy.x * 3;
	int			nb_pix;

	_compute_blur_pix_red_green(xy, h_buffer, v_buffer, size_xy);
	v_buffer[xy.y * size_x_3 + 2]
		= v_buffer[(xy.y - 1) * size_x_3 + 2];
	if (xy.y + SIZE_BOX_BLUR / 2 < size_xy.y)
		v_buffer[xy.y * size_x_3 + 2]
			+= h_buffer[(xy.y + SIZE_BOX_BLUR / 2) * size_x_3 + 2];
	if (xy.y - SIZE_BOX_BLUR / 2 - 1 >= 0)
		v_buffer[xy.y * size_x_3 + 2]
			-= h_buffer[(xy.y - SIZE_BOX_BLUR / 2 - 1) * size_x_3 + 2];
	nb_pix = (SIZE_BOX_BLUR - (xy.y + SIZE_BOX_BLUR / 2 >= size_xy.y)
			* (xy.y + SIZE_BOX_BLUR / 2 - size_xy.y + 1)
			- (xy.y - SIZE_BOX_BLUR / 2 - 1 < 0)
			* (-1 * (xy.y - SIZE_BOX_BLUR / 2 - 1) - 1))
		* (SIZE_BOX_BLUR - (xy.x - SIZE_BOX_BLUR / 2 - 1 < 0)
			* (-1 * (xy.x - SIZE_BOX_BLUR / 2 - 1) - 1)
			- (xy.x + SIZE_BOX_BLUR / 2 >= size_xy.x)
			* (xy.x + SIZE_BOX_BLUR / 2 - size_xy.x + 1));
	return (((v_buffer[xy.y * size_x_3] / nb_pix) << 16)
		| ((v_buffer[xy.y * size_x_3 + 1] / nb_pix) << 8)
		| (v_buffer[xy.y * size_x_3 + 2] / nb_pix));
}

static void	vertical_blur(t_image *dest,
		int *h_buffer, int *v_buffer)
{
	int			x;
	int			y;
	const int	size_y = dest->size.y;
	const int	size_x = dest->size.x;
	t_pixel32	*pix_dest;

	pix_dest = (t_pixel32 *)dest->addr;
	ft_bzero(v_buffer, WIN_X * 3 * sizeof(int));
	init_first_row(size_x, v_buffer, h_buffer, pix_dest);
	x = 0;
	while (x < size_x)
	{
		y = 1;
		while (y < size_y)
		{
			pix_dest[y * size_x + x]
				= _compute_blur_pix((t_vector2){x, y},
					h_buffer, v_buffer, (t_vector2){size_x, size_y});
			y++;
		}
		v_buffer += 3;
		h_buffer += 3;
		x++;
	}
}

// apply 2 times a box_blur to approximate a gaussian blur
void	blur_image(t_image *dest, t_image *src,
			int *h_buffer, int *v_buffer)
{
	horizontal_blur(src, h_buffer);
	vertical_blur(dest, h_buffer, v_buffer);
	horizontal_blur(dest, h_buffer);
	vertical_blur(src, h_buffer, v_buffer);
	ft_memcpy(dest->addr, src->addr, WIN_X * WIN_Y * 4);
}
