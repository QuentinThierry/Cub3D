/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/29 19:54:16 by qthierry         ###   ########.fr       */
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
	int					y, y1;
	int					x_img;
	float				y_img = 0;
	t_image				*image;
	enum e_orientation	orient;
	float				delta_y_img;

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

	if (orient == e_north || orient == e_south)
		x_img = (wall.x - (int)wall.x) * image->size.x;
	else
		x_img = (wall.y - (int)wall.y) * image->size.x;
	if (orient == e_west || orient == e_south)
		x_img = image->size.x - x_img - 1;
	int		i = 0;
	while (i < y)
	{
		my_mlx_pixel_put(game->image, x, i, 0x880000);
		i++;
	}
	while (y < y1)
	{
		my_mlx_pixel_put(game->image, x, y, get_color_at(image, x_img, y_img));
		y_img += delta_y_img;
		y++;
	}
	while (y1 < WIN_Y)
	{
		my_mlx_pixel_put(game->image, x, y1, 0x008800);
		y1++;
	}
}
