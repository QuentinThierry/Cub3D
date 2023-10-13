/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 17:18:02 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static void	_key_press_bis(t_keybind key, t_game *game)
{
	if (key == game->keybinds[e_key_interact_door])
		open_door(game);
	else if (key == game->keybinds[e_key_pause])
		set_pause_menu_mode(game);
	else if (key == 't')
	{
		game->fov += 1;
		game->constants[0] = (WIN_X / 2.) / tan((game->fov / 2.) * TO_RADIAN);
		game->constants[1] = tanf((game->fov / 2.0) * TO_RADIAN);
		game->constants[2] = cos((game->fov / 2.0) * TO_RADIAN);
	}
	else if (key == 'y')
	{
		game->fov -= 1;
		game->constants[0] = (WIN_X / 2.) / tan((game->fov / 2.) * TO_RADIAN);
		game->constants[1] = tanf((game->fov / 2.0) * TO_RADIAN);
		game->constants[2] = cos((game->fov / 2.0) * TO_RADIAN);
	}
}

void	key_press_hook(t_keybind key, t_game *game)
{
	if (key == XK_Escape)
		ft_close(game);
	else if (key == game->keybinds[e_key_left_look])
		game->player->view -= 1;
	else if (key == game->keybinds[e_key_right_look])
		game->player->view += 1;
	else if (key == game->keybinds[e_key_right_move])
		game->player->dir.x += 1;
	else if (key == game->keybinds[e_key_left_move])
		game->player->dir.x -= 1;
	else if (key == game->keybinds[e_key_forward_move])
		game->player->dir.y -= 1;
	else if (key == game->keybinds[e_key_backward_move])
		game->player->dir.y += 1;
	else if (key == game->keybinds[e_key_sprint])
		game->player->speed += SPRINT_BOOST;
	else if (key == game->keybinds[e_key_minimap_dezoom])
		game->minimap->zoom_dir -= 1;
	else if (key == game->keybinds[e_key_minimap_zoom])
		game->minimap->zoom_dir += 1;
	_key_press_bis(key, game);
}

int	exit_hook(int key, t_game *game)
{
	if (key == 65307)
		ft_close(game);
	return (0);
}

void	key_release_hook(t_keybind key, t_game *game)
{
	if (key == game->keybinds[e_key_left_look])
		game->player->view += 1;
	else if (key == game->keybinds[e_key_right_look])
		game->player->view -= 1;
	else if (key == game->keybinds[e_key_right_move])
		game->player->dir.x -= 1;
	else if (key == game->keybinds[e_key_left_move])
		game->player->dir.x += 1;
	else if (key == game->keybinds[e_key_forward_move])
		game->player->dir.y += 1;
	else if (key == game->keybinds[e_key_backward_move])
		game->player->dir.y -= 1;
	else if (key == game->keybinds[e_key_sprint])
		game->player->speed -= SPRINT_BOOST;
	else if (key == game->keybinds[e_key_minimap_dezoom])
		game->minimap->zoom_dir += 1;
	else if (key == game->keybinds[e_key_minimap_zoom])
		game->minimap->zoom_dir -= 1;
}
