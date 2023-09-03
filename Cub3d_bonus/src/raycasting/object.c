/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 18:39:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/03 20:41:33 by qthierry         ###   ########.fr       */
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

void	draw_object(t_game *game, t_object *object, t_player *player)
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


	if (player_angle >= 0 && player_angle < 90) // 0-90
		angle = (90 - player_angle) + FOV / 2. - arctan2;
	else if (player_angle >= 90 && player_angle < 180) // 90-180
		angle = (FOV / 2.) - (player_angle - 90) - arctan2;
	else if (player_angle >= 180 && player_angle < 270) // 180-270
		angle = arctan2 - ((player_angle - 90) - FOV / 2.);
	else // 270-360
		angle = 360 - ((player_angle - 90) - (FOV / 2.)) - arctan2;

	// if (arctan2 < 0)
	// 	angle = FOV - angle;
	x_screen = WIN_X * angle / FOV;
	
	// printf("angle : %f\n", angle);
	printf("x_screen: %d, angle: %f, p_angle: %f arctan2: %f\n", x_screen, angle, player_angle, arctan2);
	if (x_screen > 0 && x_screen < WIN_X)
	{
		for (int y = 0; y < WIN_Y; y++)
			my_mlx_pixel_put(game->image->addr, game->image->size_line, (t_vector2){x_screen, y}, 0xFF00);
	}
}


void	draw_objects(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->nb_objects)
	{
		if (game->object_array[i]->visited)
			draw_object(game, game->object_array[i], game->player);
		game->object_array[i]->visited = false;
		i++;
	}


	
}



































































// static double	get_dist_real(t_fvector2 fpos, t_fvector2 wall)
// {
// 	t_fvector2	delta;

// 	delta.x = fabs(wall.x - fpos.x);
// 	delta.y = fabs(wall.y - fpos.y);

// 	return (sqrt(delta.x * delta.x + delta.y * delta.y));
// }

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
// 		// image = get_image_wall(game, ray, &x_door);
// 		// if (image->addr == NULL)
// 		// 	return (printf("Error : Invalid image\n"),(void)ft_close(game));
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
// 		// printf("angle b : %f	c : %f	a:%f\n", fabs(180 - angle), 90 - (game->player->angle - 90), 180 - (90 - (game->player->angle - 90)) - (fabs(180 - angle)));
// 		// a = get_dist_real(demi, ray.hit);
// 		// printf("a : %f\n", a);
// 		// c = (a / sin((180 - (90 - (game->player->angle - 90)) - (fabs(180 - angle))) * TO_RADIAN))
// 		// 	* sin((90 - (game->player->angle - 90)) * TO_RADIAN);
// 		// res.x = cos(fabs(180 - angle)) * c;
// 		// res.y = sin(fabs(180 - angle)) * c;
// 		// ray.hit.x += res.x;
// 		// ray.hit.y += res.y;
// 		// if ((game->map[(int)ray.hit.y][(int)ray.hit.x].type & OBJECT) != OBJECT)
// 		// {
// 		// 	i++;
// 		// 	continue ;
// 		// }
// 		d = get_dist(game->player->f_real_pos, demi, angle) ;
// 		height = 1 / dist * game->constants[0];
// 		printf("height : %f\n", height);
// 		draw_line_object(game, x_draw, ray, height);
// 		i++;
// 	}
// }
