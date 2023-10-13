/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_mouse_hooks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:16:58 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/13 19:52:06 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	check_press_opt_buttons(t_game *game, int x, int y)
{
	t_option_menu	*opt_menu;
	t_button		*button;
	int				i;

	opt_menu = &game->menu->option_menu;
	i = 0;
	while (i < NB_OPTIONS_BUTTONS)
	{
		button = &opt_menu->buttons[i];
		if (x >= button->pos.x && x <= button->pos.x + button->size.x
			&& y >= button->pos.y && y <= button->pos.y + button->size.y)
		{
			game->menu->state = CHOOSING_KEY_MENU;
			opt_menu->pressed_button = i;
			apply_menu_dark_filter(game->menu->image, DARK_PERCENT_OPTION);
			draw_centered_text_at_y(game, game->menu->image,
				WIN_Y / 2. - game->size_letter.y / 2, KEY_TEXT_CHANGE);
			mlx_hook(game->win, 2, 1, (void *)choose_key_hook, game);
			break ;
		}
		i++;
	}
}

static void	menu_mouse_down_hook_pause_menu(int x, int y, t_game *game)
{
	t_pause_menu	*pause_menu;

	pause_menu = &game->menu->pause_menu;
	if (x >= pause_menu->option_button.pos.x
		&& x <= pause_menu->option_button.pos.x
		+ pause_menu->option_button.size.x
		&& y >= pause_menu->option_button.pos.y
		&& y <= pause_menu->option_button.pos.y
		+ pause_menu->option_button.size.y)
		game->menu->state = OPTION_MENU;
	else if (x >= pause_menu->play_button.pos.x
		&& x <= pause_menu->play_button.pos.x
		+ pause_menu->play_button.size.x
		&& y >= pause_menu->play_button.pos.y
		&& y <= pause_menu->play_button.pos.y
		+ pause_menu->play_button.size.y)
		resume_menu(game, game->menu);
	else if (x >= pause_menu->quit_button.pos.x
		&& x <= pause_menu->quit_button.pos.x
		+ pause_menu->quit_button.size.x
		&& y >= pause_menu->quit_button.pos.y
		&& y <= pause_menu->quit_button.pos.y
		+ pause_menu->quit_button.size.y)
		ft_close(game);
}

static void	menu_mouse_down_hook_option_menu(int x, int y, t_game *game)
{
	t_option_menu	*opt_menu;
	t_button		*exit_button;

	opt_menu = &game->menu->option_menu;
	exit_button = &opt_menu->exit_opt_button;
	if (x >= exit_button->pos.x && x <= exit_button->pos.x
		+ exit_button->size.x && y >= exit_button->pos.y
		&& y < exit_button->pos.y + exit_button->size.y)
		resume_menu(game, game->menu);
	if (x >= opt_menu->slider_fov.pos.x
		&& x <= opt_menu->slider_fov.pos.x
		+ opt_menu->slider_fov.size.x
		+ opt_menu->slider_fov.vert_image->size.x / 2
		&& y >= opt_menu->slider_fov.pos.y
		&& y <= opt_menu->slider_fov.pos.y + opt_menu->slider_fov.size.y)
		opt_menu->pressed_slider_ref = &opt_menu->slider_fov;
	if (x >= opt_menu->slider_sound.pos.x
		&& x <= opt_menu->slider_sound.pos.x
		+ opt_menu->slider_sound.size.x
		+ opt_menu->slider_sound.vert_image->size.x / 2
		&& y >= opt_menu->slider_sound.pos.y
		&& y <= opt_menu->slider_sound.pos.y
		+ opt_menu->slider_sound.size.y)
		opt_menu->pressed_slider_ref = &opt_menu->slider_sound;
}

void	menu_mouse_down_hook(int mouse_button, int x, int y, t_game *game)
{
	if (mouse_button != 1)
		return ;
	if (game->menu->state == PAUSE_MENU)
		menu_mouse_down_hook_pause_menu(x, y, game);
	else if (game->menu->state == OPTION_MENU)
	{
		check_press_opt_buttons(game, x, y);
		menu_mouse_down_hook_option_menu(x, y, game);
	}
}

void	menu_mouse_up_hook(int mouse_button, int x, int y, t_game *game)
{
	(void)x;
	(void)y;
	if (mouse_button != 1)
		return ;
	if (game->menu->state == OPTION_MENU)
		game->menu->option_menu.pressed_slider_ref = NULL;
}
