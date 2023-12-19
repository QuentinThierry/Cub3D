/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_pixels.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 18:53:56 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/15 14:06:37 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline unsigned int	get_color_at(char *addr,
	int size_line, t_vector2 pos)
{
	return (*(int *)(addr + (pos.y * size_line + pos.x * 4)));
}

__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line,
	t_vector2 pos, int color)
{
	*(int *)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

__attribute__((always_inline))
static inline unsigned int	dark_with_dist(unsigned int color,
	float dark_quantity)
{
	float	color_quantity;

	color_quantity = 1 - dark_quantity;
	return (((unsigned char)(((color >> 16) & 0xFF) * color_quantity
			+ ((FOG_COLOR >> 16) & 0xff) * dark_quantity) << 16)
		| ((unsigned char)(((color >> 8) & 0xFF) * color_quantity
		+ ((FOG_COLOR >> 8) & 0xff) * dark_quantity) << 8)
		| (unsigned char)((color & 0xFF) * color_quantity
		+ (FOG_COLOR & 0xff) * dark_quantity));
}

void	draw_pixel_dark(int end, t_image *image, t_vector2 pos_screen)
{
	while (pos_screen.y < end)
	{
		my_mlx_pixel_put(image->addr,
			image->size_line, pos_screen, FOG_COLOR);
		pos_screen.y++;
	}
}

void	draw_near_pixel(t_draw_infos *infos, t_image *image,
		int end, t_vector2 pos)
{
	if (infos->dark_quantity == 0)
	{
		while (pos.y < end)
		{
			my_mlx_pixel_put(image->addr, image->size_line, pos,
				get_color_at(infos->image->addr, infos->image->size_line,
					(t_vector2){infos->img_pos.x, infos->img_pos.y}));
			infos->img_pos.y += infos->delta_y_img;
			pos.y++;
		}
	}
	else
	{
		while (pos.y < end)
		{
			my_mlx_pixel_put(image->addr, image->size_line,
				pos, dark_with_dist(get_color_at(infos->image->addr,
						infos->image->size_line,
						(t_vector2){infos->img_pos.x, infos->img_pos.y}),
					infos->dark_quantity));
					infos->img_pos.y += infos->delta_y_img;
			pos.y++;
		}
	}
}
