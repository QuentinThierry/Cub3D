/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:34:06 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/12 20:14:31 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_minimap_size =
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE
};

void	generate_minimap_bounds(t_game *game)
{
	int			x;
	int			y;
	int			p;
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
	game->minimap->back_img = btmlx_new_image(game->mlx_ptr, g_minimap_size);
	if (!game->minimap->back_img)
		return (false);
	fill_rectangle(game->minimap->back_img->addr,
		game->minimap->back_img->size, MINIMAP_BACKGROUND_COLOR);
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
	if (!minimap->buffer_img)
		return (false);
	if (!generate_background_image(game))
		return (false);
	minimap->player_img = btmlx_xpm_file_to_image(game->mlx_ptr,
			"assets/minimap_player.xpm", (t_vector2){
			g_minimap_size.x / MINIMAP_PLAYER_SIZE,
			g_minimap_size.y / MINIMAP_PLAYER_SIZE});
	if (!minimap->player_img)
		return (false);
	minimap->zoom = ZOOM_OFFSET;
	return (true);
}
