/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/13 00:28:15 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline unsigned int	get_color_at(char *addr, int size_line, t_vector2 pos)
{
	return (*(int*)(addr + (pos.y * size_line + pos.x * 4)));
}

__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, int color)
{
	*(int*)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

void	draw_vert(t_game *game, int x, t_ray ray, double height)
{
	register int		i = 0;
	int					y;
	int					y1;
	int					x_img;
	float				y_img = 0;
	t_image				*image;
	enum e_orientation	orient;
	float				delta_y_img;
	int					size_line;
	char				*addr;

	size_line = game->image->size_line;
	y = WIN_Y / 2.0 - (int)(height / 2);
	y1 = WIN_Y / 2.0 + ((int)height - (int)(height / 2));
	if (height != 0)
	{
		orient = ray.orient;
		image = get_image(game, orient, (t_fvector2){ray.hit.x, ray.hit.y});
		delta_y_img = image->size.y / height;
		if (y < 0)
		{
			y_img = -y * delta_y_img;
			y = 0;
		}
		if (y1 > WIN_Y)
			y1 = WIN_Y;
		if (orient == e_north || orient == e_south)
			x_img = (ray.hit.x - (int)ray.hit.x) * image->size.x;
		else
			x_img = (ray.hit.y - (int)ray.hit.y) * image->size.x;
		if (orient == e_west || orient == e_south)
			x_img = image->size.x - x_img - 1;
	}
	addr = game->image->addr;
	while (i < y)
	{
		my_mlx_pixel_put(addr, size_line, (t_vector2){x, i}, 0x666666);
		i++;
	}
	while (i < y1)
	{
		my_mlx_pixel_put(addr, size_line, (t_vector2){x, i}, get_color_at(image->addr, image->size_line, (t_vector2){x_img, y_img}));
		y_img += delta_y_img;
		i++;
	}
	while (i < WIN_Y)
	{
		my_mlx_pixel_put(addr, size_line, (t_vector2){x, i}, 0x222222);
		i++;
	}
}
