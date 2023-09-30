/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/30 14:18:12 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

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
	else if (key == game->keybinds[e_key_interact_door])
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


int	exit_hook(int key, t_game *game)
{
	if (key == 65307) // esc
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

void	player_move(t_game *game, t_player *player, double delta_time, t_map **map)
{
	t_dvector2 move_value;

	if (player->view != 0)
		player->angle += ROTATION_KEYBOARD * delta_time * player->view;
	move_value.x = 0;
	move_value.y = 0;
	if (player->dir.y != 0)
	{
		move_value.x -= sinf(player->angle * TO_RADIAN) * player->speed * player->dir.y;
		move_value.y += cosf(player->angle * TO_RADIAN) * player->speed * player->dir.y;
	}
	if (player->dir.x != 0)
	{
		move_value.x += cosf(player->angle * TO_RADIAN) * player->speed * player->dir.x;
		move_value.y += sinf(player->angle * TO_RADIAN) * player->speed * player->dir.x;
	}
	if (player->dir.x != 0 && player->dir.y != 0)
	{
		move_value.x *= 0.707;
		move_value.y *= 0.707;
	}
	if (player->dir.x != 0 || player->dir.y != 0)
	{
		move_value.x = player->f_real_pos.x + move_value.x * delta_time;
		move_value.y = player->f_real_pos.y + move_value.y * delta_time;
		move_value = check_colliding(game, move_value, map);
		if (((int)move_value.x != (int)player->f_real_pos.x || (int)move_value.y != (int)player->f_real_pos.y)
			&& (map[(int)move_value.y][(int)move_value.x].type & MUSIC) == MUSIC)
			play_music(&map[(int)move_value.y][(int)move_value.x]
				, game->music_array, map[(int)move_value.y][(int)move_value.x].music, IS_PLAYING_MUSIC);
		if (((int)move_value.x != (int)player->f_real_pos.x || (int)move_value.y != (int)player->f_real_pos.y)
			&& (map[(int)move_value.y][(int)move_value.x].type & NARRATOR) == NARRATOR)
		{
			play_narrator(game, &map[(int)move_value.y][(int)move_value.x], game->music_array);
			map[(int)move_value.y][(int)move_value.x].type ^= NARRATOR;
		}
		game->player->f_real_pos = move_value;
	}
}

int	mouse_hook(int x, int y, t_game *game)
{
	game->player->angle -= (double)(game->player->mouse_pos.x - x) / ROTATION_MOUSE;
	if (x != WIN_X / 2 || y != WIN_Y / 2)
	{
		mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
		game->player->mouse_pos.x = WIN_X / 2;
		game->player->mouse_pos.y = WIN_Y / 2;
	}
	return (0);
}

int	mouse_stay_in_window_hook(int x, int y, t_game *game)
{
	if (x != WIN_X / 2 || y != WIN_Y / 2)
	{
		mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
		game->player->mouse_pos.x = WIN_X / 2;
		game->player->mouse_pos.y = WIN_Y / 2;
	}
	return (0);
}

int	mouse_leave(t_game *game)
{
	mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
	game->player->mouse_pos.x = WIN_X / 2;
	game->player->mouse_pos.y = WIN_Y / 2;
	return (0);
}

int	mouse_click(int button, int x, int y,t_game *game)
{
	(void)x;
	(void)y;
	if (button == 1)
	{
		if (game->player->has_item == true)
			drop_object(game->player, game->map, game->exit, game);
		else
			take_object_click(game, game->player, game->map);
	}
	return (0);
}
