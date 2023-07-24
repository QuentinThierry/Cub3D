/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bettermlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:51:49 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/24 19:17:52 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

t_image	*resize_img(void *mlx, t_image *src,
					t_vector2 dst_size, t_vector2 src_size)
{
	t_image		*dst;
	t_fvector2	ratio;
	int			x;
	int			y;

	dst = mlx_new_image(mlx, dst_size.x, dst_size.y);
	if (!dst)
		return (NULL);
	ratio.x = (double)(src_size.x) / (double)(dst_size.x);
	ratio.y = (double)(src_size.y) / (double)(dst_size.y);
	y = -1;
	while (++y < dst_size.y)
	{
		x = -1;
		while (++x < dst_size.x)
		{
			*(unsigned int *)(dst->addr + (int)(y) *dst->size_line
					+ (int)(x) *(dst->opp))
				= *(unsigned int *)(src->addr + (int)(y * ratio.y)
					*src->size_line
					+ (int)(x * ratio.x) *(src->opp));
		}
	}
	return (dst);
}

t_image	*btmlx_xpm_file_to_image(void *mlx, char *path,
			t_vector2 dst_size)
{
	t_image		*src;
	t_image		*dst;
	t_vector2	src_size;

	if (!dst_size.x || !dst_size.y)
		return (NULL);
	src = mlx_xpm_file_to_image(mlx, path, &src_size.x, &src_size.y);
	if (!src)
		return (NULL);
	if ((src_size.x == dst_size.x && src_size.y == dst_size.y)
		|| (dst_size.x == 0 || dst_size.y == 0))
		return (src);
	dst = resize_img(mlx, src, dst_size, src_size);
	if (!dst)
		return (mlx_destroy_image(mlx, src), NULL);
	mlx_destroy_image(mlx, src);
	return (dst);
}
