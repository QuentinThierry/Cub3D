/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/20 19:20:35 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


float	get_dist(t_game *game, double x, double y, double angle)
{
	t_fvector2	delta;

	delta.x = fabs(x - game->player->f_real_pos.x);
	delta.y = fabs(y - game->player->f_real_pos.y);
	return (cos(angle * M_PI / 180) * sqrt((delta.x * delta.x + delta.y * delta.y)));
	// return (delta.x * cos((FOV / 2.0) * M_PI / 180) + delta.y * sin((FOV / 2.0) * M_PI / 180));
}

t_vector2	get_sign(double angle)
{
	t_vector2	sign;

	if (angle >= 0 && angle <= 180)
		sign.x = 1;
	else
		sign.x = -1;
	if (angle >= 90 && angle <= 270)
		sign.y = 1;
	else
		sign.y = -1;
	return (sign);
}

t_fvector2	get_wall_hit(t_game *game, double angle)
{
	t_fvector2	step;
	t_fvector2	delta;
	t_fvector2	comp;
	t_vector2	sign;
	int		x,y;
	
	sign = get_sign(angle);

	angle = fabs(tan(angle * M_PI / 180));
	
	x = (int)(game->player->f_real_pos.x) + (sign.x == 1);
	y = (int)(game->player->f_real_pos.y) + (sign.y == 1);
	delta.x = fabs(game->player->f_real_pos.x - (x));
	delta.y = fabs(game->player->f_real_pos.y - (y));

	step.x = 1 * angle * sign.x;
	step.y = 1 / angle * sign.y;
	
	comp.x = game->player->f_real_pos.x + delta.y * angle * sign.x;
	comp.y = game->player->f_real_pos.y + delta.x / angle * sign.y;

	while (true)
	{
		while ((sign.y == 1 && y >= comp.y) || (y <= comp.y && sign.y == -1))		//x
		{
			if (game->maps[(int)comp.y][x + (sign.x == -1) * -1] == '1')
			{
				return ((t_fvector2){x, comp.y});
			}
			comp.y += step.y;
			x += sign.x;
		}
		while ((sign.x == 1 && x >= comp.x) || (x <= comp.x && sign.x == -1))		//y
		{
			if (game->maps[y + (sign.y == -1) * -1][(int)comp.x] == '1')
			{
				return ((t_fvector2){comp.x, y});
			}
			comp.x += step.x;
			y += sign.y;
		}
	}
	return ((t_fvector2){0});
}


void	raycasting(t_game *game)
{
	int		x;
	double	height;
	double	angle;
	t_fvector2	wall;

	x = - WIN_X / 2.0;
	while (x <  WIN_X / 2.0)
	{
		angle = ((double)x * (FOV / 2.0)) / (WIN_X/ 2.0);
		if (game->player->angle + angle >= 360)
			game->player->angle = game->player->angle - 360;
		if (game->player->angle + angle < 0)
			game->player->angle = game->player->angle + 360;
		wall = get_wall_hit(game, game->player->angle + angle);
		height = HEIGHT_WALL / get_dist(game, wall.x, wall.y, fabs(angle));
		
		draw_vert_sprite(game, x + WIN_X / 2.0, wall, height);
		x++;
	}
}

