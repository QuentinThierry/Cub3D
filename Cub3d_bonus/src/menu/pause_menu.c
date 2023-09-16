/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pause_menu.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:57:18 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/16 20:04:33 by qthierry         ###   ########.fr       */
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

void draw_text_in_button(t_game *game, t_image *image, t_button *button)
{
	int			i;
	t_vector2	start_pos;

	i = 0;
	start_pos = button->pos;
	while (button->text[i])
	{
		draw_image_with_transparence(image->addr + (start_pos.y * image->size_line + (start_pos.x
			+ (int)(game->size_letter.x * i)) * 4), game->font,
			(t_vector2){(game->size_letter.x * (button->text[i] - '!')), 0},
			(t_vector2){game->size_letter.x, game->size_letter.y});
		i++;
	}
}

static void	draw_button(t_button *button, t_image *image)
{
	t_image	*button_image;
	int		y;

	if (button->is_hovered)
		button_image = button->hovered_image;
	else
		button_image = button->base_image;
	y = 0;
	while (y < button->size.y)
	{
		ft_memcpy(image->addr + (button->pos.y + y) * image->size_line + button->pos.x * 4,
			button_image->addr + y * button_image->size_line, button_image->size_line);
		y++;
	}
}

static void draw_option_menu(t_game *game)
{
	int	i;

	i = 0;
	while (i < NB_OPTIONS_BUTTONS)
	{
		draw_button(&game->menu->buttons[i], game->menu->image);
		draw_text_in_button(game, game->menu->image, &game->menu->buttons[i]);
		i++;
	}
}

void	draw_choosing_key_menu(t_game *game)
{
	
}

__attribute__((always_inline))
static inline t_pixel32	darken_pixel(t_pixel32 pixel)
{
	const float	dark_quantity = 1 - DARK_PERCENT_OPTION;

	return (((unsigned char)(((pixel >> 16) & 0xFF) * dark_quantity + ((DARK_COLOR >> 16) & 0xff) * DARK_PERCENT_OPTION) << 16)
	| ((unsigned char)(((pixel >> 8) & 0xFF) * dark_quantity + ((DARK_COLOR >> 8) & 0xff) * DARK_PERCENT_OPTION) << 8)
	| (unsigned char)((pixel & 0xFF) * dark_quantity + (DARK_COLOR & 0xff) * DARK_PERCENT_OPTION));
}

#define SIZE_BOX_BLUR 21.

static void	set_to_average_pix(t_image *src, t_image *dest, int ori_pos_x, int ori_pos_y)
{
	t_pixel32	*pix;
	float		red;
	float		green;
	float		blue;
	int			y_start;
	int			x_start;
	int			x_end;
	int			y_end;
	int			x;
	int			nb_pixels;

	red = 0;
	green = 0;
	blue = 0;
	x = 0;
	x_start = -SIZE_BOX_BLUR / 2;
	y_start = -SIZE_BOX_BLUR / 2;
	x_end = SIZE_BOX_BLUR / 2;
	y_end = SIZE_BOX_BLUR / 2;
	if (ori_pos_y + y_start < 0)
		y_start = -ori_pos_y;
	if (ori_pos_y + SIZE_BOX_BLUR > dest->size.y)
		y_end -= ori_pos_y + SIZE_BOX_BLUR / 2 - dest->size.y;
	if (ori_pos_x + x_start < 0)
		x_start = -ori_pos_x;
	if (ori_pos_x + SIZE_BOX_BLUR > dest->size.x)
		x_end -= ori_pos_x + SIZE_BOX_BLUR / 2 - dest->size.x;
	nb_pixels = (y_end - y_start) * (x_end - x_start);
	
	while (y_start < y_end)
	{
		x = 0;
		pix = (t_pixel32 *)(src->addr + ((ori_pos_y + y_start)
			* src->size_line + (ori_pos_x + x_start) * 4));
		while (x + x_start < x_end)
		{
			red += ((*(pix + x) >> 16) & 0xff);
			green += ((*(pix + x) >> 8) & 0xff);
			blue += (*(pix + x) & 0xff);
			x++;
		}
		y_start++;
	}
	red /= nb_pixels;
	green /= nb_pixels;
	blue /= nb_pixels;
	*(t_pixel32*)(dest->addr + (ori_pos_y * dest->size_line + ori_pos_x * 4))
		= ((t_byte)red) << 16 | ((t_byte)green << 8) | (t_byte)blue;
	*(t_pixel32*)(dest->addr + (ori_pos_y * dest->size_line + ori_pos_x * 4) + 4)
		= ((t_byte)red) << 16 | ((t_byte)green << 8) | (t_byte)blue;
	*(t_pixel32*)(dest->addr + (ori_pos_y * dest->size_line + ori_pos_x * 4) + dest->size_line)
		= ((t_byte)red) << 16 | ((t_byte)green << 8) | (t_byte)blue;
	*(t_pixel32*)(dest->addr + (ori_pos_y * dest->size_line + ori_pos_x * 4) + 4 + dest->size_line)
		= ((t_byte)red) << 16 | ((t_byte)green << 8) | (t_byte)blue;
}

