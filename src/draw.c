/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/27 01:59:32 by jvigny           ###   ########.fr       */
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

static inline unsigned int	get_color_at(t_image *image, int x, int y)
{
	char	*dst;

	dst = image->addr
		+ (y * image->size_line + x * image->opp);
	return (*(unsigned int *)(dst));
}

void	my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->size_line + x * img->opp);
	*(unsigned int*)dst = color;
}

void	draw_vert_sprite(t_game *game, int x, t_fvector2 wall, float height)
{
	int		y, y1;
	int		x_img;
	float	y_img = 0;
	t_image	*image;
	enum e_orientation	orient;
	float	delta_y_img;
	float	wallx_mod;

	orient = get_wall_orientation(*(game->player), wall);
	image = get_image(game, orient);
	y = WIN_Y / 2.0 - height / 2.0;
	y1 = WIN_Y / 2.0 + height / 2.0;
	delta_y_img = image->size.y / height;
	if (y < 0)
	{
		y_img = -y * delta_y_img;
		y = 0;
	}
	if (y1 > WIN_Y)
		y1 = WIN_Y;
	wallx_mod = fmodf(wall.x, CHUNK_SIZE);
	if (wallx_mod == 0)
		x_img = fmodf(wall.y, CHUNK_SIZE) * image->size.x / CHUNK_SIZE;
	else
		x_img = wallx_mod * image->size.x / CHUNK_SIZE;
	printf("x_img : %d	%d	res : %d\n", x_img, image->size.x / CHUNK_SIZE, image->size.x - x_img - 1);
	if (orient == e_west || orient == e_south)
		x_img = image->size.x - x_img - 1;
	while (y < y1)
	{
		my_mlx_pixel_put(game->image, x, y, get_color_at(image, x_img, y_img));
		y_img += delta_y_img;
		y++;
	}
}
