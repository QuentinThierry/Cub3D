/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bettermlx_bilinear_resize.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:36:03 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/04 15:38:00 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

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