/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pause_menu.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:57:18 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/21 19:40:00 by qthierry         ###   ########.fr       */
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

void	draw_text_in_button(t_game *game, t_image *image, t_button *button)
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

void	draw_text_at_pos(t_game *game, t_image *image, t_vector2 pos, const char *text)
{
	int	i;

	i = 0;
	if (game->size_letter.y + pos.y > image->size.y)
		return ;
	while (text[i])
	{
		if (i * game->size_letter.x + pos.x > image->size.x)
			return ;
		if (text[i] == ' ')
		{
			i++;
			continue ;
		}
		draw_image_with_transparence(image->addr + (pos.y * image->size_line + (pos.x
			+ (int)(game->size_letter.x * i)) * 4), game->font,
		(t_vector2){(game->size_letter.x * (text[i] - '!')), 0},
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

static void	horizontal_blur(t_image *src, int *h_rgb_blur_buffer)
{
	int			x;
	int			y;
	int			size_x_3;
	int			size_x;
	int			size_y;
	t_pixel32	*pix_src;

	size_x = src->size.x;
	size_y = src->size.y;
	size_x_3 = src->size.x * 3;
	pix_src = (t_pixel32 *)src->addr;

	y = 0;
	while (y < size_y)
	{
		h_rgb_blur_buffer[y * size_x_3] = 0;
		h_rgb_blur_buffer[y * size_x_3 + 1] = 0;
		h_rgb_blur_buffer[y * size_x_3 + 2] = 0;
		x = 0;
		while (x <= SIZE_BOX_BLUR / 2)
		{
			h_rgb_blur_buffer[y * size_x_3] +=
				((pix_src[y * size_x + x] >> 16) & 0xff);
			h_rgb_blur_buffer[y * size_x_3 + 1] +=
				((pix_src[y * size_x + x] >> 8) & 0xff);
			h_rgb_blur_buffer[y * size_x_3 + 2] +=
				((pix_src[y * size_x + x]) & 0xff);
			x++;
		}
		y++;
	}
	y = 0;
	while (y < size_y)
	{
		x = 1;
		while (x < size_x)
		{
			h_rgb_blur_buffer[x * 3] =
				h_rgb_blur_buffer[x * 3 - 3];
			if (x + SIZE_BOX_BLUR / 2 < size_x)
				h_rgb_blur_buffer[x * 3] +=
					((pix_src[x + SIZE_BOX_BLUR / 2] >> 16) & 0xff);
			if (x - SIZE_BOX_BLUR / 2 - 1 >= 0)
				h_rgb_blur_buffer[x * 3] -=
					((pix_src[x - SIZE_BOX_BLUR / 2 - 1] >> 16) & 0xff);
			h_rgb_blur_buffer[x * 3 + 1] = 
				h_rgb_blur_buffer[x * 3 - 2];
			if (x + SIZE_BOX_BLUR / 2 < size_x)
				h_rgb_blur_buffer[x * 3 + 1] +=
					((pix_src[x + SIZE_BOX_BLUR / 2] >> 8) & 0xff);
			if (x - SIZE_BOX_BLUR / 2 - 1 >= 0)
				h_rgb_blur_buffer[x * 3 + 1] -=
					((pix_src[x - SIZE_BOX_BLUR / 2 - 1] >> 8) & 0xff);
			h_rgb_blur_buffer[x * 3 + 2] =
				h_rgb_blur_buffer[x * 3 - 1];
			if (x + SIZE_BOX_BLUR / 2 < size_x)
				h_rgb_blur_buffer[x * 3 + 2] +=
					((pix_src[x + SIZE_BOX_BLUR / 2]) & 0xff);
			if (x - SIZE_BOX_BLUR / 2 - 1 >= 0)
				h_rgb_blur_buffer[x * 3 + 2] -=
					((pix_src[x - SIZE_BOX_BLUR / 2 - 1]) & 0xff);
			x++;
		}
		h_rgb_blur_buffer += size_x_3;
		pix_src += size_x;
		y++;
	}
}

static void	vertical_blur(t_image *dest, int *h_rgb_blur_buffer, int *v_rgb_blur_buffer)
{
	int			x;
	int			y;
	const int	size_x_3 = dest->size.x * 3;
	const int	size_y = dest->size.y;
	const int	size_x = dest->size.x;
	t_pixel32	*pix_dest;
	int			nb_pixel_div;

	pix_dest = (t_pixel32 *)dest->addr;

	ft_bzero(v_rgb_blur_buffer, WIN_X * 3 * sizeof(int));
	x = 0;
	while (x < size_x)
	{
		y = 0;
		while (y <= SIZE_BOX_BLUR / 2)
		{
			v_rgb_blur_buffer[x * 3] += 
				h_rgb_blur_buffer[x * 3 + y * size_x_3];
			v_rgb_blur_buffer[x * 3 + 1] +=
				h_rgb_blur_buffer[x * 3 + y * size_x_3 + 1];
			v_rgb_blur_buffer[x * 3 + 2] +=
				h_rgb_blur_buffer[x * 3 + y * size_x_3 + 2];
			y++;
		}
		nb_pixel_div = SIZE_BOX_BLUR - (x - SIZE_BOX_BLUR / 2 - 1 < 0) * (-1 * (x - SIZE_BOX_BLUR / 2 - 1) - 1)
				- (x + SIZE_BOX_BLUR / 2 >= size_x) * (x + SIZE_BOX_BLUR / 2 - size_x + 1);
		pix_dest[x] =
			((v_rgb_blur_buffer[x * 3] / (nb_pixel_div * (SIZE_BOX_BLUR / 2 + 1))) << 16) |
			((v_rgb_blur_buffer[x * 3 + 1] / (nb_pixel_div * (SIZE_BOX_BLUR / 2 + 1))) << 8) |
			(v_rgb_blur_buffer[x * 3 + 2] / (nb_pixel_div * (SIZE_BOX_BLUR / 2 + 1)));
		x++;
	}
	x = 0;
	while (x < size_x)
	{
		y = 1;
		while (y < size_y)
		{
			v_rgb_blur_buffer[y * size_x_3] = 
				v_rgb_blur_buffer[(y - 1) * size_x_3];
			if (y + SIZE_BOX_BLUR / 2 < size_y)
				v_rgb_blur_buffer[y * size_x_3] +=
					h_rgb_blur_buffer[(y + SIZE_BOX_BLUR / 2) * size_x_3];
			if (y - SIZE_BOX_BLUR / 2 - 1 >= 0)
				v_rgb_blur_buffer[y * size_x_3] -=
					h_rgb_blur_buffer[(y - SIZE_BOX_BLUR / 2 - 1) * size_x_3];
			v_rgb_blur_buffer[y * size_x_3 + 1] = 
				v_rgb_blur_buffer[(y - 1) * size_x_3 + 1];
			if (y + SIZE_BOX_BLUR / 2 < size_y)
				v_rgb_blur_buffer[y * size_x_3 + 1] +=
					h_rgb_blur_buffer[(y + SIZE_BOX_BLUR / 2) * size_x_3 + 1];
			if (y - SIZE_BOX_BLUR / 2 - 1 >= 0)
				v_rgb_blur_buffer[y * size_x_3 + 1] -=
					h_rgb_blur_buffer[(y - SIZE_BOX_BLUR / 2 - 1) * size_x_3 + 1];
			v_rgb_blur_buffer[y * size_x_3 + 2] = 
				v_rgb_blur_buffer[(y - 1) * size_x_3 + 2];
			if (y + SIZE_BOX_BLUR / 2 < size_y)
				v_rgb_blur_buffer[y * size_x_3 + 2] +=
					h_rgb_blur_buffer[(y + SIZE_BOX_BLUR / 2) * size_x_3 + 2];
			if (y - SIZE_BOX_BLUR / 2 - 1 >= 0)
				v_rgb_blur_buffer[y * size_x_3 + 2] -=
					h_rgb_blur_buffer[(y - SIZE_BOX_BLUR / 2 - 1) * size_x_3 + 2];
			nb_pixel_div = (SIZE_BOX_BLUR - (y + SIZE_BOX_BLUR / 2 >= size_y) * (y + SIZE_BOX_BLUR / 2 - size_y + 1)
				- (y - SIZE_BOX_BLUR / 2 - 1 < 0) * (-1 * (y - SIZE_BOX_BLUR / 2 - 1) - 1))
				* (SIZE_BOX_BLUR - (x - SIZE_BOX_BLUR / 2 - 1 < 0) * (-1 * (x - SIZE_BOX_BLUR / 2 - 1) - 1)
				- (x + SIZE_BOX_BLUR / 2 >= size_x) * (x + SIZE_BOX_BLUR / 2 - size_x + 1)
			);
			pix_dest[y * size_x + x] =
				((v_rgb_blur_buffer[y * size_x_3] / nb_pixel_div) << 16) |
				((v_rgb_blur_buffer[y * size_x_3 + 1] / nb_pixel_div) << 8) |
				(v_rgb_blur_buffer[y * size_x_3 + 2] / nb_pixel_div);
			y++;
		}
		v_rgb_blur_buffer += 3;
		h_rgb_blur_buffer += 3;
		x++;
	}
}

// apply 3 times a box_blur to approximate a gaussian blur
void	blur_image(t_image *dest, t_image *src,
			int *h_rgb_blur_buffer, int *v_rgb_blur_buffer)
{
	horizontal_blur(src, h_rgb_blur_buffer);
	vertical_blur(dest, h_rgb_blur_buffer, v_rgb_blur_buffer);
	horizontal_blur(dest, h_rgb_blur_buffer);
	vertical_blur(src, h_rgb_blur_buffer, v_rgb_blur_buffer);
	ft_memcpy(dest->addr, src->addr, WIN_X * WIN_Y * 4);
}



static void	apply_menu_dark_filter(t_image *menu_image)
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
			*pix_addr = darken_pixel(*pix_addr);
			pix_addr++;
			x++;
		}
		y++;
	}
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
				menu->pressed_button = i;
				apply_menu_dark_filter(menu->image);
				draw_text_at_pos(game, menu->image,
					(t_vector2){
						WIN_X / 2. - (ft_strlen(KEY_TEXT_CHANGE) * game->size_letter.x) / 2,
						WIN_Y / 2. - game->size_letter.y / 2},
					KEY_TEXT_CHANGE);
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
	// if ((nb_fps % 500) == 0)
	// 	printf("fps : %ld\n", fps);
	last_time = cur_time;
	
	return (0);
}

