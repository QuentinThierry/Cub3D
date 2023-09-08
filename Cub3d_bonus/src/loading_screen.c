/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_screen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 19:04:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/07 20:24:34 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	draw_image_with_transparence(t_image *dest, t_image *src
			, t_vector2 begin_dest, t_vector2 begin_src, t_vector2 size_src)
{
	int	y;
	int	x;
	int	start_dest;
	int	start_src;
	
	y = 0;
	start_dest = begin_dest.y * dest->size_line + begin_dest.x * 4;
	start_src = begin_src.y * src->size_line + begin_src.x * 4;
	while (y < size_src.y)
	{
		x = 0;
		while (x < size_src.x * 4)
		{
			if (*(int*)(src->addr + start_src + x) != GREEN_SCREEN)
				*(int*)(dest->addr + start_dest + x) = *(int*)(src->addr + start_src + x);
			x+=4;
		}
		start_dest += WIN_X * 4;
		start_src += (src->size.x) * 4;
		y++;
	}
}

void print_text(t_game *game, char *str, t_image *alpha, t_vector2 start_pos)
{
	int	i;

	
	i = 0;
	while (str[i])
	{
		draw_image_with_transparence(game->image, alpha, (t_vector2){start_pos.x
			+ (int)(game->size_letter.x * i),start_pos.y}
			, (t_vector2){(int)(game->size_letter.x * (str[i] - '!')), 0}
			, (t_vector2){(int)game->size_letter.x, (int)game->size_letter.y});
		i++;
	}
}

char	*itoa_join(char *str, int nb)
{
	int		power;
	int		i;
	char	*res;
	
	i = 0;
	power = 100;
	res = ft_calloc(ft_strlen(str) + 5, sizeof(char));
	if (res == NULL)
		return (NULL);
	while (str[i])
	{
		res[i] = str[i];
		i++;
	}
	while (power > 0)
	{
		res[i] = nb / power + '0';
		if (!(res[i] == '0' && ft_strlen(str) == i))
			i++;
		nb = nb % power;
		power = power / 10;
	}
	res[i] = '%';
	return (res);
}

bool	update_loading_screen(t_game *game, t_loading *loading_screen)
{
	float	delta;
	char	*text;
	t_vector2	size_bar;
	t_vector2	pos_text;

	loading_screen->nb_image_load++;
	delta = (float)loading_screen->center->size.x / game->nb_images;
	size_bar.x = delta * loading_screen->nb_image_load;
	size_bar.y = loading_screen->center->size.y;
	game->image->addr = ft_memcpy(game->image->addr
		, loading_screen->background->addr, WIN_X * WIN_Y * 4);
	draw_image_with_transparence(game->image, loading_screen->bordure
		, (t_vector2){WIN_X / 3, WIN_Y / 2 - loading_screen->bordure->size.y / 2}
		, (t_vector2){0}, loading_screen->bordure->size);
	draw_image_with_transparence(game->image, loading_screen->center
		, (t_vector2){WIN_X / 3, WIN_Y / 2 - loading_screen->center->size.y / 2}
		, (t_vector2){0}, size_bar);
	text = "Loading...";
	text = itoa_join(text, (int)((float)loading_screen->nb_image_load / game->nb_images * 100));
	if (text == NULL)
		return (false);
	pos_text = (t_vector2){WIN_X / 2 - (int)(ft_strlen(text) * game->size_letter.x / 2.)
		, WIN_Y / 2 - loading_screen->center->size.y / 2 - game->alphabet->size.y};
	print_text(game, text, game->alphabet, pos_text);
	free(text);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	return (true);
}

bool	loading_screen(t_game *game)
{
	t_loading *loading_screen;
	t_vector2	pos_text;
	
	game->loading_screen = ft_calloc(1, sizeof(t_loading));
	if (game->loading_screen == NULL)
		return (false);
	loading_screen = game->loading_screen;
	game->alphabet = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_ALPHABET, (t_vector2){WIN_X, 25});
	if (game->alphabet == NULL)
		return (false);
	game->size_letter = (t_fvector2){game->alphabet->size.x * WIDTH_LETTER / WIDTH_ALPHA, game->alphabet->size.y};
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
		, (t_vector2){0}, loading_screen->bordure->size);
	pos_text = (t_vector2){WIN_X / 2 - (int)(ft_strlen("Loading...0%") * game->size_letter.x / 2.)
		, WIN_Y / 2 - loading_screen->center->size.y / 2 - game->alphabet->size.y};
	print_text(game, "Loading...0%", game->alphabet, pos_text);
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
