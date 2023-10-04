/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_rect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:56:06 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/04 17:07:36 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_minimap_size =
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE
};

static const int	g_show_case_range =
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE / MMAP_CHUNK / 2 + 1;

void	draw_rectangular_minimap(t_game *game)
{
	int				x;
	int				y;
	t_pixel32		color;
	const t_map		**map = (const t_map **)game->map;
	const t_minimap	*minimap = (const t_minimap *)game->minimap;

	y = game->player->f_real_pos.y - (g_show_case_range - (minimap->zoom / 2 - ZOOM_OFFSET));
	while (y < game->player->f_real_pos.y + (g_show_case_range - (minimap->zoom / 2 - ZOOM_OFFSET)) && y < game->map_size.y)
	{
		if (y < 0)
			y = 0;
		x = game->player->f_real_pos.x - (g_show_case_range - (minimap->zoom / 2 - ZOOM_OFFSET));
		while (x < game->player->f_real_pos.x + (g_show_case_range - (minimap->zoom / 2 - ZOOM_OFFSET)) && x < game->map_size.x)
		{
			if (x < 0)
				x = 0;
			if ((map[y][x].type & WALL) == WALL)
				color = MINIMAP_WALL_COLOR;
			else
				color = MINIMAP_BACKGROUND_COLOR;
			draw_rectangle(minimap->image,
				(t_vector2){
				(x - game->player->f_real_pos.x) * (MMAP_CHUNK + (int)minimap->zoom - ZOOM_OFFSET) + g_minimap_size.x / 2.0,
				(y - game->player->f_real_pos.y) * (MMAP_CHUNK + (int)minimap->zoom - ZOOM_OFFSET) + g_minimap_size.y / 2.0},
				(t_vector2){MMAP_CHUNK + minimap->zoom - ZOOM_OFFSET + 1, MMAP_CHUNK + minimap->zoom - ZOOM_OFFSET + 1}, color);
			x++;
		}
		y++;
	}
}
