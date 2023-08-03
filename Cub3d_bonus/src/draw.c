/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/03 19:45:51 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

static inline unsigned int	get_color_at(char *addr, int size_line, t_vector2 pos)
{
	return (*(int*)(addr + (pos.y * size_line + pos.x * 4)));
}

static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, int color)
{
	*(int*)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

void	draw_vert(t_game *game, int x, t_fvector2 wall, double height)
{
	int					y, y1;
	int					x_img;
	double				y_img = 0;
	t_image				*image;
	enum e_orientation	orient;
	double				delta_y_img;
	int					size_line;
	int					i = 0;
	char				*addr;

	size_line = game->image->size_line;
	y = WIN_Y / 2.0 - (int)(height / 2);
	y1 = WIN_Y / 2.0 + ((int)height - (int)(height / 2));
	if (height != 0)
	{
		orient = get_wall_orientation(game->player->f_real_pos, wall);
		if (game->map[(int)wall.y][(int)wall.x].symbol == 'c')
			orient = e_west;
		if (orient == e_east)
			printf("east\n");
		if (orient == e_north)
			printf("north\n");
		if (orient == e_south)
			printf("south\n");
		if (orient == e_west)
			printf("west\n");
		image = get_image(game, orient, wall);

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
	}
	
	addr = game->image->addr;
	while (i < y)
	{
		my_mlx_pixel_put(addr, size_line, (t_vector2){x, i}, 0x666666);
		i++;
	}
	while (y < y1)
	{
		my_mlx_pixel_put(addr, size_line, (t_vector2){x, y}, get_color_at(image->addr, image->size_line, (t_vector2){x_img, y_img}));
		y_img += delta_y_img;
		y++;
	}
	while (y1 < WIN_Y)
	{
		my_mlx_pixel_put(addr, size_line, (t_vector2){x, y1}, 0x222222);
		y1++;
	}
}
