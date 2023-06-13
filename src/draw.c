/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/13 17:26:33 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	print_map(char **maps)
{
	int y = 0;
	
	while(maps[y] != NULL)
	{
		printf("%s\n", maps[y]);
		y++;
	}
}

void	my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->data + (y * img->size_line + x * (img->bpp / 8));
	*(unsigned int*)dst = color;
}

// draw a vertical line on 'x' axis, from y1 up to y2 down
void	draw_vert(t_game *game, int x, int y1, int y2)
{
	int	y;

	y = y1;
	while (y < y2)
	{
		my_mlx_pixel_put(&game->image, x, y, 0xFF0000);
		y++;
	}
}


void	quadrillage(t_game *game)
{
	int	x;
	int	y;
	unsigned color;
	
	y = 0;
	while (y < WIN_Y)
	{
		x = 0;
		while (x < WIN_X)
		{
			if (game->maps[y / CHUNK_SIZE][x / CHUNK_SIZE] == '1'
				|| game->maps[(y-1) / CHUNK_SIZE][(x-1) / CHUNK_SIZE] == '1')
				color = 0x404040;
			else
				color = 0x202020;
			my_mlx_pixel_put(&game->image, x, y, color);
			x++;
		}
		y += CHUNK_SIZE;
	}
	x = 0;
	while (x < WIN_X)
	{
		y = 0;
		while (y < WIN_Y)
		{
			if (game->maps[y / CHUNK_SIZE][x / CHUNK_SIZE] == '1'
				|| game->maps[(y-1) / CHUNK_SIZE][(x-1) / CHUNK_SIZE] == '1')
				color = 0x404040;
			else
				color = 0x202020;
			my_mlx_pixel_put(&game->image, x, y, color);
			y++;
		}
		x += CHUNK_SIZE;
	}
}