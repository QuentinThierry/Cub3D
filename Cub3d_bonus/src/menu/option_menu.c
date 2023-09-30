/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option_menu.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:10:09 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/30 16:52:10 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const char	*g_key_str_ascii[]
	= {"Space", "!", "\"", "#", "$", "%", "&", "\'", "(", ")", "*", "+", ",",
	"-", ".", "/", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";",
	"<","=", ">", "?", "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
	"K","L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y",
	"Z","[", "\\", "]", "^", "_", "`", "a", "b", "c", "d", "e", "f", "g", "h",
	"i","j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w",
	"x","y", "z", "{", "|", "}", "~"};

static const t_keybind	g_special_key_int[]
	= {XK_F1, XK_F2, XK_F3, XK_F4, XK_F5, XK_F6, XK_F7, XK_F8, XK_F9, XK_F10,
	XK_F11, XK_F12, XK_dead_grave, XK_BackSpace, XK_Insert, XK_Home, XK_Prior,
	XK_Num_Lock, XK_KP_Divide, XK_KP_Multiply, XK_KP_Subtract, XK_Tab,
	XK_Delete, XK_End, XK_Next, XK_KP_Home, XK_KP_Up, XK_KP_Prior, XK_KP_Add,
	XK_Caps_Lock, XK_dead_acute, XK_Return, XK_KP_Left, XK_KP_Begin,
	XK_KP_Right, XK_Shift_L, XK_Shift_R, XK_Up, XK_KP_End, XK_KP_Down,
	XK_KP_Next, XK_KP_Enter, XK_Control_L, XK_Alt_L, XK_ISO_Level3_Shift,
	XK_Menu, XK_Control_R, XK_Left, XK_Down, XK_Right, XK_KP_Insert,
	XK_KP_Delete, XK_KP_Equal, 0, XK_Super_L, XK_Super_R};

static const char	*g_special_key_str[]
	= {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
	"F11", "F12", "Dead_Grave", "BackSpace", "Insert", "Home", "PageUp",
	"Num_Lock", "PAD_/", "PAD_*", "PAD_-", "Tabulation",
	"Delete", "End", "Next", "PAD_7", "PAD_8", "PAD_9", "PAD_+",
	"Caps_Lock", "Dead_Acute", "Return", "PAD_4", "PAD_5",
	"PAD_6", "Left_Shift", "Right_Shift", "Up_Arrow", "PAD_1", "PAD_2",
	"PAD_3", "PAD_Enter", "Left_Control", "Left_Alt", "AltGr",
	"Menu", "Right_Control", "Left_Arrow", "Down_Arrow", "Right_Arrow",
	"PAD_0", "PAD_.", "PAD_=", "(null)", "Left_Command", "Right_Command"};

const char	*get_key_str(t_keybind key)
{
	int	i;

	if (key >= 32 && key <= 126)
	{
		if (key >= 97 && key <= 122)
			key -= 32;
		return (g_key_str_ascii[key - 32]);
	}
	else
	{
		i = 0;
		while (i < 56)
		{
			if (key == g_special_key_int[i])
				return (g_special_key_str[i]);
			i++;
		}
		return ("/?\\");
	}
}

__attribute__((always_inline))
static inline t_pixel32	get_alpha_pixel(t_pixel32 pixel, t_pixel32 color)
{
	const float	dark_quantity = 1 - DARK_PERCENT_OPTION;

	return (((unsigned char)(((pixel >> 16) & 0xFF) * dark_quantity + ((color >> 16) & 0xff) * DARK_PERCENT_OPTION) << 16)
	| ((unsigned char)(((pixel >> 8) & 0xFF) * dark_quantity + ((color >> 8) & 0xff) * DARK_PERCENT_OPTION) << 8)
	| (unsigned char)((pixel & 0xFF) * dark_quantity + (color & 0xff) * DARK_PERCENT_OPTION));
}

