/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_slider.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 15:10:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/13 15:11:44 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"


static void	draw_slider_linked_text(t_game *game,
	t_slider *slider, t_image *image)
{
	draw_text_at_with_backgroud(game, image,
		(t_vector2){
		slider->pos.x
		- ft_strlen(slider->linked_text) * (game->size_letter.x + 1),
		slider->pos.y - slider->hor_image->size.y / 2},
		slider->linked_text);
}

static int	count_digits(int value)
{
	int	res;

	res = 0;
	if (value == 0)
		return (1);
	if (value == -2147483648)
		return (10);
	if (value < 0)
		value = -value;
	while (value > 0)
	{
		value /= 10;
		res++;
	}
	return (res);
}

static char	get_digit_at_itoa(int value, int pos)
{
	if (pos == 0)
		return ("0123456789"[value % 10]);
	value = value / powf(10, pos);
	return ("0123456789"[value % 10]);
}

static void	draw_slider_value(t_game *game, t_slider *slider, t_image *image)
{
	long		value_to_draw;
	int			nb_digits;
	int			i;
	char		digit;
	t_vector2	pos;

	value_to_draw = slider->min_max_value.x
		+ (slider->min_max_value.y - slider->min_max_value.x) * slider->percent;
	nb_digits = count_digits(value_to_draw);
	value_to_draw *= 1 - (value_to_draw < 0) * 2;
	pos.x = slider->pos.x + slider->hor_image->size.x * 1.1;
	pos.y = slider->pos.y - slider->hor_image->size.y / 2.;
	draw_alpha_rectangle(image,
		(t_vector2){pos.x + (game->size_letter.x / 2), pos.y},
		(t_vector2){(nb_digits) * (game->size_letter.x) + game->size_letter.x,
		game->size_letter.y});
	i = 0;
	while (++i <= nb_digits)
	{
		digit = get_digit_at_itoa(value_to_draw, nb_digits - i);
		draw_image_with_transparence(image->addr + (pos.y * image->size_line
				+ (pos.x + (int)(game->size_letter.x * i)) * 4), game->font,
			(t_vector2){(game->size_letter.x * (digit - '!')), 0},
			(t_vector2){game->size_letter.x, game->size_letter.y});
	}
}

void	draw_slider(t_game *game, t_slider *slider, t_image *image)
{
	register int	y;
	int				x_pos;

	x_pos = slider->vert_image->size.x / 2;
	y = 0;
	while (y < slider->hor_image->size.y)
	{
		ft_memcpy(image->addr + (slider->pos.x + x_pos) * 4
			+ (y + slider->pos.y + (slider->size.y
					- slider->hor_image->size.y) / 2) * image->size_line,
			slider->hor_image->addr + y * slider->hor_image->size_line,
			slider->hor_image->size_line);
		y++;
	}
	x_pos = slider->pos.x + (slider->percent * slider->hor_image->size.x);
	y = 0;
	while (y < slider->vert_image->size.y)
	{
		ft_memcpy(image->addr + x_pos * 4 + (y + slider->pos.y)
			* image->size_line, slider->vert_image->addr + y
			* slider->vert_image->size_line, slider->vert_image->size_line);
		y++;
	}
	draw_slider_value(game, slider, image);
	draw_slider_linked_text(game, slider, image);
}
