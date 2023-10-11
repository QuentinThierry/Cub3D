/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:54:48 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 21:59:59 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_H
# define MENU_H

typedef struct s_image		t_image;
struct s_vector2;

typedef struct s_button
{
	t_image		*base_image;
	t_image		*hovered_image;
	struct s_vector2	pos;
	struct s_vector2	size;
	const char	*text;
	const char	*linked_text;
	bool		is_hovered;
}	t_button;

typedef struct t_slider
{
	struct s_vector2	pos;
	struct s_vector2	size;
	struct s_vector2	min_max_value;
	t_image		*hor_image;
	t_image		*vert_image;
	const char	*linked_text;
	float		percent;
}	t_slider;

typedef struct s_pause_menu
{
	t_button	play_button;
	t_button	option_button;
	t_button	quit_button;
}	t_pause_menu;

typedef struct s_option_menu
{
	t_button	buttons[NB_OPTIONS_BUTTONS];
	t_button	exit_opt_button;
	t_slider	slider_fov;
	t_slider	slider_sound;
	t_slider	*pressed_slider_ref;
	struct s_vector2	vert_bar_pos;
	struct s_vector2	vert_bar_size;
	struct s_vector2	hor_bar_pos;
	struct s_vector2	hor_bar_size;
	t_byte		pressed_button;
}	t_option_menu;

typedef struct s_menu
{
	t_image			*image;
	t_image			*background_image;
	t_image			*exit_option_image;
	t_byte			state;
	int				*h_rgb_blur_buffer;
	int				*v_rgb_blur_buffer;
	t_pause_menu	pause_menu;
	t_option_menu	option_menu;
	long			time_start_menu;
}	t_menu;

#endif