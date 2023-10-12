/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 18:23:04 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 18:28:49 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	load_image(t_game *game, t_image *img, char *filename,
		t_animation *anim)
{
	img->img = mlx_xpm_file_to_image(game->mlx_ptr, filename,
			&(img->size.x), &(img->size.y));
	if (img->img == NULL)
		return (false);
	img->addr = mlx_get_data_addr(img->img,
			&img->opp, &img->size_line, &img->endian);
	if (img->opp != 32)
		return (false);
	img->opp /= 8;
	if (!update_loading_screen(game, game->loading_screen))
		return (false);
	if (anim == NULL)
		return (true);
	img->time_animation = anim->time_animation;
	img->time_frame = anim->time_sprite;
	img->nb_total_frame = anim->nb_sprite - 1;
	return (true);
}

static t_image	*_resize_img(t_image *src,
					t_vector2 dst_size, t_image *dst)
{
	t_dvector2	ratio;
	int			x;
	int			y;

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

static t_image	*_load_image(t_game *game, t_image *img, char *filename)
{
	img->img = mlx_xpm_file_to_image(game->mlx_ptr, filename,
			&(img->size.x), &(img->size.y));
	if (img->img == NULL)
		return (NULL);
	img->addr = mlx_get_data_addr(img->img,
			&img->opp, &img->size_line, &img->endian);
	if (img->opp != 32)
		return (NULL);
	img->opp /= 8;
	return (img);
}

bool	load_resize_image(t_game *game, t_image *img, char *filename,
		t_vector2 dst_size)
{
	t_image	dst;

	img = _load_image(game, img, filename);
	if (img == NULL)
		return (false);
	if ((img->size.x == dst_size.x && img->size.y == dst_size.y)
		|| (dst_size.x == 0 || dst_size.y == 0))
		return (true);
	dst.img = mlx_new_image(game->mlx_ptr, dst_size.x, dst_size.y);
	if (!dst.img)
		return (false);
	dst.addr = mlx_get_data_addr(dst.img, &dst.opp, &dst.size_line,
			&dst.endian);
	if (dst.opp != 32)
		return (mlx_destroy_image(game->mlx_ptr, dst.img), false);
	dst.opp /= 8;
	dst.size = dst_size;
	dst = *_resize_img(img, dst_size, &dst);
	mlx_destroy_image(game->mlx_ptr, img->img);
	*img = dst;
	if (!update_loading_screen(game, game->loading_screen))
		return (false);
	return (true);
}
