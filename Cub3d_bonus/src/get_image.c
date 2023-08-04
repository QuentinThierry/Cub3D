/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_image.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 16:13:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/04 20:06:00 by qthierry         ###   ########.fr       */
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
void	update_anim(long int time, t_sprite *sprite, t_image *img)
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
	sprite->frame += n_time;
	sprite->time += img->time_frame * n_time;
	delta = time - sprite->time;
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
	t_sprite *img;
	
	if (orient == e_south)
		wall.y += 0.5;
	if (orient == e_east)
		wall.x += 0.5;

	if (orient == e_south)
		img = &(game->map[(int)wall.y - 1][(int)wall.x].sprite[orient]);
	else if (orient == e_north || orient == e_west)
		img = &(game->map[(int)wall.y][(int)wall.x].sprite[orient]);
	if (orient == e_east)
		img = &(game->map[(int)wall.y][(int)wall.x - 1].sprite[orient]);
	if (img->frame == -1)
		return (&(game->tab_images[img->index]));
	else		//animation
	{
		if (img->time == 0)
		{
			img->time = game->time;
			return (&(game->tab_images[img->index + img->frame]));
		}
		else if (game->time - img->time >= game->tab_images[img->index].time_frame 
				&& img->frame < game->tab_images[img->index].nb_total_frame)
			update_anim(game->time, img, &game->tab_images[img->index]);
		else if (game->time - img->time >= game->tab_images[img->index].time_animation 
				&& img->frame == game->tab_images[img->index].nb_total_frame)
			update_anim(game->time, img, &game->tab_images[img->index]);
		if (img->frame == game->tab_images[img->index].nb_total_frame)
			return (&(game->tab_images[img->index]));
		return (&(game->tab_images[img->index + img->frame]));
	}
}

long int	time_to_long(struct timespec *time)
{
	return (time->tv_nsec / 1000000 + time->tv_sec * 1000);
}