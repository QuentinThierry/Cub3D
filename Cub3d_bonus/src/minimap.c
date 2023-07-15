/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 00:30:38 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/15 18:39:05 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

#define MINIMAP_SIZEX 100
#define MINIMAP_SIZEY 100

#define MINIMAP_POSX 500
#define MINIMAP_POSY 500

static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, int color)
{
	*(int*)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

void	draw_horiz_line(t_image *image, int y, int x1, int x2)
{
	while (x1 < x2)
		my_mlx_pixel_put(image->addr, image->size_line, (t_vector2){x1++, y}, 0xFF0000);
}

void	generate_minimap_bounds(t_game *game)
{
	float	x;
	float	y;
	float	p;
	// int	x2;

	// y = 0;
	// while (y < MINIMAP_SIZEY)
	// {
	// 	x1 = MINIMAP_POSX;
	// 	x2 = MINIMAP_POSX + 360 * cos(y * TO_RADIAN);
	// 	draw_horiz_line(game->image, y, x1,x2);
	// 	y++;
	// }

	int a = 800;
	int b = 800;
	x = 0;
	y = 100;
	p = 1 - 100;
	while (x <= y)
	{
		if (p < 0)
		{
			x++;
			p = p + 2 * x + 1;
		}
		else
		{
			y--;
			x++;
			p = p - 2 * y + 2 * x + 1;
		}
		// draw_horiz_line(game->image, b+y, a-x, a+x);
		// draw_horiz_line(game->image, b-y, a-x, a+x);
		// draw_horiz_line(game->image, b+x - 1, a-y, a+y + 1);
		// draw_horiz_line(game->image, b-x, a-y, a+y);
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){a+x, b-y}, 0xFF0000);
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){a-x, b-y}, 0xFF0000);
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){a+y, b-x}, 0xFF0000);
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){a-y, b-x}, 0xFF0000);
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){a+x, b+y}, 0xFF0000);
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){a-x, b+y}, 0xFF0000);
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){a+y, b+x}, 0xFF0000);
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){a-y, b+x}, 0xFF0000);
	}
	

}