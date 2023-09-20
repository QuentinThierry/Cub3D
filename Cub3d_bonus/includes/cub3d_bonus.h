/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 00:16:42 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/20 20:39:34 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <strings.h>
# include <unistd.h>
# include <stdbool.h>
# include <math.h>
# include <time.h>
# include <stdint.h>
# include <dirent.h>
# include <pthread.h>
# include <signal.h>
# include <float.h>

# include "minilibx-linux/mlx.h"
# include "get_next_line.h"

# include "raudio/src/raudio.h"

// define for XK keybinds handle only in latin qwerty
#include <X11/keysym.h>


# define WIN_X 1280 //1920 - 918 - 1280
# define WIN_Y 720 //1080 - 468 - 720
# define CHUNK_SIZE 100
# define FOV 80		//
# define SPEED 100
# define SPRINT_BOOST 300
# define ROTATION_KEYBOARD 125	//
# define ROTATION_MOUSE 20		// + keyboard binds + play + quit + activer sous-titre 
# define SPEEP_DOOR_OPENING 100
# define TO_RADIAN .01745329251994
# define DARK_COLOR 0x101010
# define DIST_MAX_DARK 15.
# define DIST_MIN_DARK 3.
# define DIST_TO_WALL 0.0999

// MINIMAP
# define PATH_MMAP_PLAYER "../assets/minimap_player.xpm"
# define MINIMAP_PLAYER_SIZE 12
# define MMAP_CHUNK 20
# define ZOOM_SPEED 10
# define ZOOM_OFFSET 20
# define MAX_ZOOM 20
# define MIN_ZOOM -10
// Represents the minimap padding equals to a percentage of the total window
# define MINIMAP_PAD 0.05

// Represents the minimap size equals to a percentage of the total window
# define MINIMAP_SIZE 0.25

// access tab
# define PLANE_DIST 0
# define TAN_HALF_FOV 1
# define COS_HALF_FOV 2

# define NB_MAX_SOUNDS 16

// infos for loading screen
# define LOADING_SCREEN "./assets/smiley.xpm"
# define LOADING_BORDURE "./assets/loading_bordure.xpm"
# define LOADING_CENTER "./assets/loading_center.xpm"
# define LOADING_FONT "./assets/ascii.xpm"
# define WIDTH_ALPHA 1880
# define WIDTH_LETTER (1880 / 94.)
# define HEIGHT_ALPHA 34
# define GREEN_SCREEN 0x00ff00

// MENU
# define PAUSE_MENU 0
# define OPTION_MENU 1
# define CHOOSING_KEY_MENU 2

#define SIZE_BOX_BLUR 17
# define DARK_PERCENT_OPTION 0.5
# define NB_OPTIONS_BUTTONS 11

// KEYBINDS
# define DFL_KEY_LEFT_MOVE 'a'
# define DFL_KEY_RIGHT_MOVE 'd'
# define DFL_KEY_FORWARD_MOVE 'w'
# define DFL_KEY_BACKWARD_MOVE 's'
# define DFL_KEY_LEFT_LOOK XK_Left
# define DFL_KEY_RIGHT_LOOK XK_Right
# define DFL_KEY_PAUSE 'p'
# define DFL_KEY_MINIMAP_ZOOM '='
# define DFL_KEY_MINIMAP_DEZOOM '-'
# define DFL_KEY_INTERACT_DOOR ' '
# define DFL_KEY_SPRINT XK_Shift_L

// t_type for arg
# define NONE 0b0
# define WALL 0b1
# define DOOR 0b10
# define OBJECT 0b100
# define DOOR_NORTH 0b1000
# define DOOR_EAST 0b10000
# define DOOR_SOUTH 0b100000
# define DOOR_WEST 0b1000000

extern long tot_fps;
extern long nb_fps;

typedef unsigned char	t_byte;
typedef u_int32_t		t_pixel32;
typedef u_int32_t		t_type;
typedef u_int32_t		t_keybind;
typedef Music			t_music;

enum e_orientation
{
	e_north = 0,
	e_east,
	e_south,
	e_west,
	e_floor,
	e_ceiling,
	e_wall,
	e_door,
	e_object_wall,
	e_object_entity,
	e_object_image = e_north
};

