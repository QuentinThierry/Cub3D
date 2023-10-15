/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 18:39:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 14:55:56 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline unsigned int	get_color_at(char *addr, int size_line, t_vector2 pos)
{
	return (*(int *)(addr + (pos.y * size_line + pos.x * 4)));
}

__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, int color)
{
	*(int *)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

__attribute__((always_inline))
static inline unsigned int	dark_with_dist(unsigned int color, float dark_quantity)
{
	float	color_quantity;

	color_quantity = 1 - dark_quantity;
	return (((unsigned char)(((color >> 16) & 0xFF) * color_quantity + ((FOG_COLOR >> 16) & 0xff) * dark_quantity) << 16)
		| ((unsigned char)(((color >> 8) & 0xFF) * color_quantity + ((FOG_COLOR >> 8) & 0xff) * dark_quantity) << 8)
		| (unsigned char)((color & 0xFF) * color_quantity + (FOG_COLOR & 0xff) * dark_quantity));
}

void	draw_object_projection(t_game *game, t_object *object, float object_dist, int x_pos)
{
	float		height;
	float		width;
	int			y_pos;
	t_image		*image;
	int			x;
	int			y;
	float		x_img;
	float		y_img;
	float		x_ratio;
	float		y_ratio;
	float		size_ratio;
	t_pixel32	color;
	float		dark_quantity;

	if (object_dist <= 0. || object_dist <= -0.)
		return ;
	height = 1 / object_dist * game->constants[0];
	if ((game->map[(int)object->map_pos.y][(int)object->map_pos.x].type & OBJECT_INTERACTIVE) == OBJECT_INTERACTIVE
		&& (game->map[(int)object->map_pos.y][(int)object->map_pos.x].type & WALL) == WALL)
		image = get_image_wall(game, (t_ray){object->map_pos, e_object_interactive_before_image}, &x);
	else
		image = get_image_wall(game, (t_ray){object->map_pos, e_object_image}, &x);
	size_ratio = (float)image->size.x / image->size.y;
	width = height * size_ratio;
	if (width > height)
		width = height;
	x_pos = x_pos - width / 2;
	y_pos = WIN_Y / 2. - height / 2.;
	x_ratio = image->size.x / width;
	y_ratio = image->size.y / height;
	if (object->dist >= DIST_MIN_FOG)
		dark_quantity = (-DIST_MIN_FOG + object->dist) / (DIST_MAX_FOG - DIST_MIN_FOG);
	else
		dark_quantity = 0;
	x = 0;
	if (x_pos < 0)
		x = -x_pos;
	while (x < width)
	{
		x_img = x * x_ratio;
		if (x_pos + x >= WIN_X)
			break ;
		if (game->dist_tab[x_pos + x] < object->dist)
		{
			x++;
			continue ;
		}
		y = 0;
		if (y_pos < 0)
			y = -y_pos;
		while (y < height)
		{
			if (y_pos + y < 0)
			{
				y++;
				continue ;
			}
			if (y_pos + y >= WIN_Y)
				break ;
			y_img = y * y_ratio;
			color = get_color_at(image->addr, image->size_line, (t_vector2){x_img, y_img});
			if (color != GREEN_SCREEN)
				my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){x + x_pos, y_pos + y}, dark_with_dist(color, dark_quantity));
			y++;
		}
		x++;
	}
}

void	find_object_projection(t_game *game, t_object *object, t_player *player)
{
	t_dvector2	relative_pos;
	float		angle;
	float		arctan2;
	float		player_angle;
	int			x_screen;

	player_angle = player->angle;
	relative_pos.x = (object->map_pos.x - player->f_real_pos.x);
	relative_pos.y = (object->map_pos.y - player->f_real_pos.y);
	arctan2 = (atan2f(-relative_pos.y, relative_pos.x) * 180 / M_PI);

	if (arctan2 > 0) // positive
	{
		if (player_angle >= 0 && player_angle < 90) // 0-90
			angle = (90 - player_angle) + game->fov / 2. - arctan2;
		else if (player_angle >= 90 && player_angle < 180) // 90-180
			angle = (game->fov / 2.) - (player_angle - 90) - arctan2;
		else if (player_angle >= 180 && player_angle < 270) // 180-270
			angle = 360 - (player_angle - 90 - game->fov / 2.) - arctan2;
		else // 270-360
			angle = 360 - (player_angle - 90 - game->fov / 2.) - arctan2;
	}
	else // negative
	{
		if (player_angle >= 0 && player_angle < 90) // 0-90
			angle = (90 - player_angle) + game->fov / 2. - arctan2;
		else if (player_angle >= 90 && player_angle < 180) // 90-180
			angle = (game->fov / 2.) - (player_angle - 90) - arctan2;
		else if (player_angle >= 180 && player_angle < 270) // 180-270
			angle = fabsf(arctan2) - (player_angle - 90 - game->fov / 2.);
		else // 270 - 360
			angle = fabsf(arctan2) - (player_angle - 90 - game->fov / 2.);
	}
	if (angle < game->fov / 2.)
		angle = -(game->fov / 2. - angle);
	else
		angle = angle - game->fov / 2.;
	x_screen = WIN_X / 2. + tanf(angle * TO_RADIAN) * game->constants[0];
	draw_object_projection(game, object, object->dist * cosf(angle * TO_RADIAN), x_screen);
}

static void	fill_object_dist(t_game *game)
{
	int			i;
	t_dvector2	relative_pos;

	i = 0;
	while (i < game->nb_objects)
	{
		if (game->object_array[i]->visited)
		{
			relative_pos.x = (game->object_array[i]->map_pos.x - game->player->f_real_pos.x);
			relative_pos.y = (game->object_array[i]->map_pos.y - game->player->f_real_pos.y);
			game->object_array[i]->dist = sqrtf(relative_pos.x * relative_pos.x + relative_pos.y * relative_pos.y);
		}
		else
			game->object_array[i]->dist = -1;
		i++;
	}
}

static void	sort_objects_distance(t_object **objects, int nb_object)
{
	int			i;
	int			j;
	t_object	*key;

	i = 1;
	while (i < nb_object)
	{
		j = i - 1;
		key = objects[i];
		while (j >= 0 && key->dist > objects[j]->dist && key->dist != -1)
		{
			objects[j + 1] = objects[j];
			j--;
		}
		objects[j + 1] = key;
		i++;
	}
}

void	draw_objects(t_game *game)
{
	int	i;

	i = 0;
	if (game->nb_objects != 0)
	{
		fill_object_dist(game);
		sort_objects_distance(game->object_array, game->nb_objects);
		while (i < game->nb_objects)
		{
			if (game->object_array[i]->visited)
				find_object_projection(game, game->object_array[i], game->player);
			game->object_array[i]->visited = false;
			i++;
		}
	}
}
