/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_screen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 19:04:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/04 16:45:15 by jvigny           ###   ########.fr       */
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

void	draw_image_with_transparence(t_image *dest, t_image *src, t_vector2 begin, t_vector2 size)
{
	int	y;
	int	x;
	int	start_dest;
	int	start_src;
	
	y = 0;
	start_dest = begin.y * dest->size_line + begin.x * 4;
	start_src = 0;
	while (y < size.y)
	{
		x = 0;
		while (x < size.x * 4)
		{
			if (*(int*)(src->addr + start_src + x) != GREEN_SCEEN)
				*(int*)(dest->addr + start_dest + x) = *(int*)(src->addr + start_src + x);
			x+=4;
		}
		start_dest += WIN_X * 4;
		start_src += (src->size.x) * 4;
		y++;
	}
}

void	update_loading_screen(t_game *game, t_loading *loading_screen)
{
	float	delta;
	t_vector2	size_bar;

	loading_screen->nb_image_load++;
	delta = (float)loading_screen->center->size.x / game->nb_images;
	size_bar.x = delta * loading_screen->nb_image_load;
	size_bar.y = loading_screen->center->size.y;
	game->image->addr = ft_memcpy(game->image->addr
		, loading_screen->background->addr, WIN_X * WIN_Y * 4);
	draw_image_with_transparence(game->image, loading_screen->bordure
		, (t_vector2){WIN_X / 3, WIN_Y / 2 - loading_screen->bordure->size.y / 2}, loading_screen->bordure->size);
	
	draw_image_with_transparence(game->image, loading_screen->center
		, (t_vector2){WIN_X / 3, WIN_Y / 2 - loading_screen->center->size.y / 2}, size_bar);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
}

bool	loading_screen(t_game *game)
{
	t_loading *loading_screen;
	
	game->loading_screen = ft_calloc(1, sizeof(t_loading));
	if (game->loading_screen == NULL)
		return (false);
	loading_screen = game->loading_screen;
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
		, (t_vector2){WIN_X / 3, WIN_Y / 2 - loading_screen->bordure->size.y / 2}
		, loading_screen->bordure->size);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	return (true);
}

void free_loading_screen(t_game *game)
{
	if (game->loading_screen != NULL)
	{
		if (game->loading_screen->background != NULL)
		{
			if (game->loading_screen->background->img != NULL)	
				mlx_destroy_image(game->mlx_ptr, game->loading_screen->background->img);
			free(game->loading_screen->background);
		}
		if (game->loading_screen->bordure != NULL)
		{
			if (game->loading_screen->bordure->img != NULL)	
				mlx_destroy_image(game->mlx_ptr, game->loading_screen->bordure->img);
			free(game->loading_screen->bordure);
		}
		if (game->loading_screen->center != NULL)
		{
			if (game->loading_screen->center->img != NULL)	
				mlx_destroy_image(game->mlx_ptr, game->loading_screen->center->img);
			free(game->loading_screen->center);
		}
		free(game->loading_screen);
		game->loading_screen = NULL;
	}
}
