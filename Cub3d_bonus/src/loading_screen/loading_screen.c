/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_screen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 19:04:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 15:10:57 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_size_font = (t_vector2){WIN_X, WIN_Y / 16};
static const t_vector2	g_size_background = (t_vector2){WIN_X, WIN_Y};
static const t_vector2	g_size_loading_bar = (t_vector2){WIN_X / 3, WIN_Y / 16};

void	print_text(t_game *game, char *str, t_image *alpha, t_vector2 pos)
{
	int	i;

	if (pos.y + game->subtitle_size.y > game->image->size.y || pos.x < 0
		|| pos.y < 0)
		return ;
	i = -1;
	while (str[++i])
	{
		if (str[i] == 32)
			continue ;
		else if (str[i] < 32 || str[i] > 126)
			draw_image_with_transparence(game->image->addr + (pos.y
					* game->image->size_line + (pos.x
						+ (int)(game->size_letter.x * i)) * 4), alpha,
				(t_vector2){(int)(game->size_letter.x * ('?' - '!')), 0},
				(t_vector2){(int)game->size_letter.x,
				(int)game->size_letter.y});
		else
			draw_image_with_transparence(game->image->addr + (pos.y
					* game->image->size_line + (pos.x
						+ (int)(game->size_letter.x * i)) * 4), alpha,
				(t_vector2){(int)(game->size_letter.x * (str[i] - '!')), 0},
				(t_vector2){(int)game->size_letter.x,
				(int)game->size_letter.y});
	}
}

static char	*_itoa_join(char *str, int nb)
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

static bool	_draw_text_loading(t_game *game, t_loading *screen)
{
	char		*text;
	t_vector2	pos_text;

	text = "Loading...";
	text = _itoa_join(text, screen->nb_image_load * 100. / game->nb_images);
	if (text == NULL)
		return (false);
	pos_text = (t_vector2){WIN_X / 2 - (int)(ft_strlen(text)
			* game->size_letter.x / 2),
		WIN_Y / 2 - screen->center->size.y / 2 - game->font->size.y};
	print_text(game, text, game->font, pos_text);
	free(text);
	return (true);
}

bool	update_loading_screen(t_game *game, t_loading *screen)
{
	t_vector2	size_bar;

	screen->nb_image_load++;
	size_bar.x = screen->center->size.x * screen->nb_image_load
		/ game->nb_images;
	size_bar.y = screen->center->size.y;
	game->image->addr = ft_memcpy(game->image->addr, screen->background->addr,
			WIN_X * WIN_Y * 4);
	draw_image_with_green_sreen(game->image->addr
		+ ((WIN_Y / 2 - screen->bordure->size.y / 2) * game->image->size_line
			+ (WIN_X / 3) * 4), screen->bordure, (t_vector2){0},
		g_size_loading_bar);
	draw_image_with_green_sreen(game->image->addr
		+ ((WIN_Y / 2 - screen->center->size.y / 2) * game->image->size_line
			+ (WIN_X / 3) * 4), screen->center, (t_vector2){0}, size_bar);
	if (!_draw_text_loading(game, screen))
		return (false);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	return (true);
}
