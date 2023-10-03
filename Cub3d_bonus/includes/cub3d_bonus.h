/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 00:16:42 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/03 18:09:21 by qthierry         ###   ########.fr       */
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
# include <X11/keysym.h>

# include "minilibx-linux/mlx.h"
# include "minilibx-linux/mlx_int.h"
# include "get_next_line.h"
# include "raudio/src/raudio.h"

# include <X11/X.h>
# include <X11/Xlib.h>

# define WIN_X 1280 //1920 - 918 - 1280
# define WIN_Y 720 //1080 - 468 - 720
# define CHUNK_SIZE 100
# define DFL_FOV 100 // check min is less than min and diff > 0
# define MIN_FOV 70
# define MAX_FOV 130
# define DFL_SOUND 0.5 // check between 0 and 1
# define SPEED 1
# define SPRINT_BOOST 1
# define ROTATION_KEYBOARD 125
# define ROTATION_MOUSE 20
# define SPEEP_DOOR_OPENING 100
# define SPEEP_UNLOCK_DOOR_OPENING 50
# define TO_RADIAN .01745329251994
# define DARK_COLOR 0x101010
# define DIST_MAX_DARK 15.
# define DIST_MIN_DARK 3.
# define DIST_TO_WALL 0.0999
# define MAX_MENU_FPS 120

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
# define MINIMAP_WALL_COLOR 0x505050
# define MINIMAP_BACKGROUND_COLOR 0x808080
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
# define END_SCREEN "./assets/end.xpm"

// MENU
# define PAUSE_MENU 0
# define OPTION_MENU 1
# define CHOOSING_KEY_MENU 2

# define SIZE_BOX_BLUR 9
# define DARK_PERCENT_OPTION 0.5
# define NB_OPTIONS_BUTTONS 11
# define NB_SLIDERS 11
# define KEY_TEXT_CHANGE "Press any key"
# define COLOR_BAR_OPTION 0x707070
# define DARK_COLOR_OPTION 0x101010
# define DIST_TO_WALL 0.0999

# define BACKGROUND_MUSIC "./assets/sounds/test1.mp3"


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
# define DOOR_NORTH_END 0b10000000
# define DOOR_EAST_END 0b100000000
# define DOOR_SOUTH_END 0b1000000000
# define DOOR_WEST_END 0b10000000000
# define OBJECT_INTERACTIVE 0b100000000000
# define RECEPTACLE 0b1000000000000
# define DOOR_LOCK 0b10000000000000
# define DOOR_UNLOCK 0b100000000000000
# define EXIT 0b1000000000000000
# define MUSIC 0b10000000000000000
# define MUSIC_OBJECT 0b100000000000000000
# define NARRATOR 0b1000000000000000000
# define NARRATOR_RECEPTACLE 0b10000000000000000000
# define IS_PLAYING_MUSIC 0b100000000000000000000
# define IS_PLAYING_MUSIC_OBJECT 0b1000000000000000000000
# define IS_PLAYING_NARRATOR 0b10000000000000000000000



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
	e_door_lock,
	e_door_unlock,
	e_object_wall,
	e_object_entity,		//transparent
	e_object_interactive,
	e_object_interactive_hand,
	e_object_interactive_before,
	e_object_interactive_after,
	e_receptacle_empty,
	e_receptacle_full,
	e_exit,
	e_music,
	e_music_object,
	e_music_receptacle,
	e_music_receptacle_complete,
	e_narrator,
	e_narrator_receptacle,
	e_narrator_receptacle_complete,
	e_end_screen,
	e_object_image = e_north,
	e_door_image = e_north,
	e_object_interactive_image = e_north,
	e_object_interactive_hand_image,
	e_object_interactive_before_image,
	e_object_interactive_after_image,
	e_receptacle_empty_image = e_north,
	e_receptacle_full_image
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

typedef struct s_music_name
{
	char				*filename;
	char				*subtitle;
	unsigned int		offset;
	long int			time;
	enum e_orientation	orient;
	char				symbol;
}	t_music_name;

typedef struct s_object
{
	t_dvector2		map_pos;
	bool			visited;
	float			dist;
	long int		time;	//for animation
	char			symbol_receptacle;
	bool			is_completed;	//receptacle
	char			*music;
}	t_object;

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
	char			symbol;
	t_type			type;
	void			*arg;
	t_sprite		sprite[6];
	char			*music;
	t_music_name	*narrator;
}	t_map;

