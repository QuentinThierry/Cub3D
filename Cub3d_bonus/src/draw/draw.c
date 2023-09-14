/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/14 20:41:34 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

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

__attribute__((always_inline))
static inline unsigned int	dark_with_dist(unsigned int color, float dark_quantity)
{
	float	color_quantity;

	color_quantity = 1 - dark_quantity;
	return (((unsigned char)(((color >> 16) & 0xFF) * color_quantity + ((DARK_COLOR >> 16) & 0xff) * dark_quantity) << 16)
		| ((unsigned char)(((color >> 8) & 0xFF) * color_quantity + ((DARK_COLOR >> 8) & 0xff) * dark_quantity) << 8)
		| (unsigned char)((color & 0xFF) * color_quantity + (DARK_COLOR & 0xff) * dark_quantity));
}

void	draw_vert(t_game *game, int x, t_ray ray, double height)
{
	register int		i = 0;
	int					y;
	int					y1;
	int					x_img;
	int					x_door;
	float				y_img = 0;
	t_image				*image;
	enum e_orientation	orient;
	float				delta_y_img;
	int					size_line;
	char				*addr;
	float				dark_quantity;
	
	if (game->dist_tab[x] >= DIST_MIN_DARK)
		dark_quantity = (-DIST_MIN_DARK + game->dist_tab[x]) / (DIST_MAX_DARK - DIST_MIN_DARK);
	else
		dark_quantity = 0;
	size_line = game->image->size_line;
	if (((int)height & 1) == 1)
		height++;
	y = WIN_Y / 2.0 - (int)(height / 2);
	y1 = WIN_Y / 2.0 + ((int)height - (int)(height / 2));
	if (height != 0)
	{
		image = get_image_wall(game, ray, &x_door);
		if (image->addr == NULL)
			return (printf("Error : Invalid image\n"),(void)ft_close(game));
		delta_y_img = image->size.y / height;
		if (y < 0)
		{
			y_img = -y * delta_y_img;
			y = 0;
		}
		if (y1 > WIN_Y)
			y1 = WIN_Y;
		orient = ray.orient;
		if (orient == e_north || orient == e_south)
			x_img = (ray.hit.x - (int)ray.hit.x) * image->size.x;
		else
			x_img = (ray.hit.y - (int)ray.hit.y) * image->size.x;
		if (orient == e_west || orient == e_south)
			x_img = image->size.x - x_img - 1;
		if (x_door != -1)
			x_img = x_door;
	}
	addr = game->image->addr;
	i = y;
	if (dark_quantity >= 1)
	{
		while (i < y1)
		{
			my_mlx_pixel_put(addr, size_line, (t_vector2){x, i}, DARK_COLOR);
			y_img += delta_y_img;
			i++;
		}
	}
	else if (dark_quantity == 0)
	{
		while (i < y1)
		{
			my_mlx_pixel_put(addr, size_line, (t_vector2){x, i}, get_color_at(image->addr, image->size_line, (t_vector2){x_img, y_img}));
			y_img += delta_y_img;
			i++;
		}
	}
	else
	{
		while (i < y1)
		{
			my_mlx_pixel_put(addr, size_line, (t_vector2){x, i}, dark_with_dist(get_color_at(image->addr, image->size_line, (t_vector2){x_img, y_img}), dark_quantity));
			y_img += delta_y_img;
			i++;
		}
	}
}
