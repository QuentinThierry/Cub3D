/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bettermlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:51:49 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/11 14:56:14 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

t_image	*btmlx_new_image(void *mlx_ptr, t_vector2 size)
{
	t_image	*image;

	image = ft_calloc(1, sizeof(t_image));
	if (!image)
		return (NULL);
	image->img = mlx_new_image(mlx_ptr, size.x, size.y);
	if (!image->img)
		return (free(image), NULL);
	image->addr = mlx_get_data_addr(image->img,
		&image->opp, &image->size_line, &image->endian);
	image->opp /= 8;
	image->size = size;
	return (image);
}

t_image	*resize_img(void *mlx, t_image *src,
					t_vector2 dst_size)
{
	t_dvector2	ratio;
	t_image		*dst;
	int			x;
	int			y;

	dst = btmlx_new_image(mlx, dst_size);
	if (!dst)
		return (NULL);
	ratio.x = (double)(src->size.x) / (double)(dst_size.x);
	ratio.y = (double)(src->size.y) / (double)(dst_size.y);
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

	src = ft_calloc(1, sizeof(t_image));
	if (!src)
		return (NULL);
	src->img = mlx_xpm_file_to_image(mlx, path, &src_size.x, &src_size.y);
	if (!src->img)
		return (NULL);
	src->addr = mlx_get_data_addr(src->img, &src->opp,
		&src->size_line, &src->endian);
	src->opp /= 8;
	src->size = src_size;
	if ((src_size.x == dst_size.x && src_size.y == dst_size.y)
		|| (dst_size.x == 0 || dst_size.y == 0))
		return (src);
	dst = resize_img(mlx, src, dst_size);
	mlx_destroy_image(mlx, src->img);
	free(src);
	return (dst);
}
