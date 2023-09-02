/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 17:16:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/28 17:20:55 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	init_audio(t_game *game)
{
	InitAudioDevice();
	if (!IsAudioDeviceReady() || !game)
		return (false);
	
	return (true);
}

void	close_sound(t_game *game)
{
	if (!IsAudioDeviceReady())
		return ;
	
	CloseAudioDevice();
}