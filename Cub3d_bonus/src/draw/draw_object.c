/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 15:51:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/14 15:59:32 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

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

static t_image	*get_object_image(t_game *game, t_object *object)
{
	int	dummy;

	if ((game->map[(int)object->map_pos.y][(int)object->map_pos.x].type
		& OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE
		&& (game->map[(int)object->map_pos.y][(int)object->map_pos.x].type
			& WALL) == WALL)
		return (get_image_wall(game, (t_ray){object->map_pos,
				e_obj_int_before_img}, &dummy));
	else
		return (get_image_wall(game, (t_ray){object->map_pos,
				e_object_img}, &dummy));
}

static void	draw_object_pixel(t_object_infos *infos, t_vector2 xy,
	t_vector2 pos, float dark_quantity)
{
	t_pixel32	color;

	color = *((t_pixel32 *)infos->image->addr
			+ ((int)(xy.y * (infos->image->size.y / infos->dim_draw.y))
				*infos->image->size.x
				+ (int)(xy.x * (infos->image->size.x / infos->dim_draw.x))));
	if (color != GREEN_SCREEN)
			*((t_pixel32 *)infos->game->image->addr
				+ ((pos.y + xy.y) * infos->game->image->size.x
					+ xy.x + pos.x)) = dark_with_dist(color, dark_quantity);
}

static void	draw_all_object_pixels(t_object_infos *infos, t_vector2 pos,
	t_object *object, float dark_quantity)
{
	t_vector2		xy;

	xy.x = -1;
	if (pos.x < 0)
		xy.x = -pos.x - 1;
	while (++xy.x < infos->dim_draw.x)
	{
		if (pos.x + xy.x >= WIN_X)
			break ;
		if (infos->game->dist_tab[pos.x + xy.x] < object->dist)
			continue ;
		xy.y = -1;
		if (pos.y < 0)
			xy.y = -pos.y - 1;
		while (++xy.y < infos->dim_draw.y)
		{
			if (pos.y + xy.y < 0)
				continue ;
			if (pos.y + xy.y >= WIN_Y)
				break ;
			draw_object_pixel(infos, xy, pos, dark_quantity);
		}
	}
}

void	draw_object_projection(t_game *game, t_object *object,
			float object_dist, int x_pos)
{
	t_object_infos	infos;
	int				y_pos;
	float			dark_quantity;

	if (object_dist <= 0. || object_dist <= -0.)
		return ;
	infos.game = game;
	infos.image = get_object_image(game, object);
	infos.dim_draw.y = 1 / object_dist * game->constants[0];
	infos.dim_draw.x = infos.dim_draw.y
		* ((float)infos.image->size.x / infos.image->size.y);
	if (infos.dim_draw.x > infos.dim_draw.y)
		infos.dim_draw.x = infos.dim_draw.y;
	x_pos = x_pos - infos.dim_draw.x / 2;
	y_pos = WIN_Y / 2. - infos.dim_draw.y / 2.;
	dark_quantity = 0;
	if (object->dist >= DIST_MIN_DARK)
		dark_quantity = (-DIST_MIN_DARK + object->dist)
			/ (DIST_MAX_DARK - DIST_MIN_DARK);
	draw_all_object_pixels(&infos,
		(t_vector2){x_pos, y_pos}, object, dark_quantity);
}
