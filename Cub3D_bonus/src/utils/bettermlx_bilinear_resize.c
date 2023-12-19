/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bettermlx_bilinear_resize.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:36:03 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 13:20:49 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static uint	compute_alpha_red(t_pixel32 *src_pix,
	t_image *src, t_fvector2 src_pos)
{
	u_char		a;
	u_char		r;

	a = ((src_pix[0] >> 24 & 0xff) * (1 - (src_pos.x - (int)src_pos.x))
			+ (src_pix[1] >> 24 & 0xff) * (src_pos.x - (int)src_pos.x))
		* (1 - (src_pos.y - (int)src_pos.y))
		+ ((src_pix[src->size.x] >> 24 & 0xff)
			* (1 - (src_pos.x - (int)src_pos.x))
			+ (src_pix[src->size.x + 1] >> 24 & 0xff)
			* (src_pos.x - (int)src_pos.x)) * (src_pos.y - (int)src_pos.y);
	r = (((src_pix[0] >> 16 & 0xff) * (1 - (src_pos.x - (int)src_pos.x))
				+ (src_pix[1] >> 16 & 0xff) * (src_pos.x - (int)src_pos.x))
			* (1 - (src_pos.y - (int)src_pos.y))
			+ ((src_pix[src->size.x] >> 16 & 0xff)
				* (1 - (src_pos.x - (int)src_pos.x))
				+ (src_pix[src->size.x + 1] >> 16 & 0xff)
				* (src_pos.x - (int)src_pos.x)) * (src_pos.y - (int)src_pos.y));
	return ((uint)a << 24 | r << 16);
}

static uint	compute_green_blue(t_pixel32 *src_pix,
	t_image *src, t_fvector2 src_pos)
{
	u_char		g;
	u_char		b;

	g = ((src_pix[0] >> 8 & 0xff) * (1 - (src_pos.x - (int)src_pos.x))
			+ (src_pix[1] >> 8 & 0xff) * (src_pos.x - (int)src_pos.x))
		* (1 - (src_pos.y - (int)src_pos.y))
		+ ((src_pix[src->size.x] >> 8 & 0xff)
			* (1 - (src_pos.x - (int)src_pos.x))
			+ (src_pix[src->size.x + 1] >> 8 & 0xff)
			* (src_pos.x - (int)src_pos.x)) * (src_pos.y - (int)src_pos.y);
	b = ((src_pix[0] & 0xff) * (1 - (src_pos.x - (int)src_pos.x))
			+ (src_pix[1] & 0xff) * (src_pos.x - (int)src_pos.x))
		* (1 - (src_pos.y - (int)src_pos.y))
		+ ((src_pix[src->size.x] & 0xff)
			* (1 - (src_pos.x - (int)src_pos.x))
			+ (src_pix[src->size.x + 1] & 0xff)
			* (src_pos.x - (int)src_pos.x)) * (src_pos.y - (int)src_pos.y);
	return (g << 8 | b);
}

static void	compute_new_pixel(t_pixel32 *dst_addr,
	t_pixel32 *src_addr, t_image *src, t_fvector2 src_pos)
{
	t_pixel32	*src_pix;

	src_pix = src_addr + (int)src_pos.x + (int)src_pos.y * src->size.x;
	*dst_addr = compute_alpha_red(src_pix, src, src_pos)
		| compute_green_blue(src_pix, src, src_pos);
}

t_image	*resize_bilinear_img_malloc(void *mlx, t_image *src,
					t_vector2 dst_size)
{
	t_image		*dst;
	int			x;
	int			y;
	float		x_ratio;
	float		y_ratio;

	dst = btmlx_new_image(mlx, dst_size);
	if (!dst)
		return (NULL);
	x_ratio = (float)(src->size.x - 1) / (dst->size.x - 1);
	y_ratio = (float)(src->size.y - 1) / (dst->size.y - 1);
	y = 0;
	while (y < dst_size.y - 1)
	{
		x = 0;
		while (x < dst_size.x)
		{
			compute_new_pixel((t_pixel32 *)dst->addr + x + dst->size.x * y,
				(t_pixel32 *)src->addr, src,
				(t_fvector2){x * x_ratio, y * y_ratio});
			x++;
		}
		y++;
	}
	return (dst);
}

t_image	*btmlx_xpm_file_to_image_bilinear_resize(void *mlx, char *path,
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
