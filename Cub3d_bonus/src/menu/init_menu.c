/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:53:39 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/11 15:34:33 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_button_size =
{
	WIN_X / 8,
	WIN_Y / 10
};

static const int g_button_pos_left_offset_x =
	WIN_X / 30 + (WIN_X / 8) * 2;

static const int	g_inter_button_y =
	(WIN_Y - (WIN_Y / 10) * NB_OPTIONS_BUTTONS / 2) / (NB_OPTIONS_BUTTONS * 2);

static const char	*g_description_opt_button[NB_OPTIONS_BUTTONS] =
	{"Forward", "Left", "Backward", "Right", "Look Left", "Look Right",
	"Pause", "Map Zoom", "Map Unzoom", "Door Interact", "Sprint"};

static const char	*g_description_slider_button[NB_SLIDERS] =
	{"FOV", "VOLUME"};

static const t_vector2 g_exit_button_pos =
{
	WIN_X / 100,
	WIN_Y / 100
};

static const t_vector2 g_exit_button_size =
{
	WIN_X / 20,
	WIN_Y / 20
};

static const t_vector2 g_slider_fov_pos =
{
	(WIN_X / 8),
	((WIN_Y / 10) * (NB_OPTIONS_BUTTONS / 2 + (NB_OPTIONS_BUTTONS / 2) % 2 + 2))
};

static const t_vector2 g_slider_sound_pos =
{
	(WIN_X / 8) + WIN_X / 2,
	((WIN_Y / 10) * (NB_OPTIONS_BUTTONS / 2 + (NB_OPTIONS_BUTTONS / 2) % 2 + 2))
};

static const t_vector2 g_slider_hor_size =
{
	WIN_X / 5,
	WIN_Y / 50
};

static const t_vector2 g_slider_vert_size =
{
	WIN_X / 75,
	WIN_Y / 20
};

static const t_vector2 g_vert_bar_pos =
{
	WIN_X / 2 - WIN_X / 400,
	WIN_Y / 20
};

static const t_vector2 g_vert_bar_size =
{
	WIN_X / 200,
	((WIN_Y / 10) * (NB_OPTIONS_BUTTONS / 2 + (NB_OPTIONS_BUTTONS / 2) % 2 + 1))
	+ WIN_Y / 20 - (WIN_Y / 20) * 2
};

static const t_vector2 g_hor_bar_pos =
{
	WIN_X / 20,
	((WIN_Y / 10) * (NB_OPTIONS_BUTTONS / 2 + (NB_OPTIONS_BUTTONS / 2) % 2 + 1))
	+ WIN_Y / 20
};

static const t_vector2 g_hor_bar_size =
{
	WIN_X - (WIN_X / 20) * 2,
	(WIN_Y / 100)
};

bool	init_keybinds(t_game *game)
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
	*button_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/button.xpm", g_button_size);
	*button_hovered_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/button_hovered.xpm", g_button_size);
	game->menu->image = btmlx_new_image(game->mlx_ptr, (t_vector2){WIN_X, WIN_Y});
	game->menu->background_image = btmlx_new_image(game->mlx_ptr, (t_vector2){WIN_X, WIN_Y});
	game->menu->option_menu.exit_opt_button.base_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/button_exit_option.xpm", g_exit_button_size);
	game->menu->option_menu.slider_fov.hor_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/slider_hor.xpm", g_slider_hor_size);
	game->menu->option_menu.slider_fov.vert_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/slider_vert.xpm", g_slider_vert_size);
	game->menu->option_menu.slider_sound.hor_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/slider_hor.xpm", g_slider_hor_size);
	game->menu->option_menu.slider_sound.vert_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/slider_vert.xpm", g_slider_vert_size);
	return (*button_image && *button_hovered_image && game->menu->image
		&& game->menu->background_image
		&& game->menu->option_menu.exit_opt_button.base_image
		&& game->menu->option_menu.slider_fov.hor_image
		&& game->menu->option_menu.slider_fov.vert_image
		&& game->menu->option_menu.slider_sound.hor_image
		&& game->menu->option_menu.slider_sound.vert_image);
}

