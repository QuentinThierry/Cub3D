/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 18:39:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/08 18:39:32 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline unsigned int	get_color_at(char *addr, int size_line, t_vector2 pos)
{
	return (*(int*)(addr + (pos.y * size_line + pos.x * 4)));
}

__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, int color)
{
	*(int*)(addr + (pos.y * size_line + pos.x * 4)) = color;
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

	height = 1 / object_dist * game->constants[0];
	width = height;
	x_pos = x_pos - width / 2;
	image = get_image_wall(game, (t_ray){object->map_pos, e_object_image}, &x);
	x_ratio = image->size.x / width;
	y_ratio = image->size.y / height;
	y_pos = WIN_Y / 2. - height / 2.;
	x = x_pos;
	while (x < width + x_pos)
	{
		if (x >= WIN_X)
			break ;
		if (x < 0 || game->dist_tab[x] < object->dist)
		{
			x++;
			continue ;
		}
		x_img = (x - x_pos) * x_ratio;
		y = 0;
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
			if (get_color_at(image->addr, image->size_line, (t_vector2){x_img, y_img}) !=  GREEN_SCREEN)
				my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){x, y_pos + y},
					get_color_at(image->addr, image->size_line, (t_vector2){x_img, y_img}));
			y++;
		}
		x++;
	}
}


void	find_object_projection(t_game *game, t_object *object, t_player *player)
{
	t_fvector2	relative_pos;
	float		angle;
	float		arctan2;
	float		player_angle;
	int			x_screen;

	player_angle = player->angle;
	relative_pos.x = (object->map_pos.x - player->f_real_pos.x);
	relative_pos.y = (object->map_pos.y - player->f_real_pos.y);
	arctan2 = (atan2(-relative_pos.y, relative_pos.x) * 180 / M_PI);

	if (arctan2 > 0) // positive
	{
		if (player_angle >= 0 && player_angle < 90) // 0-90
			angle = (90 - player_angle) + FOV / 2. - arctan2;
		else if (player_angle >= 90 && player_angle < 180) // 90-180
			angle = (FOV / 2.) - (player_angle - 90) - arctan2;
		else if (player_angle >= 180 && player_angle < 270) // 180-270
			angle = 360 - (player_angle - 90 - FOV / 2.) - arctan2;
		else // 270-360
			angle = 360 - (player_angle - 90 - FOV / 2.) - arctan2;
	}
	else // negative
	{
		if (player_angle >= 0 && player_angle < 90) // 0-90
			angle = (90 - player_angle) + FOV / 2. - arctan2;
		else if (player_angle >= 90 && player_angle < 180) // 90-180
			angle = (FOV / 2.) - (player_angle - 90) - arctan2;
		else if (player_angle >= 180 && player_angle < 270) // 180-270
			angle = fabs(arctan2) - (player_angle - 90 - FOV / 2.);
		else // 270 - 360
			angle = fabs(arctan2) - (player_angle - 90 - FOV / 2.);
	}
	if (angle < FOV / 2.)
		angle = -(FOV / 2. - angle);
	else
		angle = angle - FOV / 2.;

	x_screen = tan(angle * TO_RADIAN) * game->constants[0];
	draw_object_projection(game, object, object->dist * cos(angle * TO_RADIAN), x_screen + WIN_X / 2);
}

