/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_screen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 19:04:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/04 15:23:43 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define LOADING_SCREEN "./assets/smiley.xpm"
#define LOADING_BORDURE "./assets/loading_bordure.xpm"
#define LOADING_CENTER "./assets/loading_center.xpm"
#define GREEN_SCEEN 0x00ff00
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
__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, int color)
{
	*(int*)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

void	draw_image_with_transparence(t_image *dest, t_image *src, t_vector2 begin)
{
	int	y;
	int	x;
	int	star_img;
	int	start_loading;
	
	y = 0;
	star_img = begin.y * dest->size_line + begin.x * 4;
	// star_img = (WIN_X * (WIN_Y/2 - src->size.y / 2) + src->size.x) * 4;
	start_loading = 0;
	while (y < src->size.y)
	{
		x = 0;
		while (x < src->size.x * 4)
		{
			if (*(int*)(src->addr + start_loading + x) != GREEN_SCEEN)
				*(int*)(dest->addr + star_img + x) = *(int*)(src->addr + start_loading + x);
			x+=4;
		}
		star_img += WIN_X * 4;
		start_loading += (src->size.x) * 4;
		y++;
	}
}

bool	loading_screen(t_game *game, t_loading *loading_screen)
{
	loading_screen = ft_calloc(1, sizeof(t_loading));
	if (loading_screen == NULL)
		return (false);
	loading_screen->background = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_SCREEN, (t_vector2){WIN_X, WIN_Y});
	if (loading_screen->background == NULL)
		return (false);
	loading_screen->bordure = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_BORDURE,(t_vector2){WIN_X / 3, WIN_Y / 16});
	if (loading_screen->bordure == NULL)
		return (false);
	loading_screen->center = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_CENTER,(t_vector2){WIN_X / 3, WIN_Y / 16});
	if (loading_screen->center == NULL)
		return (false);
	game->image->addr = ft_memcpy(game->image->addr
		, loading_screen->background->addr, WIN_X * WIN_Y * 4);
	draw_image_with_transparence(game->image, loading_screen->bordure
		, (t_vector2){WIN_X / 3, WIN_Y / 2 - loading_screen->bordure->size.y / 2});
	// draw_image_with_transparence(game->image, loading_screen->center
	// 	, (t_vector2){WIN_X / 3, WIN_Y / 2 - loading_screen->center->size.y / 2});
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	return (true);
}

