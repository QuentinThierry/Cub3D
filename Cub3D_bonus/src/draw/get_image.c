/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_image.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 16:13:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 18:30:24 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_image		*animation(t_game *game, t_sprite *sprite, float dist, int *x_door);
void		update_anim(long int time, t_sprite *sprite, t_image *img);
t_sprite	*get_sprite(t_map **map, t_ray ray, float *dist, int *type);

t_image	*get_image_wall(t_game *game, t_ray ray, int *x_door)
{
	t_sprite	*sprite;
	float		dist;
	int			type;

	*x_door = -1;
	dist = -1;
	sprite = get_sprite(game->map, ray, &dist, &type);
	if (sprite == NULL)
		return (game->end->end_screen);
	if ((type & EXIT) == EXIT)
	{
		if (dist != -1)
			*x_door = game->tab_images[sprite->index].size.x * dist;
		return (&(game->tab_images[sprite->index + sprite->frame]));
	}
	if (sprite->frame == -1)
	{
		if (dist != -1)
			*x_door = game->tab_images[sprite->index].size.x * dist;
		return (&(game->tab_images[sprite->index]));
	}
	else
		return (animation(game, sprite, dist, x_door));
}

t_image	*get_image_non_wall(t_game *game, t_dvector2 hit,
			enum e_orientation orient)
{
	t_sprite	*sprite;
	t_image		*image;

	sprite = &(game->map[(int)hit.y][(int)hit.x].sprite[orient]);
	if (sprite->frame == -1)
		return (&(game->tab_images[sprite->index]));
	else
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
