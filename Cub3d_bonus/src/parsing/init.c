/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 21:50:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"
#include "../../includes/audio.h"

static bool	_init_value_malloc(t_game *game)
{
	game->fov = DFL_FOV;
	game->constants[0] = (WIN_X / 2.) / tanf((DFL_FOV / 2.) * TO_RADIAN);
	game->constants[1] = tanf((DFL_FOV / 2.0) * TO_RADIAN);
	game->constants[2] = cosf((DFL_FOV / 2.0) * TO_RADIAN);
	game->nb_file = 6;
	game->filename = ft_calloc(game->nb_file, sizeof(t_texture));
	if (game->filename == NULL)
		return (print_error(NULL, 0), false);
	game->dist_tab = ft_calloc(WIN_X, sizeof(float));
	if (game->dist_tab == NULL)
		return (print_error(NULL, 0), false);
	game->height_tab = ft_calloc(WIN_X, sizeof(float));
	if (game->height_tab == NULL)
		return (print_error(NULL, 0), false);
	return (true);
}

bool	init_game(t_game *game, char *filename)
{
	if (!_init_value_malloc(game))
		return (ft_close(game), false);
	if (!parse_file(filename, game))
		return (ft_close(game), false);
	if (!check_map(game))
		return (ft_close(game), false);
	if (!init_mlx(game))
		return (print_error(NULL, 0), ft_close(game), false);
	if (!init_audio(game))
		return (ft_close(game), false);
	if (!loading_screen(game))
		return (print_error(NULL, 0), ft_close(game), false);
	if (!init_end_screen(game))
		return (print_error(NULL, 0), ft_close(game), false);
	if (!init_minimap(game))
		return (print_error(NULL, 0), ft_close(game), false);
	if (!init_pause_menu(game))
		return (print_error(NULL, 0), ft_close(game), false);
	return (true);
}

static void	_init_hook(t_game *game)
{
	mlx_do_key_autorepeatoff(game->mlx_ptr);
	mlx_hook(game->win, 2, (1L << 0), (void *)key_press_hook, game);
	mlx_hook(game->win, 3, (1L << 1), (void *)key_release_hook, game);
	mlx_hook(game->win, 4, (1L << 2), mouse_click, game);
	mlx_hook(game->win, 5, (1L << 3), NULL, game);
	mlx_hook(game->win, 6, (1L << 6), mouse_hook, game);
	mlx_hook(game->win, 8, (1L << 5), mouse_leave, game);
	mlx_hook(game->win, 17, (1L << 8), ft_close, game);
	mlx_loop_hook(game->mlx_ptr, on_update, game);
}

bool	init_mlx(t_game *game)
{
	game->mlx_ptr = mlx_init();
	if (game->mlx_ptr == NULL)
		return (false);
	game->win = mlx_new_window(game->mlx_ptr, WIN_X, WIN_Y, "cub3D");
	if (game->win == NULL)
		return (false);
	init_mouse(game);
	game->image = ft_calloc(1, sizeof(t_image));
	if (game->image == NULL)
		return (false);
	game->image->img = mlx_new_image(game->mlx_ptr, WIN_X, WIN_Y);
	if (game->image->img == NULL)
		return (false);
	game->image->addr = mlx_get_data_addr(game->image->img,
			&game->image->opp, &game->image->size_line, &game->image->endian);
	if (game->image->opp != 32)
		return (false);
	game->image->size = (t_vector2){WIN_X, WIN_Y};
	game->image->opp /= 8;
	_init_hook(game);
	return (true);
}

void	move_mouse(t_game *game)
{
	mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
	game->player->mouse_pos.x = WIN_X / 2;
	game->player->mouse_pos.y = WIN_Y / 2;
}

void	init_mouse(t_game *game)
{
	mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
	// mlx_mouse_hide(game->mlx_ptr, game->win);
	game->player->mouse_pos.x = WIN_X / 2;
	game->player->mouse_pos.y = WIN_Y / 2;
}
