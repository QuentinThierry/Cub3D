/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_screen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 19:04:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/03 20:36:15 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define LOADING_SCREEN "./assets/smiley.xpm"
#define LOADING_BORDURE "./assets/loading_bordure.xpm"
#include "../includes/cub3d_bonus.h"

// void	*routine(t_game *game)
// {
// 	game->win = mlx_new_window(game->mlx_ptr, WIN_X, WIN_Y, "cub3d");
// 	while (1)
// 	{
// 		mlx_put_image_to_window(game->mlx_ptr, game->win, game->image2->img, 0, 0);
// 		usleep(10000);
// 	}
// 	return (NULL);
// }

// bool	loading_screen(t_game *game)
// {
// 	pthread_t	th;

// 	game->image2 = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/cobble.xpm", (t_vector2){250, 250});
// 	pthread_create(&th, NULL, (void *)routine, game);
// 	pthread_detach(th);
// 	return (true);
// }

bool	loading_screen(t_game *game)
{
	int	i;
	float	tmp;
	int	tmp1;

	i = 0;
	game->loading_screen = ft_calloc(1, sizeof(t_loading));
	if (game->loading_screen == NULL)
		return (false);
	game->loading_screen->background = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_SCREEN, (t_vector2){WIN_X, WIN_Y});
	if (game->loading_screen->background == NULL)
		return (false);
	game->loading_screen->bordure = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_BORDURE, (t_vector2){WIN_X / 3.0 , WIN_Y / 10.0});
	if (game->loading_screen->bordure == NULL)
		return (false);
	game->image->addr = ft_memcpy(game->image->addr
		, game->loading_screen->background->addr, WIN_X * WIN_Y * 4);
	tmp = (WIN_X * WIN_Y / 10.0 + WIN_X / 3.0) * 4;
	tmp1 = 0;
	while (i < WIN_Y / 10.0)
	{
		printf("tmp : %f\n", tmp);
		ft_memcpy(game->image->addr + (int)tmp, game->loading_screen->bordure->addr + tmp1, WIN_X * 4 / 3.0);
		tmp += WIN_X * 4;
		tmp1 += (WIN_X / 3.0) * 4;
		i++;
	}
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	return (true);
}

