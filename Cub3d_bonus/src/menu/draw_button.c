/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_button.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 14:48:10 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/13 14:48:42 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

// with background
void	draw_centered_text_at_y(t_game *game,
	t_image *image, int y, const char *text)
{
	int			i;
	t_vector2	pos;

	if (game->size_letter.y + y > image->size.y)
		return ;
	pos.x = WIN_X / 2. - (ft_strlen(text) * game->size_letter.x) / 2;
	draw_alpha_rectangle(image,
		(t_vector2){pos.x - game->size_letter.x / 2, y},
		(t_vector2){(ft_strlen(text) + 1) * game->size_letter.x,
		game->size_letter.y});
	if (ft_strlen(text) * game->size_letter.x > image->size.x)
		pos.x = 0;
	pos.y = y;
	i = -1;
	while (text[++i])
	{
		if ((i + 1) * game->size_letter.x + pos.x > image->size.x)
			return ;
		if (text[i] == ' ')
			continue ;
		draw_image_with_transparence(image->addr + (pos.y * image->size_line
				+ (pos.x + (int)(game->size_letter.x * i)) * 4), game->font,
			(t_vector2){(game->size_letter.x * (text[i] - '!')), 0},
			(t_vector2){game->size_letter.x, game->size_letter.y});
	}
}

void	draw_text_at_with_backgroud(t_game *game,
	t_image *image, t_vector2 pos, const char *text)
{
	int	i;

	if (game->size_letter.y + pos.y > image->size.y)
		return ;
	draw_alpha_rectangle(image,
		(t_vector2){pos.x - game->size_letter.x / 2, pos.y}, (t_vector2){
		(ft_strlen(text) + 1) * game->size_letter.x, game->size_letter.y});
	i = -1;
	while (text[++i])
	{
		if ((i + 1) * game->size_letter.x + pos.x > image->size.x)
			return ;
		if (text[i] == ' ')
			continue ;
		else if (text[i] < 32 || text[i] > 126)
			draw_image_with_transparence(image->addr + (pos.y * image->size_line
					+ (pos.x + (int)(game->size_letter.x * i)) * 4), game->font,
				(t_vector2){(game->size_letter.x * ('?' - '!')), 0},
				(t_vector2){game->size_letter.x, game->size_letter.y});
		else
			draw_image_with_transparence(image->addr + (pos.y * image->size_line
					+ (pos.x + (int)(game->size_letter.x * i)) * 4), game->font,
				(t_vector2){(game->size_letter.x * (text[i] - '!')), 0},
				(t_vector2){game->size_letter.x, game->size_letter.y});
	}
}

void	draw_text_in_button(t_game *game, t_image *image, t_button *button)
{
	int			i;
	t_vector2	start_pos;

	start_pos.y = button->pos.y + button->size.y / 2. - game->size_letter.y / 2;
	if (start_pos.y + game->size_letter.y > button->pos.y + button->size.y)
		return ;
	start_pos.x = button->pos.x + button->size.x / 2.
		- (ft_strlen(button->text) * game->size_letter.x) / 2;
	if (start_pos.x < button->pos.x)
		start_pos.x = button->pos.x;
	i = -1;
	while (button->text[++i])
	{
		if (button->text[i] == ' ')
			continue ;
		if ((i + 1) * game->size_letter.x
			+ start_pos.x > button->pos.x + button->size.x)
			return ;
		draw_image_with_transparence(image->addr
			+ (start_pos.y * image->size_line + (start_pos.x
					+ (int)(game->size_letter.x * i)) * 4), game->font,
			(t_vector2){(game->size_letter.x * (button->text[i] - '!')), 0},
			(t_vector2){game->size_letter.x, game->size_letter.y});
	}
}

void	draw_button(t_button *button, t_image *image)
{
	t_image			*button_image;
	register int	y;

	if (button->is_hovered)
		button_image = button->hovered_image;
	else
		button_image = button->base_image;
	y = 0;
	while (y < button->size.y)
	{
		ft_memcpy(image->addr
			+ (button->pos.y + y) * image->size_line + button->pos.x * 4,
			button_image->addr
			+ y * button_image->size_line, button_image->size_line);
		y++;
	}
}

void	check_mouse_is_in_button(t_button *button, int x, int y)
{
	button->is_hovered = (x > button->pos.x
			&& x < button->pos.x + button->size.x
			&& y > button->pos.y
			&& y < button->pos.y + button->size.y);
}
