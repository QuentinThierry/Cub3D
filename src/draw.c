/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/22 21:27:38 by qthierry         ###   ########.fr       */
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
// void	draw_vert(t_game *game, int x, int y1, int y2)
// {
// 	int	y;

// 	y = y1;
// 	if (y < 0)
// 		y = 0;
// 	if (y2 > game->map_size.y)
// 		y2 = game->map_size.y;
// 	while (y <= y2)
// 	{
// 		my_mlx_pixel_put(game->image, x, y, 0xFF0000);
// 		y++;
// 	}
// }

// draw a vertical line on 'x' axis, from y1 up to y2 down from the texture in game->asset
void	draw_vert_sprite(t_game *game, int x, t_fvector2 wall, float height)
{
	float	y, y1;
	int		x_img;
	float	y_img = 0;
	// height = rintf(height);

	y =  WIN_Y / 2.0 - height / 2.0;
	y1 = WIN_Y / 2.0 + height / 2.0;
	if (y < 0)
		y_img = -y * game->asset->size.y / height;
	if (y < 0)
		y = 0;
	if (y1 > WIN_Y)
		y1 = WIN_Y;
	// printf("height : %f		bas y: %f	haut y:%f	taille : %f\n", height, y , y1, y1 - y);
	wall.y = (int)wall.y;
	if (fmodf(wall.x, CHUNK_SIZE) * game->asset->size.x == 0)
		x_img = fmodf(wall.y, CHUNK_SIZE) * game->asset->size.x;
	else
		x_img = fmodf(wall.x, CHUNK_SIZE)* game->asset->size.x;
	while (y < y1)
	{
		my_mlx_pixel_put(game->image, x, (int)y, get_color_at(game->asset, x_img / CHUNK_SIZE, (int)y_img));
		y+=1.0;
		y_img += game->asset->size.y / height;
	}
}
