/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 20:34:15 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

__attribute__((always_inline))
static inline unsigned int	get_color_at(char *addr, int size_line,
		int x, int y)
{
	return (*(int *)(addr + (y * size_line + x * 4)));
}

__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos,
		int color)
{
	*(int *)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

static t_image	*get_draw_info(t_game *game, t_draw *draw, double height,
		t_fvector2 wall)
{
	enum e_orientation	orient;
	t_image				*image;

	draw->y_begin = WIN_Y / 2.0 - (int)(height / 2);
	draw->y_end = WIN_Y / 2.0 + ((int)height - (int)(height / 2));
	if (height != 0)
	{
		orient = get_wall_orientation(*(game->player), wall);
		image = game->tab_images[orient];
		draw->delta_y_img = image->size.y / height;
		if (draw->y_begin < 0)
		{
			draw->img.y = -draw->y_begin * draw->delta_y_img;
			draw->y_begin = 0;
		}
		if (draw->y_end > WIN_Y)
			draw->y_end = WIN_Y;
		if (orient == e_north || orient == e_south)
			draw->img.x = (wall.x - (int)wall.x) * image->size.x;
		else
			draw->img.x = (wall.y - (int)wall.y) * image->size.x;
		if (orient == e_west || orient == e_south)
			draw->img.x = image->size.x - draw->img.x - 1;
	}
	return (image);
}

void	draw_vert(t_game *game, int x, t_fvector2 wall, double height)
{
	t_draw		draw_info;
	t_image		*image;
	int			y_begin;
	t_fvector2	img;

	draw_info.img.y = 0;
	image = get_draw_info(game, &draw_info, height, wall);
	y_begin = draw_info.y_begin;
	img = draw_info.img;
	while (y_begin < draw_info.y_end)
	{
		my_mlx_pixel_put(game->image->addr, game->image->size_line,
			(t_vector2){x, y_begin},
			get_color_at(image->addr, image->size_line, img.x, img.y));
		img.y += draw_info.delta_y_img;
		y_begin++;
	}
}
