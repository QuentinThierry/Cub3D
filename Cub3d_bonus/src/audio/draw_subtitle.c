/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_subtitle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 17:19:58 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/11 17:20:18 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static int	_find_max_caracter(const char *text, bool *is_word_cut)
{
	int	i;

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

static void	_draw_letter(t_game *game, t_image *image,
		t_vector2 pos, const char text)
{
	if (text == ' ')
		return ;
	else if (text < 32 || text > 126)
		draw_image_with_transparence(image->addr + pos.y * image->size_line
			+ pos.x * 4, game->subtitle_font,
			(t_vector2){(game->subtitle_size.x * ('?' - '!')), 0},
			(t_vector2){game->subtitle_size.x, game->subtitle_size.y});
	else
		draw_image_with_transparence(image->addr + pos.y * image->size_line
			+ pos.x * 4, game->subtitle_font,
			(t_vector2){(game->subtitle_size.x * (text - '!')), 0},
			(t_vector2){game->subtitle_size.x, game->subtitle_size.y});
}

unsigned int	draw_subtitle_with_backgroud(t_game *game, t_image *image,
		t_vector2 pos, const char *text)
{
	int		i;
	int		len;
	bool	is_word_cut;

	if (game->subtitle_size.y + pos.y > image->size.y)
		return (0);
	len = _find_max_caracter(text, &is_word_cut);
	if (pos.x + game->subtitle_size.x * (len + is_word_cut + 1) > image->size.x)
	{
		len = (image->size.x - pos.x) / game->subtitle_size.x - is_word_cut - 2;
		is_word_cut = true;
	}
	draw_alpha_rectangle(image, (t_vector2){pos.x - game->subtitle_size.x / 2,
		pos.y}, (t_vector2){(len + 1 + is_word_cut) * game->subtitle_size.x,
		game->subtitle_size.y});
	i = -1;
	while (++i < len)
		_draw_letter(game, image, (t_vector2){pos.x
			+ (int)(game->subtitle_size.x * i), pos.y}, text[i]);
	if (is_word_cut)
		_draw_letter(game, image, (t_vector2){pos.x
			+ (int)(game->subtitle_size.x * i), pos.y}, '-');
	return (len);
}