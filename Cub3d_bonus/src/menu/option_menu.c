/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option_menu.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:10:09 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/13 16:49:09 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline t_pixel32	get_alpha_pixel(t_pixel32 pixel,
	t_pixel32 color, const float intensity)
{
	const float	dark_quantity = 1 - intensity;

	return (((unsigned char)(((pixel >> 16) & 0xFF) * dark_quantity
			+ ((color >> 16) & 0xff) * intensity) << 16)
	| ((unsigned char)(((pixel >> 8) & 0xFF) * dark_quantity
			+ ((color >> 8) & 0xff) * intensity) << 8)
	| (unsigned char)((pixel & 0xFF) * dark_quantity
			+ (color & 0xff) * intensity));
}

void	apply_menu_dark_filter(t_image *menu_image, const float intensity)
{
	int			x;
	int			y;
	const int	img_size_x = menu_image->size.x;
	const int	img_size_y = menu_image->size.y;
	t_pixel32	*pix_addr;

	x = 0;
	y = 0;
	pix_addr
		= (t_pixel32 *)(menu_image->addr + y * menu_image->size_line + x * 4);
	while (y < img_size_y)
	{
		x = 0;
		while (x < img_size_x)
		{
			*pix_addr
				= get_alpha_pixel(*pix_addr, DARK_COLOR_OPTION, intensity);
			pix_addr++;
			x++;
		}
		y++;
	}
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
			addr_pixel[x] = get_alpha_pixel(addr_pixel[x],
				0xeeeeee, DARK_PERCENT_OPTION);
			x++;
		}
		addr_pixel += dest->size.x;
		y++;
	}
}

static void	draw_option_buttons(t_option_menu *opt_menu,
	t_game *game, int x_mouse, int y_mouse)
{
	int	i;

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
}

void	draw_option_menu(t_game *game, t_option_menu *opt_menu)
{
	int	x_mouse;
	int	y_mouse;

	mlx_mouse_get_pos(game->mlx_ptr, game->win, &x_mouse, &y_mouse);
	if (opt_menu->pressed_slider_ref)
	{
		opt_menu->pressed_slider_ref->percent
			= (float)(x_mouse - opt_menu->pressed_slider_ref->pos.x
				- opt_menu->pressed_slider_ref->vert_image->size.x / 2.)
			/ opt_menu->pressed_slider_ref->size.x;
		if (opt_menu->pressed_slider_ref->percent < 0)
			opt_menu->pressed_slider_ref->percent = 0;
		else if (opt_menu->pressed_slider_ref->percent > 1)
			opt_menu->pressed_slider_ref->percent = 1;
	}
	draw_option_buttons(opt_menu, game, x_mouse, y_mouse);
	draw_button(&opt_menu->exit_opt_button, game->menu->image);
	draw_slider(game, &opt_menu->slider_fov, game->menu->image);
	draw_slider(game, &opt_menu->slider_sound, game->menu->image);
	draw_rectangle(game->menu->image, opt_menu->hor_bar_pos,
		opt_menu->hor_bar_size, COLOR_BAR_OPTION);
	draw_rectangle(game->menu->image, opt_menu->vert_bar_pos,
		opt_menu->vert_bar_size, COLOR_BAR_OPTION);
}