void	blur_image(t_image *dest, t_image *src)
{
	int			y;
	int			x;
	t_pixel32	pix;

	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	struct timespec			time;
	double					fps;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	clock_gettime(CLOCK_REALTIME, &time);

	y = 0;
	
	while (y < dest->size.y)
	{
		x = 0;
		while (x < dest->size.x)
		{
			set_to_average_pix(src, dest, x, y);
			x++;
			x++;
		}
		y++;
		y++;
	}


	clock_gettime(CLOCK_REALTIME, &cur_time);
	fps = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	printf("time : %lf\n", fps);
	last_time = cur_time;
	
}



static void	apply_menu_dark_filter(t_image *menu_image)
{
	// int			x;
	// int			y;
	// const int	img_size_x = menu_image->size.x;
	// const int	img_size_y = menu_image->size.y;
	// t_pixel32	*pix_addr;

	// x = 0;
	// y = 0;
	// pix_addr = (t_pixel32 *)(menu_image->addr + y * menu_image->size_line + x * 4);
	// while (y < img_size_y)
	// {
	// 	x = 0;
	// 	while (x < img_size_x)
	// 	{
	// 		*pix_addr = darken_pixel(*pix_addr);
	// 		pix_addr++;
	// 		x++;
	// 	}
	// 	y++;
	// }
}

void	menu_key_hook(t_keybind key, t_game *game)
{
	if (key == game->keybinds[e_key_pause])
		printf("return at last menu\n");
		// quit pause or return to previous menu
	if (key == XK_Escape)
		ft_close(game);
}

void	choose_key_hook(t_keybind key, t_game *game)
{
	if (key == XK_Escape)
		ft_close(game);
	game->menu->buttons[game->menu->pressed_button].text
		= get_key_str(key);
	game->menu->state = OPTION_MENU;
	game->keybinds[game->menu->pressed_button] = key;
	mlx_hook(game->win, 2, (1L << 0), (void *)menu_key_hook, game);
}

void	menu_mouse_click(int mouse_button, int x, int y, t_game *game)
{
	t_menu		*menu;
	t_button	*button;
	t_byte		i;

	menu = game->menu;
	if (menu->state == OPTION_MENU)
	{
		i = 0;
		while (i < NB_OPTIONS_BUTTONS)
		{
			button = &menu->buttons[i];
			if (x > button->pos.x && x < button->pos.x + button->size.x
				&& y > button->pos.y && y < button->pos.y + button->size.y)
			{
				menu->state = CHOOSING_KEY_MENU;
				apply_menu_dark_filter(menu->image);
				blur_image(menu->image, game->image);
				menu->pressed_button = i;
				mlx_hook(game->win, 2, (1L << 0), (void *)choose_key_hook, game);
				break ;
			}
			i++;
		}
		
	}
}

void	draw_menu(t_game *game)
{
	t_menu	*menu;
	
	menu = game->menu;
	// if (menu->state == PAUSE_MENU)
	// 	draw_base_menu(game);
	if (menu->state != CHOOSING_KEY_MENU)
		ft_memcpy(game->menu->image->addr,
				game->menu->background_image->addr, WIN_X * WIN_Y * 4);
	if (menu->state == OPTION_MENU)
		draw_option_menu(game);
	if (menu->state == CHOOSING_KEY_MENU)
		draw_choosing_key_menu(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win, menu->image->img, 0, 0);
	// exit(0);
}

int	menu_loop_hook(t_game *game)
{
	static struct timespec	last_time = {0};
	struct timespec			cur_time;
	struct timespec			time;
	long					fps;

	if (last_time.tv_sec == 0)
		clock_gettime(CLOCK_REALTIME, &last_time);
	clock_gettime(CLOCK_REALTIME, &time);
	

	
	draw_menu(game);


	clock_gettime(CLOCK_REALTIME, &cur_time);
	game->delta_time = (cur_time.tv_sec - last_time.tv_sec
			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
	fps = (long)(1.0 / game->delta_time);
	tot_fps += fps;
	nb_fps++;
	if ((nb_fps % 500) == 0)
		printf("fps : %ld\n", fps);
	last_time = cur_time;
	
	return (0);
}
