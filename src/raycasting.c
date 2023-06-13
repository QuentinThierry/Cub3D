/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/13 20:33:44 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	get_wall_dist(t_game *game, float angle)
{
	t_fvector2	step;
	t_fvector2	delta;
	t_fvector2	comp;
	t_vector2	sign;
	float	tan_angle;
	int		x,y;
	
	my_mlx_pixel_put(&game->image,
		game->player.pos.x, game->player.pos.y, 0xFF0000);
	if (angle > 360)
		angle = angle - 360 ;
	if (angle < 0)
		angle = angle + 360 ;

		
	if (angle >= 0 && angle <= 180)
		sign.x = 1;
	else
		sign.x = -1;
	if (angle >= 90 && angle <= 270)
		sign.y = 1;
	else
		sign.y = -1;
	tan_angle = (float)tan(angle * M_PI / 180);
	printf("angle : %f x : %d y : %d\n",angle, sign.x, sign.y);
	printf("player	x : %f y : %f\n", game->player.f_real_pos.x, game->player.f_real_pos.y);
	x = (int)(game->player.f_real_pos.x) + (sign.x == 1);
	y = (int)(game->player.f_real_pos.y) + (sign.y == 1);
	printf("x : %d y : %d\n", x, y);
	delta.x = (game->player.f_real_pos.x - (x)) * sign.x * -1;
	delta.y = (game->player.f_real_pos.y - (y)) * sign.y * -1;
	printf("delta	x : %f y : %f\n", delta.x, delta.y);

	step.x = 1 * tan_angle * sign.x;
	step.y = 1 / tan_angle * sign.y;
	printf("step	x : %f y : %f\n", step.x, step.y);

	// printf("x : %d y : %d\n", x, y);
	// printf("delta x : %f delta y : %f\n", delta.x, delta.y);
	comp.x = x + delta.x + (delta.y * tan_angle) * sign.x;
	comp.y = y + delta.y + (delta.x / tan_angle) * sign.y;
	printf("comp	x : %f y : %f\n\n", comp.x, comp.y);

	while (true)
	{
		while ((sign.y == 1 && y > comp.y) || (y < comp.y))
		{
			my_mlx_pixel_put(&game->image,
					x * CHUNK_SIZE, (int)(comp.y * CHUNK_SIZE), 0xD77BBA);
			if (game->maps[(int)comp.y][x] == '1')
			{
				return (0);
			}
			comp.y += step.y;
			x += sign.x;
		}
		while ((sign.x == 1 && x > comp.x) || (x < comp.x))
		{
			my_mlx_pixel_put(&game->image,
					(int)(comp.x * CHUNK_SIZE), y * CHUNK_SIZE, 0x00FF00);
			if (game->maps[y][(int)comp.x] == '1')
			{
				return (0);
			}
			comp.x += step.x;
			y += sign.y;
		}
	}

	return (0);
}


void	raycasting(t_game *game)
{
	int		x;
	float	angle;

	game->player.angle = 350;
	x = - WIN_X / 2;
	while (x <=  WIN_X / 2)
	{
		angle = ((float)x * (FOV / 2)) / (WIN_X / 2);
		if (game->player.angle + angle > 360)
			angle = angle - 360 ;
		if (game->player.angle + angle < 0)
			angle = angle + 360 ;
		printf("angle : %f 	x: %d\n", game->player.angle + angle, x);
		// get_wall_dist(game, game->player.angle + angle);
		x++;
	}
}