/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/19 19:05:26 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include <math.h>

void	print_map(char **maps)
{
	int y = 0;
	
	while(maps[y] != NULL)
	{
		printf("%s\n", maps[y]);
		y++;
	}
}

unsigned int	get_color_at(t_image *image, int x, int y)
{
	char	*dst;

	dst = image->addr
		+ (y * image->size_line + x * image->bpp / 8);
	return (*(unsigned int *)(dst));
}

void	my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->size_line + x * (img->bpp / 8));
	*(unsigned int*)dst = color;
}

// draw a vertical line on 'x' axis, from y1 up to y2 down
void	draw_vert(t_game *game, int x, int y1, int y2)
{
	int	y;

	y = y1;
	if (y < 0)
		y = 0;
	if (y2 > WIN_Y)
		y2 = WIN_Y;
	while (y <= y2)
	{
		my_mlx_pixel_put(game->image, x, y, 0xFF0000);
		y++;
	}
}

// draw a vertical line on 'x' axis, from y1 up to y2 down from the texture in game->asset
void	draw_vert_sprite(t_game *game, int x, t_fvector2 wall, float height)
{
	int	y, y1;
	int	x_img;
	float	y_img = 0;

	y =  WIN_Y / 2.0 - height / 2.0;
	y1 = WIN_Y / 2.0 + height / 2.0;
	if (y < 0)
		y = 0;
	if (y1 > WIN_Y)
		y1 = WIN_Y;
	if ((wall.x - (int)wall.x) * game->asset->size.x == 0)
		x_img = (wall.y - (int)wall.y) * game->asset->size.x;
	else
		x_img = (wall.x - (int)wall.x) * game->asset->size.x;
	while (y <= y1)
	{
		my_mlx_pixel_put(game->image, x, y, get_color_at(game->asset, x_img, (int)y_img));
		y++;
		y_img += game->asset->size.y / height;
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
			my_mlx_pixel_put(game->image, x, y, color);
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
			my_mlx_pixel_put(game->image, x, y, color);
			y++;
		}
		x += CHUNK_SIZE;
	}
}