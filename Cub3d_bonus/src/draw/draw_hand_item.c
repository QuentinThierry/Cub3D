/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_hand_item.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 13:31:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/14 13:31:30 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static t_vector2	_get_size_draw(t_image *img)
{
	t_vector2	size;

	size = img->size;
	if ((WIN_Y - img->size.y) < 0)
		size.y = WIN_Y;
	if ((WIN_X - img->size.x) < 0)
		size.x = WIN_X;
	return (size);
}

void	update_anim(long int time, t_sprite *sprite, t_image *img);

void	draw_hand_item(t_game *game, t_player *player)
{
	t_image		*img;
	t_sprite	*sprite;
	t_vector2	size;

	sprite = &player->item.sprite[e_obj_int_hand_img];
	img = &game->tab_images[sprite->index];
	if (sprite->frame != -1)
	{
		if (sprite->time == 0)
			sprite->time = game->time;
		if (sprite->frame < img->nb_total_frame
			&& game->time - sprite->time >= img->time_frame)
			update_anim(game->time, sprite, img);
		else if (sprite->frame == img->nb_total_frame
			&& game->time - sprite->time >= img->time_animation)
			update_anim(game->time, sprite, img);
		if (sprite->frame != img->nb_total_frame)
			img = &(game->tab_images[sprite->index + sprite->frame]);
	}
	size = _get_size_draw(img);
	draw_image_with_green_sreen(game->image->addr + (WIN_Y - size.y)
		* game->image->size_line + (WIN_X - size.x) * 4, img, (t_vector2){0},
		size);
}
