/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 00:16:42 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/02 19:51:56 by qthierry         ###   ########.fr       */
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

# include "minilibx-linux/mlx.h"
# include "get_next_line.h"

# include "raudio/src/raudio.h"

# include <X11/X.h>

# define WIN_X 1280 //1920 - 918
# define WIN_Y 720 //1080 - 468
# define CHUNK_SIZE 50
# define FOV 80
# define MOUV 1
# define SPEED 100
# define SPRINT_BOOST 100
# define ROTATION_KEYBOARD 125
# define ROTATION_MOUSE 20
# define SPEEP_DOOR_OPENING 100
# define MAX_VOLUME 1.0
# define TO_RADIAN .0174532
# define TRANSPARENT_PXL 0x00FF00

#define DARK_CONSTANT 5000
// between 0 and 255
#define DARK_MAXIMUN 250

// MINIMAP
#define MMAP_CHUNK 20
#define ZOOM_SPEED 10
#define ZOOM_OFFSET 20
#define MAX_ZOOM 20
#define MIN_ZOOM -10
// Represents the minimap padding equals to a percentage of the total window
#define MINIMAP_PAD 0.05

// Represents the minimap size equals to a percentage of the total window
#define MINIMAP_SIZE 0.25

#define PLANE_DIST 0
#define TAN_HALF_FOV 1
#define COS_HALF_FOV 2

// t_type for arg
#define NONE 0b0
#define WALL 0b1
#define DOOR_CLOSE 0b10
#define DOOR_OPEN 0b100
#define OBJECT 0b1000

#define NB_MAX_SOUNDS 16

#define PATH_MMAP_PLAYER "../assets/minimap_player.xpm"

extern long tot_fps;
extern long nb_fps;

typedef u_int32_t	t_pixel32;
typedef u_int32_t	t_type;
typedef Music		t_music;

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
	e_object
};

typedef struct s_vector2
{
	int	x;
	int	y;
}	t_vector2;

typedef struct s_fvector2
{
	double	x;
	double	y;
}	t_fvector2;

typedef struct s_ray
{
	t_fvector2			hit;
	enum e_orientation	orient;
	int					nb_object_hit;
}	t_ray;

typedef struct s_object
{
	char	symbol;
	t_type	type;
	float	dist;
}	t_objet;

typedef	struct s_player
{
	t_vector2	pos;
	t_fvector2 	f_pos;
	t_fvector2 	f_real_pos;
	t_vector2 	mouse_pos;
	double		angle;
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
	int			sign;
	long int	time;
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

typedef struct s_game
{
	t_image			*image;
	void			*mlx_ptr;
	void			*win;
	t_image			*tab_images;
	int				nb_images;
	t_texture		*filename;
	int				nb_file;
	t_map			**map;
	t_vector2		map_size;
	t_player		*player;
	t_minimap		*minimap;
	double			delta_time;
	long int		time;
	const double	*constants;
	t_music			*music_array;
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
void		free_str(char **str);
char		*ft_strjoin(char *str, char *str1);
char		*ft_strjoin_slash(char *str, char *str1, bool add_slash);
int			ft_atoi(const char *str);
int			get_len_texture(t_texture *texture, int len);

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
// -------Print--------
void		printf_texture(t_game *game);
void		print_map(t_game *game);

// -------Init---------
int			init_mlx(t_game *game);
bool		load_image_tab(t_game *game);
void		init_mouse(t_game *game);

// -------Hook---------
int			key_press_hook(int key, t_game *game);
int			key_release_hook(int key, t_game *game);
int			mouse_leave(t_game *game);
int			mouse_hook(int x,int y, t_game *game);
int			on_update(t_game *game);
void		player_move(t_player *player, double delta_time, t_map **map);
void		check_colliding(t_player *player, t_fvector2 new_pos, t_map **map);

// -------Raycasting-----
t_ray		get_wall_hit(t_fvector2 fpos, t_map **map, float angle, t_game *game);
double		get_wall_dist(t_game *game, double angle);
void		raycasting(t_game *game);
t_vector2	get_sign(double angle);


enum e_orientation	get_wall_orientation(t_fvector2 player, t_fvector2 wall);
t_image		*get_image_wall(t_game	*game, t_ray ray, int *x_door);
t_image	*get_image_non_wall(t_game *game, t_fvector2 hit, enum e_orientation orient);
int			ft_close(t_game *game);

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
t_fvector2	door_hit_ver_se(t_fvector2 hit, float step, float door_angle, float player_angle);
t_fvector2	door_hit_hor_se(t_fvector2 hit, float step, float door_angle, float player_angle);
t_fvector2	door_hit_ver_ne(t_fvector2 hit, float step, float door_angle, float player_angle);
t_fvector2	door_hit_hor_ne(t_fvector2 hit, float step, float door_angle, float player_angle);
t_fvector2	door_hit_ver_sw(t_fvector2 hit, float step, float door_angle, float player_angle);
t_fvector2	door_hit_hor_sw(t_fvector2 hit, float step, float door_angle, float player_angle);
t_fvector2	door_hit_ver_nw(t_fvector2 hit, float step, float door_angle, float player_angle);
t_fvector2	door_hit_hor_nw(t_fvector2 hit, float step, float door_angle, float player_angle);
void		open_door(t_vector2 map_size, t_map **map, double delta_time);
float		get_texture_door(t_ray ray);
void		step_door_open(t_door *door, long time, t_map *map_cell);

t_ray		get_object_hit(t_objet object, t_game *game, t_fvector2 begin, float angle);
void		draw_object(t_game *game, t_ray ray, int x, float angle);
double		get_dist(t_fvector2 fpos, t_fvector2 wall, double angle);

long int	time_to_long(struct timespec *time);

// floor.c
void		draw_ceiling(t_game *game);


#endif