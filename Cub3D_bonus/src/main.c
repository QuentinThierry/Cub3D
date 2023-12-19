/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/12/04 15:45:15 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline void	_update_screen(t_game *game)
{
	raycasting(game);
	if (game->player->has_item == true)
		draw_hand_item(game, game->player);
	zoom_hook_handle(game->minimap, game->delta_time);
	draw_minimap(game);
	print_subtitle(game, game->music_array[1].map_cell);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
}

int	on_update(t_game *game)
{
	struct timespec			cur_time;
	struct timespec			time;

	if (game->last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &game->last_time);
	clock_gettime(CLOCK_REALTIME, &time);
	game->time = time_to_long(&time);
	update_sounds(game->music_array);
	if ((game->map[(int)game->player->f_pos.y][(int)game->player->f_pos.x].type
		& OBJ_INTER) == OBJ_INTER)
		take_object(game, game->player, &game->map[(int)game->player->f_pos.y]
		[(int)game->player->f_pos.x], game->music_array);
	player_move(game, game->player, game->delta_time, game->map);
	if (game->player->angle + game->player->angle >= 360)
		game->player->angle = game->player->angle - 360;
	if (game->player->angle + game->player->angle < 0)
		game->player->angle = game->player->angle + 360;
	update_doors(game->door_array, game->nb_doors, game->delta_time, game->map);
	_update_screen(game);
	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - game->last_time.tv_sec
			+ (cur_time.tv_nsec - game->last_time.tv_nsec) / 1000000000.F);
	game->last_time = cur_time;
	return (0);
}
void	move_mouse(t_game *game);
void	play_first_music(t_game *game);

int	main(int argc, char **argv)
{
	t_game	game;

	game = (t_game){0};
	if (argc != 2)
		return (print_error("Invalid number of arguments\n", 1), 1);
	if (WIN_X < 100 || WIN_Y < 100)
		return (print_error("Invalid window size\n", 1), 1);
	if (!init_game(&game, argv[1]))
		return (1);
	if (!load_image_tab(&game))
		return (ft_close(&game), 1);
	free_filename(&game);
	ft_memcpy(game.image->addr, game.loading_screen->background->addr,
		WIN_X * WIN_Y * 4);
	free_loading_screen(&game);
	exit_door_no_receptacle(game.exit, game.total_receptacle, game.tab_images);
	PlayMusicStream(game.music_array[0].music);
	move_mouse(&game);
	set_pause_menu_mode(&game);
	play_first_music(&game);
	mlx_loop(game.mlx_ptr);
	return (0);
}
