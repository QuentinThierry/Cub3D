/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 17:53:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 18:23:13 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	_set_new_anim(t_sprite *sprite, long int time, t_image *img)
{
	long	delta;
	int		n_time;

	sprite->frame = 0;
	sprite->time += img->time_animation;
	delta = time - sprite->time;
	n_time = delta / img->time_frame;
	sprite->frame += n_time;
	sprite->time += img->time_frame * n_time;
}

/**
 * @brief frame nb_total_frame ->pause sur frame begin
 */
void	update_anim(long int time, t_sprite *sprite, t_image *img)
{
	long	delta;
	long	n_time;

	delta = time - sprite->time;
	if (delta >= img->time_frame * img->nb_total_frame + img->time_animation)
	{
		n_time = delta / (img->time_frame * img->nb_total_frame
				+ img->time_animation);
		sprite->time += (img->time_frame * img->nb_total_frame
				+ img->time_animation) * n_time;
		delta = time - sprite->time;
	}
	n_time = delta / img->time_frame;
	if (sprite->frame + n_time >= img->nb_total_frame)
		n_time = img->nb_total_frame - sprite->frame;
	if (n_time > 0)
	{
		sprite->frame += n_time;
		sprite->time += img->time_frame * n_time;
		delta = time - sprite->time;
	}
	if (sprite->frame == img->nb_total_frame && delta >= img->time_animation)
		_set_new_anim(sprite, time, img);
}

t_image	*animation(t_game *game, t_sprite *spr, float dist, int *x_door)
{
	t_image		*image;

	image = &game->tab_images[spr->index];
	if (spr->time == 0)
	{
		spr->time = game->time;
		if (dist != -1)
			*x_door = game->tab_images[spr->index + spr->frame].size.x * dist;
		return (&(game->tab_images[spr->index + spr->frame]));
	}
	if (spr->frame < image->nb_total_frame
		&& game->time - spr->time >= image->time_frame)
		update_anim(game->time, spr, image);
	else if (spr->frame == image->nb_total_frame
		&& game->time - spr->time >= image->time_animation)
		update_anim(game->time, spr, image);
	if (spr->frame == image->nb_total_frame)
	{
		if (dist != -1)
			*x_door = image->size.x * dist;
		return (image);
	}
	if (dist != -1)
		*x_door = game->tab_images[spr->index + spr->frame].size.x * dist;
	return (&(game->tab_images[spr->index + spr->frame]));
}
