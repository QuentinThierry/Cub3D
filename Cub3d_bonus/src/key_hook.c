/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:26:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/06 16:48:23 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	key_press_hook(int key, t_game *game)
{
	if (key == 65307 ) // esc
		ft_close(game);
	else if (key == 65361) // left rrow
		game->player->view -= 1;
	else if (key == 65363) // right arrow
		game->player->view += 1;
	else if (key == 'd')
		game->player->dir.x += 1;
	else if (key == 'a' || key == 'q')
		game->player->dir.x -= 1;
	else if (key == 'w' || key == 'z')
		game->player->dir.y -= 1;
	else if (key == 's')
		game->player->dir.y += 1;
	else if (key == 65505) // shift
		game->player->speed += SPRINT_BOOST;
	else if (key == '-' || key == 65453)
		game->minimap->zoom_dir -= 1;
	else if (key == '=' || key == '+' || key == 65451)
		game->minimap->zoom_dir += 1;
	else if (key == 'i')
	{
		for (int y = 0; y < game->map_size.y; y++)
		{
			for (int x = 0; x < game->map_size.x; x++)
			{
				if (game->map[y][x].symbol == 'c')
				{
					printf("%d\n", game->map[y][x].door_percent);
					game->map[y][x].door_percent--;
					if (game->map[y][x].door_percent < 0)
						game->map[y][x].door_percent = 0;
				}
			}
		}
	}
	else if (key == 'o')
	{
		for (int y = 0; y < game->map_size.y; y++)
		{
			for (int x = 0; x < game->map_size.x; x++)
			{
				if (game->map[y][x].symbol == 'c')
				{
					printf("%d\n", game->map[y][x].door_percent);
					game->map[y][x].door_percent++;
					if (game->map[y][x].door_percent > 90)
						game->map[y][x].door_percent = 90;
				}
			}
		}
	}
	return (0);
}

int	key_release_hook(int key, t_game *game)
{
	if (key == 65361) // left rrow
		game->player->view += 1;
	if (key == 65363) // right arrow
		game->player->view -= 1;
	if (key == 'd')
		game->player->dir.x -= 1;
	if (key == 'a' || key == 'q')
		game->player->dir.x += 1;
	if (key == 'w' || key == 'z')
		game->player->dir.y += 1;
	if (key == 's')
		game->player->dir.y -= 1;
	if (key == 65505) // shift
		game->player->speed -= SPRINT_BOOST;
	else if (key == '-' || key == 65453)
		game->minimap->zoom_dir += 1;
	else if (key == '=' || key == '+' || key == 65451)
		game->minimap->zoom_dir -= 1;
	return (0);
}

void	player_move(t_player *player, double delta_time, t_map **map)
{
	t_fvector2 move_value;

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
	if (player->dir.x != 0 || player->dir.y != 0)
	{
		move_value.x *= 0.707;
		move_value.y *= 0.707;
	}
	if (player->dir.x != 0 || player->dir.y != 0)
	{
		move_value.x = player->f_pos.x + move_value.x * delta_time;
		move_value.y = player->f_pos.y + move_value.y * delta_time;
		check_colliding(player, move_value, map);
	}
}

int mouse_hook(int x, int y, t_game *game)
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

int	mouse_leave(t_game *game)
{
	mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
	game->player->mouse_pos.x = WIN_X / 2;
	game->player->mouse_pos.y = WIN_Y / 2;
	return (0);
}
