/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 13:55:10 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/15 15:13:44 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdbool.h>
# include <math.h>
# include <time.h>
# include <stdint.h>
# include <dirent.h>
# include <stdlib.h>
# include <stdio.h>
# include <X11/keysym.h>

# include "config.h"
# include "minilibx-linux/mlx.h"
# include "raudio/src/raudio.h"
# include "enum.h"

# define NB_OPTIONS_BUTTONS 11

typedef unsigned char	t_byte;
typedef u_int32_t		t_pixel32;
typedef u_int32_t		t_type;
typedef u_int32_t		t_keybind;
typedef Music			t_music;

typedef struct s_vector2
{
	int	x;
	int	y;
}	t_vector2;

typedef struct s_dvector2
{
	double	x;
	double	y;
}	t_dvector2;

typedef struct s_fvector2
{
	float	x;
	float	y;
}	t_fvector2;

typedef struct s_ray
{
	t_dvector2			hit;
	enum e_orientation	orient;
}	t_ray;

typedef struct s_launch_ray
{
	char	symbol;
	t_type	type;
	float	dist;
}	t_launch_ray;

typedef struct s_object
{
	t_dvector2		map_pos;
	bool			visited;
	float			dist;
	long int		time;
	char			sym_rcp;
	bool			is_completed;
	char			*music;
}	t_object;

typedef struct s_music_name
{
	char				*filename;
	char				*subtitle;
	unsigned int		offset;
	long int			time;
	t_orient			orient;
	char				symbol;
}	t_music_name;
typedef t_music_name	t_music_nam;

typedef struct s_image
{
	void		*img;
	char		*addr;
	int			opp;
	int			size_line;
	int			endian;
	t_vector2	size;
	int			time_frame;
	int			time_animation;
	int			nb_total_frame;
}	t_image;

typedef struct s_sprite
{
	int			index;
	int			frame;
	long int	time;
}	t_sprite;

// Use to stock the map
typedef struct s_map
{
	char				symbol;
	t_type				type;
	void				*arg;
	t_sprite			sprite[6];
	char				*music;
	struct s_music_name	*narrator;
}	t_map;

typedef struct s_music_game
{
	t_music		music;
	t_map		*map_cell;
	bool		is_playing;
	bool		is_subtitle;
}	t_music_game;

typedef struct s_player
{
	t_dvector2	f_pos;
	t_vector2	mouse_pos;
	float		angle;
	t_vector2	dir;
	int			view;
	float		speed;
	bool		has_item;
	t_map		item;
}	t_player;

typedef struct s_door
{
	float		door_percent;
	int			is_opening_door;
	t_vector2	map_pos;
	char		open_door;
	int			nb_receptacle_completed;
}	t_door;

/**
 * @brief use to stock the texture during the parsing
 *
 *	During the parsing we can find :
 *	char *							t_animation *
 * - filename
 * - directory	-> filename / s
 *				-> directory / s	-> config
 *									-> filenames
 */
typedef struct s_animation
{
	char	**filename;
	int		nb_sprite;
	int		time_sprite;
	int		time_animation;
}	t_animation;

typedef struct s_texture
{
	char				*filename;
	char				**filename_d;
	int					nb_file;
	t_animation			*animation;
	int					nb_animation;
	int					total;
	enum e_orientation	orient;
	char				symbol;
	char				sym_rcp;
}	t_texture;

typedef struct s_minimap
{
	t_image		*image;
	t_image		*buffer_img;
	t_image		*back_img;
	t_image		*player_img;
	int			*bounds;
	int			zoom_dir;
	float		zoom;
}	t_minimap;

typedef struct s_loading
{
	t_image		*background;
	t_image		*bordure;
	t_image		*center;
	int			nb_image_load;
}	t_loading;

typedef struct s_button
{
	t_image		*base_image;
	t_image		*hovered_image;
	t_vector2	pos;
	t_vector2	size;
	const char	*text;
	const char	*linked_text;
	bool		is_hovered;
}	t_button;

typedef struct t_slider
{
	t_vector2	pos;
	t_vector2	size;
	t_vector2	min_max_value;
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
	t_vector2	vert_bar_pos;
	t_vector2	vert_bar_size;
	t_vector2	hor_bar_pos;
	t_vector2	hor_bar_size;
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

typedef struct s_end
{
	t_image				*end_screen;
	enum e_status		status;
	t_fvector2			dest;
	t_fvector2			dir;
	float				dir_angle;
	int					dest_angle;
	enum e_orientation	orient;
}	t_end;

typedef struct s_game
{
	void			*mlx_ptr;
	void			*win;
	t_image			*image;
	t_image			*tab_images;
	int				nb_images;
	t_image			*font;
	t_dvector2		size_letter;
	t_image			*subtitle_font;
	t_dvector2		subtitle_size;
	t_texture		*filename;
	int				nb_file;
	t_music_name	*file_music;
	int				nb_music;
	t_map			**map;
	t_vector2		map_size;
	t_player		*player;
	t_minimap		*minimap;
	int				fov;
	double			delta_time;
	long int		time;
	float			constants[3];
	int				nb_objects;
	t_object		**object_array;
	int				nb_doors;
	t_map			**door_array;
	t_music_game	*music_array;
	float			*dist_tab;
	float			*height_tab;
	t_loading		*loading_screen;
	t_menu			*menu;
	t_keybind		*keybinds;
	t_map			*exit;
	int				total_receptacle;
	t_end			*end;
	struct timespec	*last_time;
}	t_game;

typedef struct s_object_infos
{
	t_game		*game;
	t_image		*image;
	t_fvector2	dim_draw;
}	t_object_infos;

typedef struct s_floor_infos
{
	t_game		*game;
	t_dvector2	map_point;
	t_vector2	last_map_pos;
	t_image		*img_ceil;
	t_image		*img_floor;
}	t_floor_infos;

typedef struct s_draw_infos
{
	t_image		*image;
	t_fvector2	img_pos;
	t_vector2	pos;
	float		dark_quantity;
	float		delta_y_img;
}	t_draw_infos;

typedef struct s_info_wall
{
	float angle;
	float player_angle;
	enum e_status status;
}	t_info_wall;

typedef struct s_info_get_object
{
	t_dvector2		max_pos;
	t_dvector2		step;
	t_launch_ray	obj;
}	t_info_get_object;

#endif