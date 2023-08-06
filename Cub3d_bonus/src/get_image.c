/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_image.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 16:13:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/06 15:02:00 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"


/**
 * @brief frame nb_total_frame ->pause sur frame begin
 * 
 * @param time 
 * @param sprite 
 * @param img 
 */
static void	update_anim(long int time, t_sprite *sprite, t_image *img)
{
	long	delta;
	int		n_time;

	delta = time - sprite->time;
	if (delta >= img->time_frame * img->nb_total_frame + img->time_animation)
	{
		n_time = delta / (img->time_frame * img->nb_total_frame + img->time_animation);
		sprite->time += (img->time_frame * img->nb_total_frame + img->time_animation) * n_time;
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
	{
		sprite->frame = 0;
		sprite->time += img->time_animation;
		delta = time - sprite->time;
		n_time = delta / img->time_frame;
		sprite->frame += n_time;
		sprite->time += img->time_frame * n_time;
	}
}

t_image	*get_image(t_game *game, enum e_orientation orient, t_fvector2 wall)
{
	t_sprite	*sprite;
	t_image		*image;
	
	if (orient == e_south)
		wall.y += 0.5;
	if (orient == e_east)
		wall.x += 0.5;

	if (orient == e_south)
		sprite = &(game->map[(int)wall.y - 1][(int)wall.x].sprite[orient]);
	else if (orient == e_north || orient == e_west)
		sprite = &(game->map[(int)wall.y][(int)wall.x].sprite[orient]);
	if (orient == e_east)
		sprite = &(game->map[(int)wall.y][(int)wall.x - 1].sprite[orient]);
	if (sprite->frame == -1)
		return (&(game->tab_images[sprite->index]));
	else		//animation
	{
		image = &game->tab_images[sprite->index];
		if (sprite->time == 0)
		{
			sprite->time = game->time;
			return (&(game->tab_images[sprite->index + sprite->frame]));
		}
		if (sprite->frame < image->nb_total_frame
				&& game->time - sprite->time >= image->time_frame)
			update_anim(game->time, sprite, image);
		else if (sprite->frame == image->nb_total_frame
				&& game->time - sprite->time >= image->time_animation)
			update_anim(game->time, sprite, image);
		if (sprite->frame == image->nb_total_frame)
			return (image);
		return (&(game->tab_images[sprite->index + sprite->frame]));
	}
}

long int	time_to_long(struct timespec *time)
{
	return (time->tv_nsec / 1000000 + time->tv_sec * 1000);
}