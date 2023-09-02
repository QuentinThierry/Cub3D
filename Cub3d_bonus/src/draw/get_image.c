/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_image.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 16:13:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/27 19:46:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"


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

t_image	*get_image_wall(t_game *game, t_ray ray, int *x_door)
{
	t_sprite	*sprite;
	t_image		*image;
	t_vector2	wall;
	float		dist;
	
	*x_door = 0;
	dist = -1;
	wall.x = (int)ray.hit.x;
	wall.y = (int)ray.hit.y;
	if (ray.orient == e_south)
		wall.y = (int)ceil(ray.hit.y);
	if (ray.orient == e_east)
		wall.x = (int)ceil(ray.hit.x);

	if (ray.orient == e_south)
	{
		if ((game->map[wall.y - 1][wall.x].type & DOOR_CLOSE) == DOOR_CLOSE)
			dist = get_texture_door(ray);
		sprite = &(game->map[wall.y - 1][wall.x].sprite[ray.orient]);
	}
	else if (ray.orient == e_north || ray.orient == e_west)
	{
		if ((game->map[wall.y][wall.x].type & DOOR_CLOSE) == DOOR_CLOSE)
			dist = get_texture_door(ray);
		sprite = &(game->map[wall.y][wall.x].sprite[ray.orient]);
	}
	if (ray.orient == e_east)
	{
		if ((game->map[wall.y][wall.x - 1].type & DOOR_CLOSE) == DOOR_CLOSE)
			dist = get_texture_door(ray);
		sprite = &(game->map[wall.y][wall.x - 1].sprite[ray.orient]);
	}
	if (sprite->frame == -1)
	{
		if (dist != -1)
			*x_door = game->tab_images[sprite->index].size.x * dist;
		return (&(game->tab_images[sprite->index]));
	}
	else		//animation
	{
		image = &game->tab_images[sprite->index];
		if (sprite->time == 0)
		{
			sprite->time = game->time;
			if (dist != -1)
				*x_door = game->tab_images[sprite->index + sprite->frame].size.x * dist;
			return (&(game->tab_images[sprite->index + sprite->frame]));
		}
		if (sprite->frame < image->nb_total_frame
				&& game->time - sprite->time >= image->time_frame)
			update_anim(game->time, sprite, image);
		else if (sprite->frame == image->nb_total_frame
				&& game->time - sprite->time >= image->time_animation)
			update_anim(game->time, sprite, image);
		if (sprite->frame == image->nb_total_frame)
		{
			if (dist != -1)
				*x_door = image->size.x * dist;
			return (image);
		}
		if (dist != -1)
			*x_door = game->tab_images[sprite->index + sprite->frame].size.x * dist;
		return (&(game->tab_images[sprite->index + sprite->frame]));
	}
}

t_image	*get_image_non_wall(t_game *game, t_fvector2 hit, enum e_orientation orient)
{
	t_sprite	*sprite;
	t_image		*image;

	sprite = &(game->map[(int)hit.y][(int)hit.x].sprite[orient]);
	// if (sprite->index == -1)
	// 	return (printf("no texture"), NULL);
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