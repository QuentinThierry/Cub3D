/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 22:06:01 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/08 15:40:56 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static inline void	draw_image_alpha(t_vector2 dim,
	unsigned int *dest_addr, unsigned int *src_addr, t_vector2 size_line)
{
	int	x;
	int	y;
	unsigned int	color;

	y = 0;
	while (y < dim.y)
	{
		x = 0;
		while (x < dim.x)
		{
			color = *(src_addr + x + y * size_line.y);
			if (color == GREEN_SCREEN)
			{
				x++;
				continue ;
			}
			*(dest_addr + x + y * size_line.x) = color;
			x++;
		}
		y++;
	}
}

void	draw_image_on_image_alpha(t_image *dest,
	t_image *src, t_vector2 offset_dest)
{
	unsigned int	*src_addr;
	unsigned int	*dest_addr;
	int				src_size_line;
	int				dest_size_line;
	t_vector2		dim;

	if (src->size.x < dest->size.x - offset_dest.x)
		dim.x = src->size.x;
	else
		dim.x = dest->size.x - offset_dest.x;
	if (src->size.y < dest->size.y - offset_dest.y)
		dim.y = src->size.y;
	else
		dim.y = dest->size.y - offset_dest.y;
	dest_size_line = dest->size_line / 4;
	src_size_line = src->size_line / 4;
	dest_addr = (unsigned *)dest->addr
		+ offset_dest.y * dest_size_line + offset_dest.x;
	src_addr = (unsigned *)src->addr;
	draw_image_alpha(dim, dest_addr, src_addr,
		(t_vector2){dest_size_line, src_size_line});
}
