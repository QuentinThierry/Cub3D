/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 00:30:38 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/16 02:43:23 by qthierry         ###   ########.fr       */
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
		while (x < size.x)
		{
			my_mlx_pixel_put(addr,
				size_line, (t_vector2){pos.x + x, pos.y + y}, color);
			x++;
		}
		y++;
	}
}

static void	draw_minimap_on_main_image(t_minimap *mmap, t_image *image)
{
	t_vector2	dest_pos;
	int			y;
	int			i;

	y = 0;
	i = mmap->size.y / 2 - 1;
	dest_pos = (t_vector2){mmap->pos.x + mmap->size.x / 2, mmap->pos.y};
	while (y < mmap->size.y / 2)
	{
		ft_memcpy(
			image->addr + (dest_pos.y + y)
				* image->size_line + dest_pos.x * 4 - mmap->bounds[y] * 4,
			mmap->img->addr + y * mmap->img->size_line,
			mmap->bounds[y] * 4 * 2
		);
		ft_memcpy(
			image->addr + (dest_pos.y + (y + mmap->size.y / 2))
				* image->size_line + dest_pos.x * 4 - mmap->bounds[i] * 4,
			mmap->img->addr + (y + mmap->size.y / 2) * mmap->img->size_line,
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
	draw_rectangle(minimap->img, (t_vector2){0,0}, minimap->size, 0xFF0000);
	draw_minimap_on_main_image(minimap, game->image);
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
	// for (int i = 0; i < minimap->mmap_size.y / 2; i++) {
	// 	printf("%d\n", minimap->bounds[i]);
	// }
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
	return (true);
}