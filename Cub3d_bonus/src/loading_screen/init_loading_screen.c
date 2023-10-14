/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_loading_screen.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:42:32 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 16:57:46 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_size_font = (t_vector2){WIN_X, WIN_Y / 16};
static const t_vector2	g_size_background = (t_vector2){WIN_X, WIN_Y};
static const t_vector2	g_size_loading_bar = (t_vector2){WIN_X / 3, WIN_Y / 16};
static const double		g_resize_letter = (1880 / 94.) / 1880;

static bool	_load_image(t_game *game, t_loading *screen)
{
	game->font = btmlx_xpm_file_to_image_bilinear_resize(game->mlx_ptr,
			LOADING_FONT, g_size_font);
	if (game->font == NULL)
		return (false);
	game->size_letter.x = game->font->size.x * g_resize_letter;
	game->size_letter.y = game->font->size.y;
	screen->background = btmlx_xpm_file_to_image_bilinear_resize(game->mlx_ptr,
			LOADING_SCREEN, g_size_background);
	if (screen->background == NULL)
		return (false);
	screen->bordure = btmlx_xpm_file_to_image(game->mlx_ptr, LOADING_BORDURE,
			g_size_loading_bar);
	if (screen->bordure == NULL)
		return (false);
	screen->center = btmlx_xpm_file_to_image(game->mlx_ptr, LOADING_CENTER,
			g_size_loading_bar);
	if (screen->center == NULL)
		return (false);
	return (true);
}

/**
 * @brief load all the picture for the loading screen and draw the loading
 * 	screen
 * 
 * @param game 
 * @return true 
 * @return false 
 */
bool	loading_screen(t_game *game)
{
	t_loading	*loading_screen;

	game->loading_screen = ft_calloc(1, sizeof(t_loading));
	if (game->loading_screen == NULL)
		return (false);
	loading_screen = game->loading_screen;
	if (!_load_image(game, loading_screen))
		return (false);
	game->image->addr = ft_memcpy(game->image->addr,
			loading_screen->background->addr, WIN_X * WIN_Y * 4);
	draw_image_with_green_sreen(game->image->addr
		+ ((WIN_Y / 2 - loading_screen->bordure->size.y / 2)
			* game->image->size_line + (WIN_X / 3) * 4),
		loading_screen->bordure, (t_vector2){0}, g_size_loading_bar);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	return (true);
}

/**
 * @brief free the struct loading screen and all the picture and set to NULL
 * 
 * @param game 
 */
void	free_loading_screen(t_game *game)
{
	if (game->loading_screen == NULL)
		return ;
	if (game->loading_screen->background != NULL)
	{
		if (game->loading_screen->background->img != NULL)
			mlx_destroy_image(game->mlx_ptr,
				game->loading_screen->background->img);
		free(game->loading_screen->background);
	}
	if (game->loading_screen->bordure != NULL)
	{
		if (game->loading_screen->bordure->img != NULL)
			mlx_destroy_image(game->mlx_ptr,
				game->loading_screen->bordure->img);
		free(game->loading_screen->bordure);
	}
	if (game->loading_screen->center != NULL)
	{
		if (game->loading_screen->center->img != NULL)
			mlx_destroy_image(game->mlx_ptr, game->loading_screen->center->img);
		free(game->loading_screen->center);
	}
	free(game->loading_screen);
	game->loading_screen = NULL;
}
