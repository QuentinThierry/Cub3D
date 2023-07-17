/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 00:30:38 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/17 22:27:16 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, int color)
{
	*(int*)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

void	draw_horiz_line(t_image *image, int y, int x1, int x2)
{
	while (x1 < x2)
		my_mlx_pixel_put(image->addr, image->size_line, (t_vector2){x1++, y}, 0xFF0000);
}

static inline unsigned int	get_color_at(char *addr, int size_line, t_vector2 pos)
{
	return (*(int *)(addr + (pos.y * size_line + pos.x * 4)));
}

void	draw_rectangle(t_image *image, t_vector2 pos, t_vector2 size, unsigned int color)
{
	int		x;
	int		y;
	char	*addr;
	int		size_line;

	addr = image->addr;
	size_line = image->size_line;
	y = 0;
	while (y < size.y)
	{
		x = 0;
		while (x < size.x / 2)
		{
			my_mlx_pixel_put(addr,
				size_line, (t_vector2){pos.x + x, pos.y + y}, color);
			x++;
		}
		while (x < size.x)
		{
			my_mlx_pixel_put(addr,
				size_line, (t_vector2){pos.x + x, pos.y + y}, 0x0000FF);
			x++;
		}
		y++;
	}
}

static void	draw_minimap_buf_on_main_image(t_minimap *mmap, t_image *image)
{
	t_vector2	dest_pos;
	int			y;
	int			i;

	y = 0;
	i = mmap->buffer_img->size.y / 2 - 1;
	dest_pos = (t_vector2){mmap->pos.x, mmap->pos.y};
	while (y < mmap->buffer_img->size.y / 2)
	{
		my_mlx_pixel_put(image->addr, image->size_line, dest_pos, 0x0000);
		ft_memcpy(
			image->addr + (dest_pos.y + y)
				* image->size_line + dest_pos.x * 4 + (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[y] * 4,
			mmap->buffer_img->addr + y * mmap->buffer_img->size_line + (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[y] * 4,
			mmap->bounds[y] * 4 * 2
		);
		ft_memcpy(
			image->addr + (dest_pos.y + y + mmap->buffer_img->size.y / 2)
				* image->size_line + dest_pos.x * 4 + (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[i] * 4,
			mmap->buffer_img->addr + (y + mmap->buffer_img->size.y / 2) * mmap->buffer_img->size_line + (mmap->buffer_img->size.x / 2 * 4) - mmap->bounds[i] * 4,
			mmap->bounds[i] * 4 * 2
		);
		i--;
		y++;
	}
}

void	draw_minimap(t_game *game)
{
	t_minimap *minimap;

	minimap = game->minimap;
	if (!minimap)
		return ;
	
	draw_rectangle(minimap->img, (t_vector2){0,0}, minimap->size, 0xFF0000); // draw_minimap
	
	draw_rotated_image(minimap->buffer_img, minimap->img, (t_vector2){0, 0}, -game->player->angle * TO_RADIAN);
	draw_minimap_buf_on_main_image(minimap, game->image);
}

void	generate_minimap_bounds(t_game *game)
{
	int		x;
	int		y;
	int		p;

	t_minimap	*minimap;

	minimap = game->minimap;

	x = 0;
	y = minimap->size.x / 2.0;
	p = 1 - y;
	while (x <= y)
	{
		if (p < 0)
		{
			x++;
			p = p + 2 * x + 1;
		}
		else
		{
			y--;
			x++;
			p = p - 2 * y + 2 * x + 1;
		}
		minimap->bounds[minimap->size.x / 2 - x] = y;
		minimap->bounds[minimap->size.x / 2 - y] = x;
	}
	for (int i = 0; i< minimap->size.x / 2; i++) {
		printf("%d : %d\n", i, minimap->bounds[i]);
	}
}

bool	init_minimap(t_game *game)
{
	t_minimap	*minimap;

	game->minimap = ft_calloc(1, sizeof(t_minimap));
	if (!game->minimap)
		return (false);
	minimap = game->minimap;
	if (WIN_X < WIN_Y)
	{
		minimap->size = (t_vector2){WIN_X * MINIMAP_SIZE, WIN_X * MINIMAP_SIZE};
		minimap->pos = (t_vector2)
		{WIN_X * MINIMAP_PAD,
		WIN_X - minimap->size.y - WIN_X * MINIMAP_PAD};
	}
	else
	{
		minimap->size = (t_vector2){WIN_Y * MINIMAP_SIZE, WIN_Y * MINIMAP_SIZE};
		minimap->pos = (t_vector2)
		{WIN_Y * MINIMAP_PAD,
		WIN_Y - minimap->size.y - WIN_Y * MINIMAP_PAD};
	}
	minimap->bounds =
		ft_calloc(minimap->size.y / 2, sizeof(int));
	if (!minimap->bounds)
		return (false);
	generate_minimap_bounds(game);
	minimap->img = ft_calloc(1, sizeof(t_image));
	if (!minimap->img)
		return (false);
	minimap->img->img = mlx_new_image(game->mlx_ptr,
		minimap->size.x, minimap->size.y);
	if (!minimap->img->img)
		return (false);
	minimap->img->addr = mlx_get_data_addr(minimap->img->img, &minimap->img->opp, &minimap->img->size_line, &minimap->img->endian);
	minimap->img->opp /= 8;
	minimap->img->size = minimap->size;
	minimap->buffer_img = ft_calloc(1, sizeof(t_image));
	if (!minimap->buffer_img)
		return (false);
	minimap->buffer_img->img = mlx_new_image(game->mlx_ptr, minimap->size.x, minimap->size.y);
	if (!minimap->buffer_img->img)
		return (false);
	minimap->buffer_img->addr = mlx_get_data_addr(minimap->buffer_img->img, &minimap->buffer_img->opp, &minimap->buffer_img->size_line, &minimap->buffer_img->endian);
	minimap->buffer_img->opp /= 8;
	minimap->buffer_img->size = minimap->size;
	return (true);
}

// void	swap_pixels(t_image *image, t_vector2 pix1, t_vector2 pix2)
// {
// 	int	tmp_pix;
// 	int	*pix_pos1;
// 	int	*pix_pos2;

// 	pix_pos1 = (int *)(image->addr + (pix1.y * image->size_line + pix1.x * 4));
// 	pix_pos2 = (int *)(image->addr + (pix2.y * image->size_line + pix2.x * 4));
// 	tmp_pix = *pix_pos1;
// 	*pix_pos1 = *pix_pos2;
// 	*pix_pos2 = tmp_pix;
// }

void	draw_rotated_image(t_image *img_dest, t_image *img_src, t_vector2 pos, float angle)
{
	int				x;
	int				y;
	t_vector2		old_pos;
	t_vector2		image_size_div2;
	double			cos_angle;
	double			sin_angle;

	image_size_div2.x = img_src->size.x / 2;
	image_size_div2.y = img_src->size.y / 2;
	cos_angle = cos(angle);
	sin_angle = sin(angle);
	y = 0;
	while (y < img_src->size.y)
	{
		x = 0;
		while (x < img_src->size.x)
		{
			old_pos.x = (x - image_size_div2.x) * cos_angle - (y - image_size_div2.y) * sin_angle;
			old_pos.y = (x - image_size_div2.x) * sin_angle + (y - image_size_div2.y) * cos_angle;
			if (old_pos.x + image_size_div2.x < img_src->size.x && old_pos.x + image_size_div2.x >= 0
				&& old_pos.y + image_size_div2.y < img_src->size.y && old_pos.y + image_size_div2.y >= 0)
			{
				my_mlx_pixel_put(
					img_dest->addr, img_dest->size_line,
					(t_vector2){(int)x + pos.x, (int)y + pos.y},
					*(int *)(img_src->addr + ((old_pos.y + image_size_div2.y) * img_src->size_line + (old_pos.x + image_size_div2.x) * 4)));
			}
			x++;
		}
		y++;
	}
}