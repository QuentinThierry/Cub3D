/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subtitle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 12:38:01 by jvigny            #+#    #+#             */
/*   Updated: 2023/12/03 14:49:23 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

unsigned int	draw_subtitle_with_backgroud(t_game *game, t_image *image,
					t_vector2 pos, const char *text);

static const t_vector2	g_minimap_size = {
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE
};

static const t_vector2	g_minimap_pos = {
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y)
	- (((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE)
	- ((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD
};

static int	_find_next_line(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
		i++;
	return (i + 1);
}

static bool	_update_subtitle(t_game *game, t_music_name *narrator)
{
	narrator->time = game->time;
	narrator->offset += _find_next_line(narrator->subtitle + narrator->offset);
	if (narrator->offset >= (unsigned int)ft_strlen(narrator->subtitle))
	{
		game->music_array[1].is_subtitle = false;
		narrator->offset = 0;
		return (false);
	}
	return (true);
}

void	print_subtitle(t_game *game, t_map *map_cell)
{
	unsigned int	i;
	t_music_name	*narrator;
	const t_vector2	first_line = {
		g_minimap_pos.x + g_minimap_size.x + game->subtitle_size.x * 2,
		g_minimap_pos.y + g_minimap_size.y / 2.};
	const t_vector2	second_line = {
		g_minimap_pos.x + g_minimap_size.x + game->subtitle_size.x * 2,
		g_minimap_pos.y + (g_minimap_size.y / 2.)
		+ game->subtitle_size.y};

	if (game->music_array[1].is_subtitle == false || map_cell == NULL
		|| map_cell->narrator == NULL || map_cell->narrator->subtitle == NULL)
		return ;
	narrator = map_cell->narrator;
	if (game->time - narrator->time > SUBTITLE_TIME)
		if (!_update_subtitle(game, narrator))
			return ;
	i = draw_subtitle_with_backgroud(game, game->image, first_line,
			narrator->subtitle + narrator->offset);
	if (*(narrator->subtitle + narrator->offset + i) != '\n'
		&& *(narrator->subtitle + narrator->offset + i) != '\0')
		draw_subtitle_with_backgroud(game, game->image, second_line,
			narrator->subtitle + narrator->offset + i);
}