void	set_pause_menu_mode(t_game *game)
{
	mlx_hook(game->win, 2, (1L << 0), (void *)menu_key_hook, game);
	mlx_hook(game->win, 3, (1L << 1), NULL, game);
	mlx_mouse_hook(game->win, (void *)menu_mouse_click, game);
	mlx_mouse_show(game->mlx_ptr, game->win);
	mlx_hook(game->win, 6, (1L << 6) | (1L << 2) , NULL, game);
	mlx_loop_hook(game->mlx_ptr, menu_loop_hook, game);
	mlx_hook(game->win, 8, (1L << 5), NULL, game);
	blur_image(game->menu->background_image,
		game->image, game->menu->h_rgb_blur_buffer,
		game->menu->v_rgb_blur_buffer);
	game->menu->state = OPTION_MENU;
}

void	resume_menu(t_game *game, t_menu *menu)
{
	if (menu->state == PAUSE_MENU || menu->state == OPTION_MENU) // change, debug
	{
		init_mouse(game);
		mlx_hook(game->win, 2, (1L << 0), (void *)key_press_hook, game);
		mlx_hook(game->win, 3, (1L << 1), (void *)key_release_hook, game);
		mlx_hook(game->win, 17, (1L << 8), ft_close, game);
		mlx_hook(game->win, 6, (1L << 6) | (1L << 2) , mouse_hook, game);
		mlx_hook(game->win, 8, (1L << 5), mouse_leave, game);
		mlx_mouse_hook(game->win, mouse_click, game);
		mlx_loop_hook(game->mlx_ptr, on_update, game);
	}
	else if (menu->state == OPTION_MENU)
	{
		
	}
	else if (menu->state == CHOOSING_KEY_MENU)
	{
		
	}
}

void	menu_key_hook(t_keybind key, t_game *game)
{
	if (key == game->keybinds[e_key_pause])
		resume_menu(game, game->menu);
		// quit pause or return to previous menu
	if (key == XK_Escape)
		ft_close(game);
}
