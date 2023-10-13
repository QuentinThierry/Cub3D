/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu_images.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 20:21:56 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/13 20:25:37 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_button_size = {
	WIN_X / 8,
	WIN_Y / 10
};

static const t_vector2	g_exit_size = {
	WIN_X / 20,
	WIN_Y / 20
};

static const t_vector2	g_slider_hor_size = {
	WIN_X / 5,
	WIN_Y / 50
};

static const t_vector2	g_slider_vert_size = {
	WIN_X / 75,
	WIN_Y / 20
};

static bool	init_keybinds(t_game *game)
{
	game->keybinds = ft_calloc(NB_OPTIONS_BUTTONS, sizeof(int));
	if (!game->keybinds)
		return (false);
	game->keybinds[e_key_left_move] = DFL_KEY_LEFT_MOVE;
	game->keybinds[e_key_right_move] = DFL_KEY_RIGHT_MOVE;
	game->keybinds[e_key_forward_move] = DFL_KEY_FORWARD_MOVE;
	game->keybinds[e_key_backward_move] = DFL_KEY_BACKWARD_MOVE;
	game->keybinds[e_key_left_look] = DFL_KEY_LEFT_LOOK;
	game->keybinds[e_key_right_look] = DFL_KEY_RIGHT_LOOK;
	game->keybinds[e_key_pause] = DFL_KEY_PAUSE;
	game->keybinds[e_key_minimap_zoom] = DFL_KEY_MINIMAP_ZOOM;
	game->keybinds[e_key_minimap_dezoom] = DFL_KEY_MINIMAP_DEZOOM;
	game->keybinds[e_key_interact_door] = DFL_KEY_INTERACT_DOOR;
	game->keybinds[e_key_sprint] = DFL_KEY_SPRINT;
	return (true);
}

static bool	allocate_button_images(t_game *game,
	t_image **button_image, t_image **button_hovered_image)
{
	*button_image = btmlx_xpm_file_to_image(
			game->mlx_ptr, XPM_DFL_BUTTON, g_button_size);
	*button_hovered_image = btmlx_xpm_file_to_image(
			game->mlx_ptr, XPM_HOV_BUTTON, g_button_size);
	game->menu->image
		= btmlx_new_image(game->mlx_ptr, (t_vector2){WIN_X, WIN_Y});
	game->menu->background_image = btmlx_new_image(
			game->mlx_ptr, (t_vector2){WIN_X, WIN_Y});
	game->menu->option_menu.exit_opt_button.base_image
		= btmlx_xpm_file_to_image(game->mlx_ptr, XPM_EXIT_BUTTON, g_exit_size);
	game->menu->option_menu.slider_fov.hor_image = btmlx_xpm_file_to_image(
			game->mlx_ptr, XPM_HOR_SLIDER, g_slider_hor_size);
	game->menu->option_menu.slider_fov.vert_image = btmlx_xpm_file_to_image(
			game->mlx_ptr, XPM_VERT_SLIDER, g_slider_vert_size);
	game->menu->option_menu.slider_sound.hor_image = btmlx_xpm_file_to_image(
			game->mlx_ptr, XPM_HOR_SLIDER, g_slider_hor_size);
	game->menu->option_menu.slider_sound.vert_image = btmlx_xpm_file_to_image(
			game->mlx_ptr, XPM_VERT_SLIDER, g_slider_vert_size);
	return (*button_image && *button_hovered_image && game->menu->image
		&& game->menu->background_image
		&& game->menu->option_menu.exit_opt_button.base_image
		&& game->menu->option_menu.slider_fov.hor_image
		&& game->menu->option_menu.slider_fov.vert_image
		&& game->menu->option_menu.slider_sound.hor_image
		&& game->menu->option_menu.slider_sound.vert_image);
}

bool	allocate_menu(t_game *game,
			t_image **button_image, t_image **button_hovered_image)
{
	if (!init_keybinds(game))
		return (false);
	game->menu = ft_calloc(1, sizeof(t_menu));
	if (!game->menu)
		return (false);
	game->menu->h_rgb_blur_buffer = ft_calloc(WIN_X * WIN_Y * 3, sizeof(int));
	game->menu->v_rgb_blur_buffer = ft_calloc(WIN_X * WIN_Y * 3, sizeof(int));
	if (!game->menu->h_rgb_blur_buffer || !game->menu->v_rgb_blur_buffer)
		return (false);
	return (allocate_button_images(game, button_image, button_hovered_image));
}