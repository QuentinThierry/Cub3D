/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subtitle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 12:38:01 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/30 18:58:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"
#define SUBTITLE_TIME 5000
#define MAX_CARACTER 100

static const t_vector2	g_minimap_size =
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE
};

static const t_vector2	g_minimap_pos = 
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) - 
	(((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE) -
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD
};

int	find_max_caracter(const char *text, bool *is_word_cut)
{
	int i;

	i = 0;
	*is_word_cut = false;
	while (text[i] != '\0' && text[i] != '\n' && i < MAX_CARACTER)
		i++;
	if (text[i] == '\0' || text[i] == '\n' || text[i] == ' ')
		return (i);
	if (i > 0 && text[i - 1] == ' ')
		return (i);
	if (i > 1 && text[i - 2] == ' ')
		return (i - 1);
	*is_word_cut = true;
	return (i);
}

unsigned int	draw_subtitle_with_backgroud(t_game *game, t_image *image, t_vector2 pos, const char *text)
{
	int		i;
	int		len;
	bool	is_word_cut;

	if (game->subtitle_size_letter.y + pos.y > image->size.y)
		return (0);
	len = find_max_caracter(text, &is_word_cut);
	draw_alpha_rectangle(image, (t_vector2){pos.x - game->subtitle_size_letter.x / 2,
		pos.y}, (t_vector2){(len + 1 + is_word_cut) * game->subtitle_size_letter.x,
		game->subtitle_size_letter.y});
	i = 0;
	while (i < len)
	{
		if ((i + 1) * game->subtitle_size_letter.x + pos.x > image->size.x)
			return (i);
		if (text[i] == ' ')
		{
			i++;
			continue ;
		}
		else if (text[i] < 32 || text[i] > 126)
			draw_image_with_transparence(image->addr + (pos.y * image->size_line + (pos.x
				+ (int)(game->subtitle_size_letter.x * i)) * 4), game->subtitle_font,
			(t_vector2){(game->subtitle_size_letter.x * ('?' - '!')), 0},
			(t_vector2){game->subtitle_size_letter.x, game->subtitle_size_letter.y});
		else
			draw_image_with_transparence(image->addr + (pos.y * image->size_line + (pos.x
				+ (int)(game->subtitle_size_letter.x * i)) * 4), game->subtitle_font,
			(t_vector2){(game->subtitle_size_letter.x * (text[i] - '!')), 0},
			(t_vector2){game->subtitle_size_letter.x, game->subtitle_size_letter.y});
		i++;
	}
	if (is_word_cut)
		draw_image_with_transparence(image->addr + (pos.y * image->size_line + (pos.x
				+ (int)(game->subtitle_size_letter.x * i)) * 4), game->subtitle_font,
			(t_vector2){(game->subtitle_size_letter.x * ('-' - '!')), 0},
			(t_vector2){game->subtitle_size_letter.x, game->subtitle_size_letter.y});
	return (len);
}

static int	find_next_line(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
		i++;
	return (i + 1);
}

void	print_subtitle(t_game *game, t_map *map_cell)
{
	const t_vector2 pos = {g_minimap_pos.x + g_minimap_size.x + game->subtitle_size_letter.x
		, g_minimap_pos.y + g_minimap_size.y / 2.};
	const t_vector2 pos1 = {g_minimap_pos.x + g_minimap_size.x + game->subtitle_size_letter.x
		, g_minimap_pos.y + (g_minimap_size.y / 2.) + game->subtitle_size_letter.y};
	unsigned int	i;
	t_music_name	*narrator;

	if (game->music_array[1].is_subtitle == false || map_cell == NULL)
		return ;
	narrator = map_cell->narrator;
	if (narrator == NULL)
		return ;
	if (game->time - narrator->time > SUBTITLE_TIME)
	{
		narrator->time = game->time;
		narrator->offset += find_next_line(narrator->subtitle + narrator->offset);
		if (narrator->offset >= ft_strlen(narrator->subtitle))
		{
			printf("reset\n");
			game->music_array[1].is_subtitle = false;
			narrator->offset = 0;
			return ;
		}
	}
	i = draw_subtitle_with_backgroud(game, game->image, pos, narrator->subtitle + narrator->offset);
	if (*(narrator->subtitle + narrator->offset + i) != '\n'
		&& *(narrator->subtitle + narrator->offset + i) != '\0')
		draw_subtitle_with_backgroud(game, game->image, pos1, narrator->subtitle + narrator->offset + i);
}
