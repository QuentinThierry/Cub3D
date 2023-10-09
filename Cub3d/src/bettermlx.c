/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bettermlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 21:04:29 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/09 17:06:37 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	bettermlx_get_data_addr(t_image *image, t_vector2 size)
{
	if (!image)
		return ;
	image->addr = mlx_get_data_addr(image->img, &image->opp,
			&image->size_line, &image->endian);
	image->opp /= 8;
	image->size = size;
}
