/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 18:39:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/27 21:03:24 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static double	get_dist_real(t_fvector2 fpos, t_fvector2 wall)
{
	t_fvector2	delta;
	double		res;

	delta.x = fabs(wall.x - fpos.x);
	delta.y = fabs(wall.y - fpos.y);

	res = sqrt((delta.x * delta.x + delta.y * delta.y));
	if (res == 0)
		return (0.01);
	return (res);
}

void	draw_object(t_game *game, t_ray ray, int x, float angle)
{
	double	dist;
	int		i;
	
	i = 0;
	dist = get_dist_real(game->player->f_real_pos, ray.hit);
	while (i < ray.nb_object_hit)
	{
		ray = get_object_hit((t_objet){'\0', OBJECT, dist}, game, ray.hit, 360 - fabs(180 - angle));
		printf("angle : %f	x : %f Y : %f\n",360 - fabs(180 - angle), ray.hit.x, ray.hit.y);
		if (ray.hit.x == -1)
			break ;
		i++;
	}
}