enum e_keybinds
{
	e_key_left_move,
	e_key_right_move,
	e_key_forward_move,
	e_key_backward_move,
	e_key_left_look,
	e_key_right_look,
	e_key_pause,
	e_key_minimap_zoom,
	e_key_minimap_dezoom,
	e_key_interact_door,
	e_key_sprint
};

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
	t_dvector2	map_pos;
	bool		visited;
	float		dist;
	long int	time;	//for animation
}	t_object;

typedef	struct s_player
{
	t_dvector2 	f_real_pos;
	t_vector2 	mouse_pos;
	float		angle;
	t_vector2	dir;
	int			view;
	int			speed;
}	t_player;

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
	int			frame;		// -1 -> not a animation
	long int	time;
}	t_sprite;

// Use to stock the map
typedef struct s_map
{
	char		symbol;
	t_type		type;
	void		*arg;
	t_sprite	sprite[6];
}	t_map;

typedef struct s_door
{
	float		door_percent;
	int			is_opening_door;
	long int	time;
	t_vector2	map_pos;
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
	char				*filename;				//file
	char				**filename_d;			//dir
	t_animation			*animation;				//dir
	int					nb_file;				//dir
	int					nb_animation;			//dir
	int					total;					//all
	enum e_orientation	orient;					//all
	char				symbol;					//all
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
	bool		is_hovered;
}	t_button;

typedef struct s_menu
{
	t_image		*image;
	t_image		*background_image;
	t_image		*button_image;
	t_image		*button_hovered_image;
	int			*h_rgb_blur_buffer;
	int			*v_rgb_blur_buffer;
	t_button	buttons[NB_OPTIONS_BUTTONS];
	t_byte		pressed_button;
	t_byte		state;
}	t_menu;

typedef struct s_game
{
	t_image			*image;
	void			*mlx_ptr;
	void			*win;
	t_image			*tab_images;
	t_image			*font;
	t_dvector2		size_letter;
	int				nb_images;
	t_texture		*filename;
	int				nb_file;
	t_map			**map;
	t_vector2		map_size;
	t_player		*player;
	t_minimap		*minimap;
	double			delta_time;
	long int		time;
	const float		*constants;
	int				nb_objects;
	t_object		**object_array;
	int				nb_doors;
	t_map			**door_array;
	t_music			*music_array;
	float			*dist_tab;
	t_loading		*loading_screen;
	t_menu			*menu;
	t_keybind		*keybinds;
}	t_game;

// ------ Utils------
char		*ft_strdup(const char *s);
int			ft_strlen(char *str);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
void		*ft_realloc(void *ptr, size_t prev_size, size_t new_size);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		free_filename(t_game *game);
void		free_tab(void **str, int size);
void		free_map(t_map **map, t_vector2 size);
void		free_str(char **str);
char		*ft_strjoin(char *str, char *str1);
char		*ft_strjoin_slash(char *str, char *str1, bool add_slash);
int			ft_atoi(const char *str);
int			get_len_texture(t_texture *texture, int len);
void		ft_bzero(void *s, size_t n);

// -------Parsing-------
bool		parse_file(char *filename, t_game *game);
t_sprite	fill_texture(t_texture *tab, int len, char symbol, enum e_orientation orient);
t_vector2	get_dimension_maps(int fd, char *line, bool *error);
bool		is_wall(char symbol, t_texture *tab, int len, bool *error);
int			skip_whitespace(char *str);
bool		ft_fill_wall(t_game *game, char *line, t_map *map, t_vector2 map_size);
bool		find_player(t_game *game);
bool		check_map(t_game *game);
bool		ft_read_config(t_animation *animation, int index);
bool		parse_texture(int fd, t_game *game, int *nb_line, char **rest);
bool		is_door(char symbol, t_texture *tab, int len);
bool		is_object(char symbol, t_texture *tab, int len);
bool		fill_object_and_doors(t_game *game);

// -------Print--------
void		printf_texture(t_game *game);
void		print_map(t_game *game);

