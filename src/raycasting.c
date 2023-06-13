/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:25:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/13 18:55:04 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	my_mlx_pixel_put(t_image *img, int x, int y, int color);

int	get_wall_dist(t_game *game, float angle)
{
	t_fvector2	step;
	t_fvector2	delta;
	t_fvector2	comp;
	float	r_angle;
	int		x,y;

	// game->player.pos.x = 210;
	// game->player.pos.y = 120;
	my_mlx_pixel_put(&game->image,
		game->player.pos.x, game->player.pos.y, 0xFF0000);
	r_angle = angle * M_PI / 180;
	delta.x = (float)game->player.pos.x / CHUNK_SIZE - (int)(game->player.pos.x / CHUNK_SIZE);
	delta.y = (float)game->player.pos.y / CHUNK_SIZE - (int)(game->player.pos.y / CHUNK_SIZE);

	step.x = 1 * (float)tan(r_angle);
	step.y = 1 / (float)tan(r_angle);

	x = game->player.pos.x / CHUNK_SIZE;
	y = game->player.pos.y / CHUNK_SIZE;

	comp.x = x + delta.x - delta.y * (float)tan(r_angle);
	comp.y = y + delta.y - delta.x / (float)tan(r_angle);

	while (true)
	{
		while (y < comp.y)
		{
			my_mlx_pixel_put(&game->image,
					x * CHUNK_SIZE, (int)(comp.y * CHUNK_SIZE), 0xD77BBA);
			if (game->maps[(int)comp.y][x] == '1')
			{
				return (0);
			}
			comp.y -= step.y;
			x--;
		}
		while (x < comp.x)
		{
			my_mlx_pixel_put(&game->image,
					(int)(comp.x * CHUNK_SIZE), y * CHUNK_SIZE, 0x00FF00);
			if (game->maps[y][(int)comp.x] == '1')
			{
				return (0);
			}
			comp.x -= step.x;
			y--;
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
		angle = (float)x * (FOV / 2) / (WIN_X / 2);
		if (game->player.angle + angle > 360)
			angle = angle - 360 ;
		printf("angle : %f 	x: %d\n", game->player.angle + angle, x);
		// get_wall_dist(game, game->player.angle + angle);
		x++;
	}
	// while (angle > WIN_X / 2)
	// {
	// 	// angle = FOV / x;
	// 	get_wall_dist(game, game->player.angle + angle);
	// 	angle++;
	// 	x++;
	// }
}