static void	fill_key_button(t_game *game, t_option_menu *opt_menu,
		t_image *button_image, t_image *button_hovered_image)
{
	int	i;

	i = 0;
	while (i < NB_OPTIONS_BUTTONS)
	{
		opt_menu->buttons[i].base_image = button_image;
		opt_menu->buttons[i].size = g_button_size;
		opt_menu->buttons[i].hovered_image = button_hovered_image;
		opt_menu->buttons[i].text = get_key_str(game->keybinds[i]);
		opt_menu->buttons[i].linked_text = g_description_opt_button[i];
		if (i < NB_OPTIONS_BUTTONS / 2 + (NB_OPTIONS_BUTTONS & 1))
		{
			opt_menu->buttons[i].pos
				= (t_vector2){g_button_pos_left_offset_x,
				i * g_button_size.y + g_inter_button_y * (i + 1)};
		}
		else
		{
			opt_menu->buttons[i].pos
				= (t_vector2){WIN_X / 2 + g_button_pos_left_offset_x,
				(i - NB_OPTIONS_BUTTONS / 2 - (NB_OPTIONS_BUTTONS & 1))
				* g_button_size.y + g_inter_button_y
				* (i - NB_OPTIONS_BUTTONS / 2 + ((NB_OPTIONS_BUTTONS & 1) == 0))};
		}
		i++;
	}
}

static void	fill_option_button(t_game *game, t_option_menu *opt_menu,
		t_image *button_image, t_image *button_hovered_image)
{
	fill_key_button(game, opt_menu, button_image, button_hovered_image);
	opt_menu->exit_opt_button.hovered_image = button_hovered_image;
	opt_menu->exit_opt_button.pos = g_exit_button_pos;
	opt_menu->exit_opt_button.size = g_exit_button_size;
	opt_menu->exit_opt_button.text = "";
	opt_menu->slider_fov.size = (t_vector2){g_slider_hor_size.x, g_slider_vert_size.y};
	opt_menu->slider_fov.pos = g_slider_fov_pos;
	game->menu->option_menu.slider_fov.percent = (float)
		(DFL_FOV - MIN_FOV) / (MAX_FOV - MIN_FOV);
	game->menu->option_menu.slider_fov.min_max_value = (t_vector2){MIN_FOV, MAX_FOV};
	game->menu->option_menu.slider_fov.linked_text = g_description_slider_button[0];
	opt_menu->slider_sound.size = (t_vector2){g_slider_hor_size.x, g_slider_vert_size.y};
	opt_menu->slider_sound.pos = g_slider_sound_pos;
	game->menu->option_menu.slider_sound.percent = DFL_SOUND;
	game->menu->option_menu.slider_sound.min_max_value = (t_vector2){0, 100};
	game->menu->option_menu.slider_sound.linked_text = g_description_slider_button[1];
	opt_menu->vert_bar_pos = g_vert_bar_pos;
	opt_menu->vert_bar_size = g_vert_bar_size;
	opt_menu->hor_bar_pos = g_hor_bar_pos;
	opt_menu->hor_bar_size = g_hor_bar_size;
}

static void	fill_pause_menu_button(t_pause_menu *pause_menu,
		t_image *button_image, t_image *button_hovered_image)
{
	pause_menu->play_button.base_image = button_image;
	pause_menu->play_button.hovered_image = button_hovered_image;
	pause_menu->play_button.size = pause_menu->play_button.base_image->size;
	pause_menu->play_button.pos
		= (t_vector2){WIN_X / 2 - pause_menu->play_button.size.x / 2,
		WIN_Y / 3 - pause_menu->play_button.size.y / 2};
	pause_menu->play_button.text = "RESUME";
	pause_menu->option_button.base_image = button_image;
	pause_menu->option_button.hovered_image = button_hovered_image;
	pause_menu->option_button.size = pause_menu->option_button.base_image->size;
	pause_menu->option_button.pos
		= (t_vector2){WIN_X / 2 - pause_menu->option_button.size.x / 2,
		WIN_Y / 2 - pause_menu->option_button.size.y / 2};
	pause_menu->option_button.text = "OPTIONS";
	pause_menu->quit_button.base_image = button_image;
	pause_menu->quit_button.hovered_image = button_hovered_image;
	pause_menu->quit_button.size = pause_menu->quit_button.base_image->size;
	pause_menu->quit_button.pos
		= (t_vector2){WIN_X / 2 - pause_menu->quit_button.size.x / 2,
		WIN_Y / 3 * 2 - pause_menu->quit_button.size.y / 2};
	pause_menu->quit_button.text = "QUIT GAME";
}

bool	init_pause_menu(t_game *game)
{
	t_image			*button_image;
	t_image			*button_hovered_image;

	if (!allocate_menu(game, &button_image, &button_hovered_image))
	{
		return (free_image(game->mlx_ptr, button_image),
			free_image(game->mlx_ptr, button_hovered_image), false);
	}
	fill_option_button(game, &game->menu->option_menu, button_image, button_hovered_image);
	fill_pause_menu_button(&game->menu->pause_menu, button_image, button_hovered_image);
	return (true);
}