typedef struct s_music_game
{
	t_music		music;
	t_map		*map_cell;
	bool		is_playing;
	bool		is_subtitle;
}	t_music_game;

typedef	struct s_player
{
	t_dvector2	f_real_pos;
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
	long int	time;
	t_vector2	map_pos;
	char		symbol_unlock_door;
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

char		*ft_strdup(const char *s);

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
	char				symbol_receptacle;		//all
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
}	t_menu;

enum e_status
{
	e_go_in_font_of_door = 0,
	e_open_door,
	e_walk_through_door,
	e_end
};

typedef struct s_end
{
	enum e_status	status;
	t_fvector2		dest;
	t_fvector2		dir;
	float			dir_angle;
	int				dest_angle;
	enum e_orientation orient;
	t_image			*end_screen;
}	t_end;

typedef struct s_game
{
	t_image			*image;
	void			*mlx_ptr;
	void			*win;
	t_image			*tab_images;
	int				nb_images;
	t_image			*font;
	t_dvector2		size_letter;
	t_image			*subtitle_font;
	t_dvector2		subtitle_size_letter;
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
}	t_game;

// ------ Utils------
int			ft_strlen(const char *str);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
void		*ft_realloc(void *ptr, size_t prev_size, size_t new_size);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		free_filename(t_game *game);
void		free_tab(void **str, int size);
void		free_map(t_map **map, t_vector2 size);
void		free_tab_object(t_object **str, int size);
void		free_str(char **str);
void		free_music_file(t_music_name *music_tab, int size);
void		free_minimap(t_minimap *minimap, void *mlx_ptr);
char		*ft_strjoin(char *str, char *str1);
char		*ft_strjoin_slash(char *str, char *str1, bool add_slash);
int			ft_atoi(const char *str);
int			get_len_texture(t_texture *texture, int len);
void		ft_bzero(void *s, size_t n);
int			find_next_wsp(char *line , int i);
void		draw_rectangle(t_image *image, t_vector2 pos, t_vector2 size, t_pixel32 color);
void		print_error(char *error, int print);

// -------Parsing-------
void		exit_door_no_receptacle(t_map *exit, int nb_receptacle, t_image *tab_image);
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
bool		is_door(char symbol, t_texture *tab, int len, t_texture *type_door);
bool		is_object(char symbol, t_texture *tab, int len);
bool		is_object_interactive(char symbol, t_texture *tab, int len);
bool		is_receptacle(char symbol, t_texture *tab, int len, char *c);
bool		is_sound(t_music_name *filename, int nb_music, char symbol);
bool		fill_object_and_doors(t_game *game);
bool		find_music(t_game *game, char *str, enum e_orientation orient, int i);

// -------Print--------
void		printf_texture(t_game *game);
void		print_map(t_game *game);
void		printf_music(t_game *game);

// -------Init---------
bool		init_game(t_game *game, char *filename);
bool		init_mlx(t_game *game);
bool		load_image_tab(t_game *game, bool *print_error);
void		init_mouse(t_game *game);


// -------Hook---------
void		key_press_hook(t_keybind key, t_game *game);
void		key_release_hook(t_keybind key, t_game *game);
int			exit_hook(int key, t_game *game);
int			mouse_leave(t_game *game);
int			mouse_hook(int x,int y, t_game *game);
int			mouse_stay_in_window_hook(int x, int y, t_game *game);
int			on_update(t_game *game);
void		player_move(t_game *game, t_player *player, double delta_time, t_map **map);
int			mouse_click(int button, int x, int y,t_game *game);
int			ft_close(t_game *game);

t_dvector2	check_colliding(t_game *game, t_dvector2 new_pos, t_map **map);

// -------Raycasting-----
t_ray		get_wall_hit(t_dvector2 fpos, t_map **map, float angle);
double		get_wall_dist(t_game *game, double angle);
void		raycasting(t_game *game);
t_vector2	get_sign(float angle);
void		draw_objects(t_game *game);


enum e_orientation	get_wall_orientation(t_dvector2 player, t_dvector2 wall);
t_image		*get_image_wall(t_game	*game, t_ray ray, int *x_door);
t_image		*get_image_non_wall(t_game *game, t_dvector2 hit, enum e_orientation orient);

// draw
void		draw_vert(t_game *game, int x, t_ray ray, double height);

// image_operations.c
void		draw_image_on_image_alpha(t_image *dest, t_image *src, t_vector2 offset_dest);

// bettermlx.c
t_image		*btmlx_new_image(void *mlx_ptr, t_vector2 size);
t_image		*btmlx_xpm_file_to_image(void *mlx, char *path,
				t_vector2 dst_size);
