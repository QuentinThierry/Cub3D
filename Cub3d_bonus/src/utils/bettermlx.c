/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bettermlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:51:49 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/02 18:32:49 by qthierry         ###   ########.fr       */
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

t_image	*resize_img_malloc(void *mlx, t_image *src,
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
	dst = resize_img_malloc(mlx, src, dst_size);
	mlx_destroy_image(mlx, src->img);
	free(src);
	return (dst);
}

t_image	*resize_bilinear_img_malloc(void *mlx, t_image *src,
					t_vector2 dst_size)
{
	t_image		*dst;
	t_pixel32	*dst_addr;
	t_pixel32	*src_addr;
	t_pixel32	*src_pix;
	int			x;
	int			y;
	float		x_ratio;
	float		y_ratio;
	float		x_src;
	float		y_src;

	dst = btmlx_new_image(mlx, dst_size);
	if (!dst)
		return (NULL);
	dst_addr = (t_pixel32 *)dst->addr;
	src_addr = (t_pixel32 *)src->addr;

	x_ratio = (float)(src->size.x - 1) / (dst->size.x - 1);
	y_ratio = (float)(src->size.y - 1) / (dst->size.y - 1);
	y = 0;
	while (y < dst_size.y - 1)
	{
		y_src = y * y_ratio;
		x = 0;
		while (x < dst_size.x)
		{
			x_src = x * x_ratio;
			src_pix = src_addr + (int)x_src + (int)y_src * src->size.x;
			dst_addr[x + dst->size.x * y] =
			((u_char)((((src_pix[0] >> 24) & 0xff)
					* (1 - (x_src - (int)x_src))
				+ ((src_pix[1] >> 24) & 0xff)
					* (x_src - (int)x_src)) * (1 - (y_src - (int)y_src))
			+ (((src_pix[src->size.x] >> 24) & 0xff)
					* (1 - (x_src - (int)x_src))
				+ ((src_pix[src->size.x + 1] >> 24) & 0xff)
					* (x_src - (int)x_src)) * (y_src - (int)y_src)) << 24)
			| ((u_char)(((src_pix[0] >> 16 & 0xff)
					* (1 - (x_src - (int)x_src))
				+ (src_pix[1] >> 16 & 0xff)
					* (x_src - (int)x_src)) * (1 - (y_src - (int)y_src))
			+ ((src_pix[src->size.x] >> 16 & 0xff)
					* (1 - (x_src - (int)x_src))
				+ (src_pix[src->size.x + 1] >> 16 & 0xff)
					* (x_src - (int)x_src)) * (y_src - (int)y_src)) << 16)
			| ((u_char)(((src_pix[0] >> 8 & 0xff)
					* (1 - (x_src - (int)x_src))
				+ (src_pix[1] >> 8 & 0xff)
					* (x_src - (int)x_src)) * (1 - (y_src - (int)y_src))
			+ ((src_pix[src->size.x] >> 8 & 0xff)
					* (1 - (x_src - (int)x_src))
				+ (src_pix[src->size.x + 1] >> 8 & 0xff)
					* (x_src - (int)x_src)) * (y_src - (int)y_src)) << 8)
			| (u_char)(((src_pix[0] & 0xff)
					* (1 - (x_src - (int)x_src))
				+ (src_pix[1] & 0xff)
					* (x_src - (int)x_src)) * (1 - (y_src - (int)y_src))
			+ ((src_pix[src->size.x] & 0xff)
					* (1 - (x_src - (int)x_src))
				+ (src_pix[src->size.x + 1] & 0xff)
					* (x_src - (int)x_src)) * (y_src - (int)y_src));
			x++;
		}
		y++;
	}
	return (dst);
}

t_image		*btmlx_xpm_file_to_image_bilinear_resize(void *mlx, char *path,
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
	dst = resize_bilinear_img_malloc(mlx, src, dst_size);
	mlx_destroy_image(mlx, src->img);
	free(src);
	return (dst);
}