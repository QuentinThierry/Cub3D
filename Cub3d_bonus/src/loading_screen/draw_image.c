/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:10:38 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 15:11:13 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline t_pixel32	_get_pix_alpha(t_pixel32 dest, t_pixel32 src)
{
	float	alpha;

	alpha = ((src >> 24) & 0xff) / 255.;
	return (((unsigned char)(((src >> 16) & 0xFF) * alpha
			+ ((dest >> 16) & 0xff) * (1 - alpha)) << 16)
		| ((unsigned char)(((src >> 8) & 0xFF) * alpha
			+ ((dest >> 8) & 0xff) * (1 - alpha)) << 8)
		| (unsigned char)((src & 0xFF) * alpha
			+ (dest & 0xff) * (1 - alpha)));
}

/**
 * @brief 
 * ! NO protection if draw outside of the window
 * 
 * @param dest_addr 
 * @param src 
 * @param begin_src 
 * @param size_src 
 */
void	draw_image_with_transparence(char *dest_addr, t_image *src
			, t_vector2 begin_src, t_vector2 size_src)
{
	int	y;
	int	x;
	int	start_dest;
	int	start_src;

	y = 0;
	start_src = begin_src.y * src->size_line + begin_src.x * 4;
	start_dest = 0;
	while (y < size_src.y)
	{
		x = 0;
		while (x < size_src.x * 4)
		{
			*(int *)(dest_addr + start_dest + x) = _get_pix_alpha(
					*(int *)(dest_addr + start_dest + x),
					*(int *)(src->addr + start_src + x));
			x += 4;
		}
		start_dest += WIN_X * 4;
		start_src += (src->size.x) * 4;
		y++;
	}
}

/**
 * @brief 
 * ! NO protection if draw outside of the window
 * 
 * @param dest_addr 
 * @param src 
 * @param begin_src 
 * @param size_src 
 */
void	draw_image_with_green_sreen(char *dest_addr, t_image *src
			, t_vector2 begin_src, t_vector2 size_src)
{
	int	y;
	int	x;
	int	start_dest;
	int	start_src;

	y = 0;
	start_src = begin_src.y * src->size_line + begin_src.x * 4;
	start_dest = 0;
	while (y < size_src.y)
	{
		x = 0;
		while (x < size_src.x * 4)
		{
			if (*(int *)(src->addr + start_src + x) != GREEN_SCREEN)
				*(int *)(dest_addr + start_dest + x) = *(int *)(src->addr
						+ start_src + x);
			x += 4;
		}
		start_dest += WIN_X * 4;
		start_src += (src->size.x) * 4;
		y++;
	}
}
