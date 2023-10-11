/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:46:16 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/11 19:04:11 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	fix_angle(t_end *end, enum e_orientation orient)
{
	if (end->dir_angle > 180)
		end->dir_angle = end->dir_angle - 360;
	if (end->dir_angle < -180)
		end->dir_angle = 360 + end->dir_angle;
	if (orient == e_south && end->dir_angle < 0)
		end->dest_angle = 0;
}

/**
 * @brief init the struct end with the destination where the player need to go
 * 	and its view angle
 * 
 * @param end 
 * @param orient 
 * @param pos_exit 
 * @param player 
 */
void	find_dest(t_end *end, const enum e_orientation orient,
		const t_vector2 exit, const t_player *player)
{
	if (orient == e_north)
	{
		end->dest = (t_fvector2){exit.x + .5, exit.y - DIST_TO_WALL};
		end->dest_angle = 180;
	}
	else if (orient == e_south)
	{
		end->dest = (t_fvector2){exit.x + .5, exit.y + 1 + DIST_TO_WALL};
		end->dest_angle = 360;
	}
	else if (orient == e_east)
	{
		end->dest = (t_fvector2){exit.x + 1 + DIST_TO_WALL, exit.y + .5};
		end->dest_angle = 270;
	}
	else
	{
		end->dest = (t_fvector2){exit.x - DIST_TO_WALL, exit.y + .5};
		end->dest_angle = 90;
	}
	end->dir.x = end->dest.x - player->f_pos.x;
	end->dir.y = end->dest.y - player->f_pos.y;
	end->dir_angle = end->dest_angle - player->angle;
	fix_angle(end, orient);
}

/**
 * @brief init the end->dest to the coordonne behind the door that has been open
 * 
 * @param end 
 * @param player_pos 
 */
void	next_dest(t_end *end, const t_dvector2 player_pos)
{
	if (end->orient == e_south)
		end->dest.y -= 1 + DIST_TO_WALL;
	else if (end->orient == e_north)
		end->dest.y += 1 + DIST_TO_WALL;
	else if (end->orient == e_east)
		end->dest.x -= 1 + DIST_TO_WALL;
	else
		end->dest.x += 1 + DIST_TO_WALL;
	end->dir.x = end->dest.x - player_pos.x;
	end->dir.y = end->dest.y - player_pos.y;
	end->dir_angle = 0;
	end->status = e_open_door;
}
