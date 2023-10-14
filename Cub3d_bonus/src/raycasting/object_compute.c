/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_compute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 15:50:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/14 15:55:16 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static float	arctan2_positive(float player_angle,
	t_game *game, float arctan2)
{
	if (player_angle >= 0 && player_angle < 90)
		return ((90 - player_angle) + game->fov / 2. - arctan2);
	if (player_angle >= 90 && player_angle < 180)
		return ((game->fov / 2.) - (player_angle - 90) - arctan2);
	if (player_angle >= 180 && player_angle < 270)
		return (360 - (player_angle - 90 - game->fov / 2.) - arctan2);
	return (360 - (player_angle - 90 - game->fov / 2.) - arctan2);
}

static float	arctan2_negative(float player_angle,
	t_game *game, float arctan2)
{
	if (player_angle >= 0 && player_angle < 90)
		return ((90 - player_angle) + game->fov / 2. - arctan2);
	if (player_angle >= 90 && player_angle < 180)
		return ((game->fov / 2.) - (player_angle - 90) - arctan2);
	if (player_angle >= 180 && player_angle < 270)
		return (fabsf(arctan2) - (player_angle - 90 - game->fov / 2.));
	return (fabsf(arctan2) - (player_angle - 90 - game->fov / 2.));
}

void	find_object_projection(t_game *game, t_object *object, t_player *player)
{
	t_dvector2	relative_pos;
	float		angle;
	float		arctan2;
	float		player_angle;
	int			x_screen;

	player_angle = player->angle;
	relative_pos.x = (object->map_pos.x - player->f_pos.x);
	relative_pos.y = (object->map_pos.y - player->f_pos.y);
	arctan2 = (atan2f(-relative_pos.y, relative_pos.x) * 180 / M_PI);
	if (arctan2 > 0)
		angle = arctan2_positive(player_angle, game, arctan2);
	else
		angle = arctan2_negative(player_angle, game, arctan2);
	if (angle < game->fov / 2.)
		angle = -(game->fov / 2. - angle);
	else
		angle = angle - game->fov / 2.;
	x_screen = WIN_X / 2. + tanf(angle * TO_RADIAN) * game->constants[0];
	draw_object_projection(game, object, object->dist
		* cosf(angle * TO_RADIAN), x_screen);
}