void	apply_menu_dark_filter(t_image *menu_image)
{
	int			x;
	int			y;
	const int	img_size_x = menu_image->size.x;
	const int	img_size_y = menu_image->size.y;
	t_pixel32	*pix_addr;

	x = 0;
	y = 0;
	pix_addr = (t_pixel32 *)(menu_image->addr + y * menu_image->size_line + x * 4);
	while (y < img_size_y)
	{
		x = 0;
		while (x < img_size_x)
		{
			*pix_addr = get_alpha_pixel(*pix_addr, DARK_COLOR_OPTION);
			pix_addr++;
			x++;
		}
		y++;
	}
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

void	draw_alpha_rectangle(t_image *dest, t_vector2 pos, t_vector2 size)
{
	int			x;
	int			y;
	t_pixel32	*addr_pixel;

	addr_pixel = (t_pixel32 *)dest->addr + dest->size.x * pos.y + pos.x;
	y = 0;
	while (y < size.y)
	{
		x = 0;
		while (x < size.x)
		{
			addr_pixel[x] = get_alpha_pixel(addr_pixel[x], 0xeeeeee);
			x++;
		}
		addr_pixel += dest->size.x;
		y++;
	}
}

static void	draw_slider_linked_text(t_game *game, t_slider *slider, t_image *image)
{
	draw_text_at_with_backgroud(game, image,
	(t_vector2){
		slider->pos.x
		- ft_strlen(slider->linked_text) * (game->size_letter.x + 1),
		slider->pos.y - slider->hor_image->size.y / 2
	},
	slider->linked_text);
}

static void	draw_slider_value(t_game *game, t_slider *slider, t_image *image)
{
	long		value_to_draw;
	int			nb_digits;
	int			i;
	char		digit;
	t_vector2	pos;

	value_to_draw = slider->min_max_value.x +
		(slider->min_max_value.y - slider->min_max_value.x) * slider->percent;
	nb_digits = count_digits(value_to_draw);
	if (value_to_draw < 0)
		value_to_draw = -value_to_draw;
	pos.x = slider->pos.x + slider->hor_image->size.x + slider->hor_image->size.x * 0.1;
	pos.y = slider->pos.y - slider->hor_image->size.y / 2.;
	draw_alpha_rectangle(image,
	(t_vector2){
		pos.x + (game->size_letter.x / 2),
		pos.y
	},
	(t_vector2)
	{
		(nb_digits) * (game->size_letter.x)
		+ game->size_letter.x,
		game->size_letter.y
	});
	i = 1;
	while (i <= nb_digits)
	{
		digit = get_digit_at_itoa(value_to_draw, nb_digits - i);
		draw_image_with_transparence(image->addr + (pos.y * image->size_line + (pos.x
			+ (int)(game->size_letter.x * i)) * 4), game->font,
			(t_vector2){(game->size_letter.x * (digit - '!')), 0},
			(t_vector2){game->size_letter.x, game->size_letter.y});
		i++;
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
		ft_memcpy(image->addr + (slider->pos.x + x_pos) * 4 +
			(y + slider->pos.y + (slider->size.y - slider->hor_image->size.y) / 2) * image->size_line,
			slider->hor_image->addr + y * slider->hor_image->size_line,
			slider->hor_image->size_line);
		y++;
	}
	x_pos = slider->pos.x + (slider->percent * slider->hor_image->size.x);
	y = 0;
	while (y < slider->vert_image->size.y)
	{
		ft_memcpy(image->addr + x_pos * 4 +
			(y + slider->pos.y)
			* image->size_line,
			slider->vert_image->addr + y * slider->vert_image->size_line,
			slider->vert_image->size_line);
		y++;
	}
	draw_slider_value(game, slider, image);
	draw_slider_linked_text(game, slider, image);
}

void	draw_hor_bar(t_option_menu *opt, t_image *dest)
{
	draw_rectangle(
		dest, opt->hor_bar_pos, opt->hor_bar_size, COLOR_BAR_OPTION);
}

void	draw_vert_bar(t_option_menu *opt, t_image *dest)
{
	draw_rectangle(
		dest, opt->vert_bar_pos, opt->vert_bar_size, COLOR_BAR_OPTION);
}

void	draw_option_menu(t_game *game, t_option_menu *opt_menu)
{
	int	i;
	int	x_mouse;
	int	y_mouse;

	mlx_mouse_get_pos(game->mlx_ptr, game->win, &x_mouse, &y_mouse);
	if (opt_menu->pressed_slider_ref)
	{
		opt_menu->pressed_slider_ref->percent =
			(float)(x_mouse - opt_menu->pressed_slider_ref->pos.x
			- opt_menu->pressed_slider_ref->vert_image->size.x / 2.)
			/ opt_menu->pressed_slider_ref->size.x;
		if (opt_menu->pressed_slider_ref->percent < 0)
			opt_menu->pressed_slider_ref->percent = 0;
		else if (opt_menu->pressed_slider_ref->percent > 1)
			opt_menu->pressed_slider_ref->percent = 1;
	}
	i = 0;
	while (i < NB_OPTIONS_BUTTONS)
	{
		check_mouse_is_in_button(
			&opt_menu->buttons[i], x_mouse, y_mouse);
		draw_button(&opt_menu->buttons[i], game->menu->image);
		draw_text_in_button(game, game->menu->image, &opt_menu->buttons[i]);
		draw_text_at_with_backgroud(game, game->menu->image,
			(t_vector2){opt_menu->buttons[i].pos.x
				- opt_menu->buttons[i].size.x * 1.5,
				opt_menu->buttons[i].pos.y + opt_menu->buttons[i].size.y / 2.
				- game->size_letter.y / 2.}, opt_menu->buttons[i].linked_text);
		i++;
	}
	draw_button(&opt_menu->exit_opt_button, game->menu->image);
	draw_slider(game, &opt_menu->slider_fov, game->menu->image);
	draw_slider(game, &opt_menu->slider_sound, game->menu->image);
	draw_hor_bar(opt_menu, game->menu->image);
	draw_vert_bar(opt_menu, game->menu->image);
}