static void	fill_object_dist(t_game *game)
{
	int			i;
	t_fvector2	relative_pos;
	
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



































































// __attribute__((always_inline))
// static inline unsigned int	get_color_at(char *addr, int size_line, t_vector2 pos)
// {
// 	return (*(int*)(addr + (pos.y * size_line + pos.x * 4)));
// }

// __attribute__((always_inline))
// static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, int color)
// {
// 	*(int*)(addr + (pos.y * size_line + pos.x * 4)) = color;
// }

// void	draw_line_object(t_game *game, int x_draw, t_ray ray, double height)
// {
// 	register int		i = 0;
// 	int					y;
// 	int					y1;
// 	int					x_img;
// 	int					x_door;
// 	float				y_img = 0;
// 	t_image				image;
// 	enum e_orientation	orient;
// 	float				delta_y_img;
// 	int					size_line;
// 	char				*addr;

// 	size_line = game->image->size_line;
// 	y = WIN_Y / 2.0 - (int)(height / 2);
// 	y1 = WIN_Y / 2.0 + ((int)height - (int)(height / 2));
// 	if (height != 0)
// 	{
		// image = get_image_wall(game, ray, &x_door);
		// if (image->addr == NULL)
		// 	return (printf("Error : Invalid image\n"),(void)ft_close(game));
// 		image = game->tab_images[0];
// 		delta_y_img = image.size.y / height;
// 		if (y < 0)
// 		{
// 			y_img = -y * delta_y_img;
// 			y = 0;
// 		}
// 		if (y1 > WIN_Y)
// 			y1 = WIN_Y;
// 		orient = ray.orient;
// 		if (orient == e_north || orient == e_south)
// 			x_img = (ray.hit.x - (int)ray.hit.x) * image.size.x;
// 		else
// 			x_img = (ray.hit.y - (int)ray.hit.y) * image.size.x;
// 		if (orient == e_west || orient == e_south)
// 			x_img = image.size.x - x_img - 1;
// 		if (x_door != 0)
// 			x_img = x_door;
// 	}
// 	addr = game->image->addr;
// 	i = y;
// 	while (i < y1)
// 	{
// 		my_mlx_pixel_put(addr, size_line, (t_vector2){x_draw, i}, get_color_at(image.addr, image.size_line, (t_vector2){x_img, y_img}));
// 		y_img += delta_y_img;
// 		i++;
// 	}
// }

// void	draw_object(t_game *game, t_ray ray, int x_draw, float angle)
// {
// 	t_fvector2	demi;
// 	double		dist;
// 	double		c;
// 	double		a;
// 	double		d;
// 	double		height;
// 	t_fvector2	res;
// 	int			i;
	
// 	i = 0;
// 	dist = get_dist_real(game->player->f_real_pos, ray.hit);
// 	while (i < ray.nb_object_hit)
// 	{
// 		ray = get_object_hit((t_objet){'\0', OBJECT, dist}, game, ray.hit, 360 - fabs(180 - angle));
// 		printf("%f	angle : %f	%f	x : %f Y : %f	 orient : %d\n",game->player->angle, fabs(180 - angle), angle ,ray.hit.x, ray.hit.y, ray.orient);
// 		if (ray.hit.x == -1)
// 			break ;
// 		demi.x = (int)ray.hit.x + 0.5;
// 		demi.y = (int)ray.hit.y + 0.5;
		// printf("angle b : %f	c : %f	a:%f\n", fabs(180 - angle), 90 - (game->player->angle - 90), 180 - (90 - (game->player->angle - 90)) - (fabs(180 - angle)));
		// a = get_dist_real(demi, ray.hit);
		// printf("a : %f\n", a);
		// c = (a / sin((180 - (90 - (game->player->angle - 90)) - (fabs(180 - angle))) * TO_RADIAN))
		// 	* sin((90 - (game->player->angle - 90)) * TO_RADIAN);
		// res.x = cos(fabs(180 - angle)) * c;
		// res.y = sin(fabs(180 - angle)) * c;
		// ray.hit.x += res.x;
		// ray.hit.y += res.y;
		// if ((game->map[(int)ray.hit.y][(int)ray.hit.x].type & OBJECT) != OBJECT)
		// {
		// 	i++;
		// 	continue ;
		// }
// 		d = get_dist(game->player->f_real_pos, demi, angle) ;
// 		height = 1 / dist * game->constants[0];
// 		printf("height : %f\n", height);
// 		draw_line_object(game, x_draw, ray, height);
// 		i++;
// 	}
// }
