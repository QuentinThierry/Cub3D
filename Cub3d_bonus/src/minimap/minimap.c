/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 00:30:38 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/27 18:37:21 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_minimap_size =
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE
};

static const t_vector2	g_minimap_pos = 
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) - 
	(((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE) -
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD
};

__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, t_pixel32 color)
{
	*(int*)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

__attribute__((always_inline))
static inline void	my_mlx_pixel_put_sec(t_image *image, t_vector2 pos, t_pixel32 color)
{
	if (pos.y < 0 || pos.x < 0 || pos.x >= image->size.x || pos.y >= image->size.y)
	{
		return ;
	}
	*(int*)(image->addr + (pos.y * image->size_line + pos.x * 4)) = color;
}


void	draw_horiz_line(t_image *image, t_vector2 origin, int len, t_pixel32 color)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (origin.x + i >= image->size.x || origin.x + i < 0)
			return ;
		my_mlx_pixel_put(image->addr, image->size_line, (t_vector2){
		origin.x + i, origin.y}, color);
		i++;
	}
}

// static inline unsigned int	get_color_at(char *addr, int size_line, t_vector2 pos)
// {
// 	return (*(int *)(addr + (pos.y * size_line + pos.x * 4)));
// }

void	draw_rectangle(t_image *image, t_vector2 pos, t_vector2 size, t_pixel32 color)
{
	int	x;
	int	y;
	
	y = pos.y;
	while (y < size.y + pos.y)
	{
		x = pos.x;
		while (x < size.x + pos.x)
		{
			my_mlx_pixel_put_sec(image, (t_vector2){x, y}, color);
			x++;
		}
		y++;
	}
}

void	draw_rectangular_minimap(t_game *game)
{
	int			x;
	int			y;
	t_pixel32	color;
	const t_map **map = (const t_map **)game->map;
	const t_minimap *minimap = (const t_minimap *)game->minimap;
	const int	show_case_range = g_minimap_size.x / MMAP_CHUNK / 2 + 1;

	y = game->player->f_real_pos.y - (show_case_range - (minimap->zoom / 2 - ZOOM_OFFSET));
	while (y < game->player->f_real_pos.y + (show_case_range - (minimap->zoom / 2 - ZOOM_OFFSET)) && y < game->map_size.y)
	{
		if (y < 0)
			y = 0;
		x = game->player->f_real_pos.x - (show_case_range - (minimap->zoom / 2 - ZOOM_OFFSET));
		while (x < game->player->f_real_pos.x + (show_case_range - (minimap->zoom / 2 - ZOOM_OFFSET)) && x < game->map_size.x)
		{
			if (x < 0)
				x = 0;
			if ((map[y][x].type & WALL) == WALL)
				color = 0x505050;
			else
				color = 0x808080;
			draw_rectangle(minimap->image,
			(t_vector2){
			(x - game->player->f_real_pos.x) * (MMAP_CHUNK + (int)minimap->zoom - ZOOM_OFFSET) + g_minimap_size.x / 2.0,
			(y - game->player->f_real_pos.y) * (MMAP_CHUNK + (int)minimap->zoom - ZOOM_OFFSET) + g_minimap_size.y / 2.0},
			(t_vector2){MMAP_CHUNK + minimap->zoom - ZOOM_OFFSET, MMAP_CHUNK + minimap->zoom - ZOOM_OFFSET}, color);
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
	i = mmap->image->size.y / 2 - 1;
	dest_pos = (t_vector2){g_minimap_pos.x, g_minimap_pos.y};

	// while (y < mmap->img->size.y)
	// {
	// 	ft_memcpy(image->addr + y * image->size_line, 
	// 	mmap->img->addr + y * mmap->background_image->size_line, mmap->size.x * 4);
	// 	y++;
	// }
	
	while (y < mmap->buffer_img->size.y / 2)
	{
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

// draw player on the main image, after rotation
void	mmap_draw_player(t_game *game)
{
	draw_image_on_image_alpha(game->image, game->minimap->player_img,
		(t_vector2){
		g_minimap_pos.x + g_minimap_size.x / 2
			- game->minimap->player_img->size.x / 2,
		g_minimap_pos.y + g_minimap_size.y / 2
			- game->minimap->player_img->size.x / 2});
}

void	zoom_hook_handle(t_minimap *minimap, double delta_time)
{
	if (minimap->zoom_dir >= 1 && minimap->zoom - ZOOM_OFFSET < MAX_ZOOM)
		minimap->zoom += delta_time * ZOOM_SPEED;
	else if (minimap->zoom_dir <= -1 && minimap->zoom - ZOOM_OFFSET > MIN_ZOOM)
		minimap->zoom -= delta_time * ZOOM_SPEED;
}

void	draw_minimap(t_game *game)
{
	t_minimap *minimap;

	minimap = game->minimap;
	if (!minimap)
		return ;
	ft_memcpy(minimap->image->addr, minimap->back_img->addr,
		g_minimap_size.x * g_minimap_size.y * 4);
	draw_rectangular_minimap(game);
	draw_rotated_image(minimap->buffer_img, minimap->image, (t_vector2){}, game->player->angle * TO_RADIAN);
	draw_minimap_buf_on_main_image(minimap, game->image);
	mmap_draw_player(game);
}

void	generate_minimap_bounds(t_game *game)
{
	int		x;
	int		y;
	int		p;
	t_minimap	*minimap;

	minimap = game->minimap;
	x = 0;
	y = g_minimap_size.x / 2.0;
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
		minimap->bounds[g_minimap_size.x / 2 - x] = y;
		minimap->bounds[g_minimap_size.x / 2 - y] = x;
	}
}

static void	fill_rectangle(char *addr, t_vector2 size, unsigned int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < size.y)
	{
		x = 0;
		while (x < size.x)
		{
			*(unsigned int *)addr = color;
			addr += 4;
			x++;
		}
		y++;
	}
}

bool	generate_background_image(t_game *game)
{
	t_image *bck_image;

	bck_image = btmlx_new_image(game->mlx_ptr, g_minimap_size);
	if (!bck_image)
		return (false);
	game->minimap->back_img = bck_image;
	fill_rectangle(bck_image->addr, bck_image->size, 0x808080);
	return (true);
}


bool	init_minimap(t_game *game)
{
	t_minimap	*minimap;

	game->minimap = ft_calloc(1, sizeof(t_minimap));
	if (!game->minimap)
		return (false);
	minimap = game->minimap;
	minimap->bounds = ft_calloc(g_minimap_size.y / 2, sizeof(int));
	if (!minimap->bounds)
		return (false);
	generate_minimap_bounds(game);
	minimap->image = btmlx_new_image(game->mlx_ptr, g_minimap_size);
	if (!minimap->image)
		return (false);
	minimap->buffer_img = btmlx_new_image(game->mlx_ptr, g_minimap_size);
	if (!minimap->image)
		return (false);
	generate_background_image(game);

	minimap->player_img = btmlx_xpm_file_to_image(game->mlx_ptr, "assets/minimap_player.xpm", (t_vector2){15, 15});
	if (!minimap->player_img->img)
		return (false);
	minimap->zoom = ZOOM_OFFSET;
	return (true);
}

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