/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 00:30:38 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/03 17:52:45 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_minimap_size =
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE
};

static const t_vector2	g_minimap_pos = 
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) - 
	(((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE) -
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD
};

// draw player on the main image, after rotation
void	mmap_draw_player(t_game *game)
{
	draw_image_on_image_alpha(game->image, game->minimap->player_img,
		(t_vector2){
		g_minimap_pos.x + g_minimap_size.x / 2
			- game->minimap->player_img->size.x / 2 - 1,
		g_minimap_pos.y + g_minimap_size.y / 2
			- game->minimap->player_img->size.x / 2 - 1});
}

void	zoom_hook_handle(t_minimap *minimap, double delta_time)
{
	if (minimap->zoom_dir >= 1 && minimap->zoom - ZOOM_OFFSET < MAX_ZOOM)
		minimap->zoom += delta_time * ZOOM_SPEED;
	else if (minimap->zoom_dir <= -1 && minimap->zoom - ZOOM_OFFSET > MIN_ZOOM)
		minimap->zoom -= delta_time * ZOOM_SPEED;
}

void	draw_minimap(t_game *game)
{
	t_minimap *minimap;

	minimap = game->minimap;
	if (!minimap)
		return ;
	ft_memcpy(minimap->image->addr, minimap->back_img->addr,
		g_minimap_size.x * g_minimap_size.y * 4);
	draw_rectangular_minimap(game);
	draw_rotated_image(minimap->buffer_img, minimap->image,
		(t_vector2){0, 0}, game->player->angle * TO_RADIAN);
	draw_minimap_buf_on_main_image(minimap, game->image);
	mmap_draw_player(game);
}