// -------Init---------
bool		init_mlx(t_game *game);
bool		load_image_tab(t_game *game);
bool		init_pause_menu(t_game *game);
void		init_mouse(t_game *game);


// -------Hook---------
void		key_press_hook(t_keybind key, t_game *game);
void		key_release_hook(t_keybind key, t_game *game);
int			mouse_leave(t_game *game);
int			mouse_hook(int x,int y, t_game *game);
int			on_update(t_game *game);
void		player_move(t_player *player, double delta_time, t_map **map);
int			mouse_click(int button, int x, int y,t_game *game);
int			ft_close(t_game *game);

void		check_colliding(t_player *player, t_dvector2 new_pos, t_map **map);

// -------Raycasting-----
t_ray		get_wall_hit(t_dvector2 fpos, t_map **map, float angle);
double		get_wall_dist(t_game *game, double angle);
void		raycasting(t_game *game);
t_vector2	get_sign(float angle);
void		draw_objects(t_game *game);


enum e_orientation	get_wall_orientation(t_dvector2 player, t_dvector2 wall);
t_image			*get_image_wall(t_game	*game, t_ray ray, int *x_door);
t_image			*get_image_non_wall(t_game *game, t_dvector2 hit, enum e_orientation orient);

// draw
void		draw_vert(t_game *game, int x, t_ray ray, double height);

// image_operations.c
void		draw_image_on_image_alpha(t_image *dest, t_image *src, t_vector2 offset_dest);

// bettermlx.c
t_image		*btmlx_new_image(void *mlx_ptr, t_vector2 size);
t_image		*btmlx_xpm_file_to_image(void *mlx, char *path,
				t_vector2 dst_size);

// Minimap
void		zoom_hook_handle(t_minimap *minimap, double delta_time);
void		draw_minimap(t_game *game);
void		generate_minimap_bounds(t_game *game);
bool		init_minimap(t_game *game);
void		draw_rotated_image(t_image *img_dest, t_image *img_src, t_vector2 pos, float angle);

// ------------ Door ----------
t_dvector2	door_hit_ver_se(t_dvector2 hit, float step, float door_angle, float player_angle);
t_dvector2	door_hit_hor_se(t_dvector2 hit, float step, float door_angle, float player_angle);
t_dvector2	door_hit_ver_ne(t_dvector2 hit, float step, float door_angle, float player_angle);
t_dvector2	door_hit_hor_ne(t_dvector2 hit, float step, float door_angle, float player_angle);
t_dvector2	door_hit_ver_sw(t_dvector2 hit, float step, float door_angle, float player_angle);
t_dvector2	door_hit_hor_sw(t_dvector2 hit, float step, float door_angle, float player_angle);
t_dvector2	door_hit_ver_nw(t_dvector2 hit, float step, float door_angle, float player_angle);
t_dvector2	door_hit_hor_nw(t_dvector2 hit, float step, float door_angle, float player_angle);
float		get_texture_door(t_ray ray);
void		update_doors(t_map **doors, int	nb_doors, long time, t_map **map);
void		open_door(t_game *game);


t_ray		get_object_hit(t_launch_ray object, t_game *game, t_dvector2 begin, float angle);
void		draw_objects(t_game *game);
// float		get_dist(t_dvector2 fpos, t_dvector2 wall);

long int	time_to_long(struct timespec *time);

// floor.c
void		draw_ceiling(t_game *game);

// ------ Loading screen ------
bool		loading_screen(t_game *game);
bool		update_loading_screen(t_game *game, t_loading *loading_screen);
void		free_loading_screen(t_game *game);
void		draw_image_with_transparence(char *dest_addr, t_image *src
				, t_vector2 begin_src, t_vector2 size_src);

// ------- menu ----------
void		draw_menu(t_game *game);
void		draw_menu(t_game *game);
int			menu_loop_hook(t_game *game);
const char	*get_key_str(t_keybind key);
void		menu_mouse_click(int button, int x, int y, t_game *game);
void		menu_key_hook(t_keybind key, t_game *game);


// unsigned int	dark_with_dist(int color, float dark_quantity);

#endif