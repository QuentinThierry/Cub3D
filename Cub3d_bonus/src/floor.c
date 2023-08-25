/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:50:23 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/25 20:04:47 by jvigny           ###   ########.fr       */
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

__attribute__((always_inline))
static inline void draw_pixels(t_game *game, t_fvector2 map_point, t_fvector2 step_dir, int y_screen)
{
	int			i;
	t_image		*image;
	t_pixel32	pix;
	t_sprite	*tab_sprite;

	i = 0;
	while (i < WIN_X)
	{
		if (map_point.x >= game->map_size.x || map_point.x < 0 || map_point.y >= game->map_size.y || map_point.y < 0)
		{
			map_point.x += step_dir.x;
			map_point.y += step_dir.y;
			i++;
			continue;
		}
		tab_sprite = game->map[(int)map_point.y][(int)map_point.x].sprite;
		if (tab_sprite[e_ceiling].index != -1)
		{
			image = &game->tab_images[tab_sprite[e_ceiling].index];
			pix = get_color_at(image->addr, image->size_line,
				(t_vector2){(map_point.x - (int)map_point.x) * image->size.x,
				(map_point.y - (int)map_point.y) * image->size.y});
			my_mlx_pixel_put(game->image->addr, game->image->size_line,
				(t_vector2){i, WIN_Y / 2 - y_screen}, pix);
			image = &game->tab_images[tab_sprite[e_floor].index];
			pix = get_color_at(image->addr, image->size_line,
				(t_vector2){(map_point.x - (int)map_point.x) * image->size.x,
				(map_point.y - (int)map_point.y) * image->size.y});
			my_mlx_pixel_put(game->image->addr, game->image->size_line,
				(t_vector2){i, WIN_Y / 2 + y_screen}, pix);
		}
		map_point.x += step_dir.x; 
		map_point.y += step_dir.y;
		i++;
	}
}

void	draw_line_ceiling(t_game *game, float x_dist, int y_screen, t_fvector2 hit)
{
	float				dist_to_left;
	float				h;
	t_fvector2			a;
	t_fvector2			map_point;
	t_fvector2			left_p;

	dist_to_left = fabs((game->constants[TAN_HALF_FOV]) * x_dist);
	h = x_dist / game->constants[COS_HALF_FOV];
	left_p.x = cos((game->player->angle - 90 - FOV / 2.) * TO_RADIAN) * h;
	left_p.y = sin((game->player->angle - 90 - FOV / 2.) * TO_RADIAN) * h;
	a.y = ((hit.y - left_p.y) / dist_to_left) * fabs(dist_to_left * 2) / WIN_X;
	a.x = ((hit.x - left_p.x) / dist_to_left) * fabs(dist_to_left * 2) / WIN_X;
	map_point.x = game->player->f_real_pos.x + left_p.x;
	map_point.y = game->player->f_real_pos.y + left_p.y;
	draw_pixels(game, map_point, a, y_screen);
}

// if odd WIN_Y, draw 1 floor pixel more than ceiling
void	draw_ceiling(t_game *game)
{
	t_fvector2	hit;
	int			y_screen;
	float		x_dist;

	x_dist = 0;
	y_screen = WIN_Y / 2.;
	while (y_screen > 0)
	{
		x_dist = (0.5 * (game->constants[0] / y_screen));
		hit.x = cos((game->player->angle - 90) * TO_RADIAN) * x_dist;
		hit.y = sin((game->player->angle - 90) * TO_RADIAN) * x_dist;
		draw_line_ceiling(game, x_dist, y_screen, hit);
		y_screen--;
	}
	y_screen = 0;
	while (y_screen < WIN_X)
	{
		my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){y_screen, WIN_Y / 2}, 0x00ff00);
		y_screen++;
	}
}
