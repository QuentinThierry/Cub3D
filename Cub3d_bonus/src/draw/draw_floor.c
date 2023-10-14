/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 17:34:57 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/14 17:45:09 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline unsigned int	get_color_at(char *addr,
	int size_line, t_vector2 pos)
{
	return (*(int *)(addr + (pos.y * size_line + pos.x * 4)));
}

__attribute__((always_inline))
static inline float	get_dist(t_dvector2 fpos, t_dvector2 wall)
{
	return (((wall.x - fpos.x) * (wall.x - fpos.x)
			+ (wall.y - fpos.y) * (wall.y - fpos.y)));
}

__attribute__((always_inline))
static inline unsigned int	dark_with_dist(unsigned int color,
	float dark_quantity)
{
	float	color_quantity;

	color_quantity = 1 - dark_quantity;
	return (((unsigned char)(((color >> 16) & 0xFF) * color_quantity
			+ ((DARK_COLOR >> 16) & 0xff) * dark_quantity) << 16)
		| ((unsigned char)(((color >> 8) & 0xFF) * color_quantity
		+ ((DARK_COLOR >> 8) & 0xff) * dark_quantity) << 8)
		| (unsigned char)((color & 0xFF) * color_quantity
		+ (DARK_COLOR & 0xff) * dark_quantity));
}

static void	put_pixel_floor(t_floor_infos *inf, t_image *image,
	t_vector2 screen_pos, float dark_quantity)
{
	if (dark_quantity != -1)
	{
		*((t_pixel32 *)inf->game->image->addr + screen_pos.y
				* inf->game->image->size.x + screen_pos.x)
			= dark_with_dist(get_color_at(image->addr, image->size_line,
					(t_vector2){(inf->map_point.x - inf->last_map_pos.x)
					* image->size.x, (inf->map_point.y - inf->last_map_pos.y)
					* image->size.y}), dark_quantity);
	}
	else
	{
		*((t_pixel32 *)inf->game->image->addr + screen_pos.y
				* inf->game->image->size.x + screen_pos.x)
			= get_color_at(image->addr, image->size_line,
				(t_vector2){(inf->map_point.x - inf->last_map_pos.x)
				* image->size.x,
				(inf->map_point.y - inf->last_map_pos.y) * image->size.y});
	}
}

void	compute_pixel(t_floor_infos *in, int i, int y_screen)
{
	float	dark_quantity;
	float	dist;

	dist = get_dist(in->game->player->f_pos, in->map_point);
	if (dist >= DIST_MIN_DARK * DIST_MIN_DARK)
	{
		dark_quantity
			= (-DIST_MIN_DARK + sqrtf(dist)) / (DIST_MAX_DARK - DIST_MIN_DARK);
		if (dark_quantity >= 1)
		{
			*((t_pixel32 *)in->game->image->addr + (WIN_Y / 2 - y_screen)
					* in->game->image->size.x + i) = DARK_COLOR;
			*((t_pixel32 *)in->game->image->addr + (WIN_Y / 2 + y_screen - 1)
					* in->game->image->size.x + i) = DARK_COLOR;
			return ;
		}
		put_pixel_floor(in, in->img_ceil,
			(t_vector2){i, WIN_Y / 2 - y_screen}, dark_quantity);
		put_pixel_floor(in, in->img_floor,
			(t_vector2){i, WIN_Y / 2 + y_screen - 1}, dark_quantity);
		return ;
	}
	(put_pixel_floor(in, in->img_ceil, (t_vector2){i, WIN_Y / 2 - y_screen},
		-1), put_pixel_floor(in, in->img_floor,
		(t_vector2){i, WIN_Y / 2 + y_screen - 1}, -1));
}