t_image		*btmlx_xpm_file_to_image_bilinear_resize(void *mlx, char *path,
				t_vector2 dst_size);

// Minimap
void		zoom_hook_handle(t_minimap *minimap, double delta_time);
void		draw_minimap(t_game *game);
void		generate_minimap_bounds(t_game *game);
bool		init_minimap(t_game *game);
void		draw_rotated_image(t_image *img_dest, t_image *img_src, t_vector2 pos, float angle);
void		draw_rectangular_minimap(t_game *game);
void		draw_minimap_buf_on_main_image(t_minimap *mmap, t_image *image);

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
void		end_step_door_open(double time, t_map *map_cell, t_map **map, t_end *end);
void		update_doors(t_map **doors, int	nb_doors, double time, t_map **map);
void		open_door(t_game *game);


t_ray		get_object_hit(t_launch_ray object, t_map **map, t_dvector2 begin, float angle);
void		draw_objects(t_game *game);

long int	time_to_long(struct timespec *time);

// floor.c
void		draw_ceiling(t_game *game);

// ------ Loading screen ------
bool		loading_screen(t_game *game);
bool		update_loading_screen(t_game *game, t_loading *loading_screen);
void		free_loading_screen(t_game *game);
void		draw_image_with_transparence(char *dest_addr, t_image *src
				, t_vector2 begin_src, t_vector2 size_src);
void		draw_image_with_green_sreen(char *dest_addr, t_image *src
				, t_vector2 begin_src, t_vector2 size_src);

// ------- menu ----------
int			menu_loop_hook(t_game *game);
const char	*get_key_str(t_keybind key);
void		menu_mouse_down_hook(int button, int x, int y, t_game *game);
void		menu_mouse_up_hook(int mouse_button, int x, int y, t_game *game);
void		menu_key_hook(t_keybind key, t_game *game);
void		set_pause_menu_mode(t_game *game);
bool		init_pause_menu(t_game *game);
void		draw_text_in_button(t_game *game, t_image *image, t_button *button);
void		draw_button(t_button *button, t_image *image);
void		apply_menu_dark_filter(t_image *menu_image);
void		draw_option_menu(t_game *game, t_option_menu *opt_menu);
void		draw_pause_menu(t_game *game, t_pause_menu *pause_menu);
void		resume_menu(t_game *game, t_menu *menu);
void		draw_centered_text_at_y(t_game *game, t_image *image, int y, const char *text);
void		check_mouse_is_in_button(t_button *button, int x, int y);
void		draw_text_at_with_backgroud(t_game *game, t_image *image, t_vector2 pos, const char *text);
void		draw_alpha_rectangle(t_image *dest, t_vector2 pos, t_vector2 size);


// ------ Blur ------------
void		blur_image(t_image *dest, t_image *src,
				int *h_rgb_blur_buffer, int *v_rgb_blur_buffer);

// ------ Object interactive -----
void		take_object_click(t_game *game, t_player *player, t_map **map);
void		take_object(t_game *game, t_player *player, t_map *cell_map, t_music_game *music_tab);
void		drop_object(t_player *player, t_map **map, t_map *exit, t_game *game);
t_object	*find_empty_object(t_game *game);
void		draw_hand_item(t_game *game, t_player *player);

// ----------END ------------
bool		init_end_screen(t_game *game);
void		end_of_the_game(t_game *game, enum e_orientation orient);
t_ray		get_wall_hit_end(t_dvector2 fpos, t_map **map, float angle, enum e_status status);

// -------- Music ----------
char			*get_music(t_music_name *filename, int nb_music, char symbol, enum e_orientation orient);
t_music_name	*get_narrator(t_music_name *filename, int nb_music, char symbol, enum e_orientation orient);
bool			init_audio(t_game *game, t_music_name *music_file, int nb_music);
void			update_sounds(t_music_game *music_array);
void			close_audio(t_music_game *music_tab);
void			play_music(t_map *map_cell, t_music_game *music_tab, char *filename, unsigned int type);
void			play_narrator(t_game *game, t_map *map_cell, t_music_game *music_tab);
void			play_sound_fail(t_game *game, t_map *map_cell, t_music_game *music_tab);
void			set_next_narrator(t_map *map_cell);
void			update_map_cell_music(t_map *map_cell, t_map *old_map_cell, t_music_game *music_array);
void			clear_sound(t_music_game *music_array);
void			print_subtitle(t_game *game, t_map *map_cell);

#endif