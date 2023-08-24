/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:50:23 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/24 22:19:16 by qthierry         ###   ########.fr       */
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

void	draw_line_ceiling(t_game *game, float x_dist, int y_screen, t_fvector2 hit)
{
	int					i;
	float				dist_to_left;
	float				h;
	t_fvector2			a;
	float				step;
	float				y_map;
	float				x_map;
	t_fvector2			left_p;
	const t_fvector2	f_pos = game->player->f_real_pos;
	t_image				image;
	t_pixel32			pix;

	dist_to_left = tanf(-(FOV / 2.0) * TO_RADIAN) * x_dist;
	h = x_dist / cos((FOV / 2.0) * TO_RADIAN);
	left_p.x = cos((game->player->angle - 90 - FOV / 2.) * TO_RADIAN) * h;
	left_p.y = -sin((game->player->angle - 90 - FOV / 2.) * TO_RADIAN) * h;
	step = fabs(dist_to_left * 2) / WIN_X;
	a.y = (hit.y - left_p.y) * step;
	a.x = (hit.x - left_p.x) * step;
	printf("y_screen : %d	step : %f	a :%f	%f\n", y_screen, step, a.x, a.y);
	y_map = f_pos.y + left_p.y;
	x_map = f_pos.x + left_p.x;
	fflush(stdout);
	i = 0;
	// hit.x += f_pos.x;
	// if (x_map > game->map_size.x || x_map < 0 || y_map > game->map_size.y || y_map < 0)
	// {
	// 	return ;
	// }
	while (i < WIN_X)
	{
		if (x_map >= game->map_size.x || x_map < 0 || y_map >= game->map_size.y || y_map < 0)
		{
			i++;
			y_map += a.y;
			x_map += a.x;
			continue;
		}
		image = game->tab_images[game->map[(int)y_map][(int)x_map].sprite[e_ceiling].index];
		pix = get_color_at(image.addr, image.size_line, (t_vector2){(x_map - (int)x_map) * image.size.x, (y_map - (int)y_map) * image.size.y});
		my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){i, WIN_Y / 2 - y_screen}, pix);
		y_map += a.y;
		x_map += a.x; 
		i++;
	}
}

// if odd WIN_Y, draw 1 floor pixel more than ceiling
void	draw_ceiling(t_game *game)
{
	t_fvector2	hit;
	int			y_screen;
	float		x_dist;
	float		y_dist;
	t_image		image;
	t_pixel32	pix;
	t_fvector2	f_pos;

	x_dist = 0;
	y_screen = WIN_Y / 2.;
	f_pos = game->player->f_real_pos;	
	while (y_screen > 0)
	{
		x_dist = (0.5 * (game->constants[0] / y_screen));
		hit.x = cos((game->player->angle - 90) * TO_RADIAN) * x_dist;
		hit.y = -sin((game->player->angle - 90) * TO_RADIAN) * x_dist;
		// printf("angle : %f	x_dist : %f, y_dist : %f\n",game->player->angle, hit.x, hit.y);

		// if (f_pos.x + hit.x > game->map_size.x || f_pos.x + hit.x < 0 || f_pos.y + hit.y > game->map_size.y || f_pos.y + hit.y < 0)
		// {
		// 	y_screen--;
		// 	continue ;
		// }
		draw_line_ceiling(game, x_dist, y_screen, hit);
		// if (game->map[(int)x_dist][1].type & WALL)
		// printf("image index : %d\n", game->map[1][(int)x_dist].sprite[e_ceiling].index);
		// image = game->tab_images[game->map[(int)(f_pos.y + hit.y)][(int)(f_pos.x + hit.x)].sprite[e_ceiling].index];
		// pix = get_color_at(image.addr, image.size_line, (t_vector2){((f_pos.x + hit.x) - (int)(f_pos.x + hit.x)) * image.size.x, ((f_pos.y + hit.y) - (int)(f_pos.y + hit.y)) * image.size.y});
		// my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){WIN_X / 2, WIN_Y / 2 - y_screen}, pix);
		y_screen--;
	}
	y_screen = 0;
	while (y_screen < WIN_X)
	{
		my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){y_screen, WIN_Y / 2}, 0x00ff00);
		y_screen++;
	}
}

			
			// y_dist += f_pos.y;