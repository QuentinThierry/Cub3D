/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:50:23 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/24 20:13:21 by qthierry         ###   ########.fr       */
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

void	draw_line_ceiling(t_game *game, float x_dist, int y_screen)
{
	int					i;
	float				y_left;
	// float				y_right;
	float				delta_y;
	float				y_map;
	const t_fvector2	f_pos = game->player->f_real_pos;
	t_image				image;
	t_pixel32			pix;

	y_left = tanf(-(FOV / 2.0) * TO_RADIAN) * x_dist;
	// y_right = -y_left;
	delta_y = fabs(y_left * 2) / WIN_X;
	y_map = f_pos.y + y_left;
	// printf("y_screen : %d	x : %f	player x : %f	y_left : %f\n", y_screen, x_dist, f_pos.x, y_left);
	// fflush(stdout);
	i = 0;
	x_dist += f_pos.x;
	if (x_dist > game->map_size.x || x_dist < 0)
	{
		return ;
	}
	while (i < WIN_X)
	{
		if (y_map > game->map_size.y || y_map < 0)
		{
			i++;
			y_map += delta_y;
			continue;
		}
		image = game->tab_images[game->map[(int)y_map][(int)x_dist].sprite[e_ceiling].index];
		pix = get_color_at(image.addr, image.size_line, (t_vector2){(y_map - (int)y_map) * image.size.y, (x_dist - (int)x_dist) * image.size.x});
		my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){i, WIN_Y / 2 - y_screen}, pix);
		y_map += delta_y;
		i++;
	}
}

// if odd WIN_Y, draw 1 floor pixel more than ceiling
void	draw_ceiling(t_game *game)
{
	int			x_screen;
	int			y_screen;
	float		x_dist;
	// t_image		image;
	// t_pixel32	pix;
	t_fvector2	f_pos;

	x_dist = 0;
	y_screen = 0;
	f_pos = game->player->f_real_pos;
	// bzero(game->image->addr, WIN_X * WIN_Y * 4);
	
	while (y_screen <= WIN_Y / 2)
	{
		x_dist = (0.5 * (game->constants[0] / y_screen));


		
		draw_line_ceiling(game, x_dist, y_screen);
		// if (game->map[(int)x_dist][1].type & WALL)
		// printf("image index : %d\n", game->map[1][(int)x_dist].sprite[e_ceiling].index);
		// image = game->tab_images[game->map[(int)f_pos.y][(int)x_dist].sprite[e_ceiling].index];
		// pix = get_color_at(image.addr, image.size_line, (t_vector2){(f_pos.y - (int)f_pos.y) * image.size.y, (x_dist - (int)x_dist) * image.size.x});
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){WIN_X / 2, WIN_Y / 2 - y_screen}, pix);
		y_screen++;
	}
	y_screen = 0;
	while (y_screen < WIN_X)
	{
		my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){y_screen, WIN_Y / 2}, 0x00ff00);
		y_screen++;
	}
}

			// x_dist = cos((game->player->angle - 90) * TO_RADIAN) * x_dist;
			// y_dist = -sin((game->player->angle - 90) * TO_RADIAN) * x_dist;
			
			// y_dist += f_pos.y;