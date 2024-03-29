/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/12/19 19:17:03 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	on_update(t_game *game)
{
	if (game->player->angle + game->player->angle >= 360)
		game->player->angle = game->player->angle - 360;
	if (game->player->angle + game->player->angle < 0)
		game->player->angle = game->player->angle + 360;
	player_move(game->player);
	raycasting(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	game;

	game = (t_game){0};
	if (argc != 2)
		return (print_error("Invalid number of arguments\n", 1), 1);
	if (WIN_X < 100 || WIN_Y < 100)
		return (print_error("Invalid window size\n", 1), 1);
	game.filename = ft_calloc(4, sizeof(char *));
	if (game.filename == NULL)
		return (print_error(NULL, 0), 1);
	if (!parse_file(argv[1], &game))
		return (ft_close(&game), 1);
	if (!check_map(&game))
		return (ft_close(&game), 1);
	if (!init_all(&game))
		return (ft_close(&game), 1);
	game.consts = WIN_X / tan((FOV / 2.0) * TO_RADIAN);
	mlx_hook(game.win, 02, (1L << 0), key_press_hook, &game);
	mlx_hook(game.win, 03, (1L << 1), key_release_hook, game.player);
	mlx_hook(game.win, 17, (1L << 5), ft_close, &game);
	mlx_loop_hook(game.mlx_ptr, on_update, &game);
	mlx_loop(game.mlx_ptr);
	return (0);
}
