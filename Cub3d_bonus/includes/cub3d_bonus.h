/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 00:16:42 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/13 20:58:39 by qthierry         ###   ########.fr       */
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
# include "libao/include/ao/ao.h"

# include "minilibx-linux/mlx.h"
# include "minilibx-linux/mlx_int.h"
# include "get_next_line.h"

# include <X11/X.h>

# define WIN_X 1000 //1920 - 918
# define WIN_Y 1000 //1080 - 468
# define CHUNK_SIZE 50
# define FOV 120
# define MOUV 1
# define SPEED 100
# define SPRINT_BOOST 100
# define ROTATION_KEYBOARD 125
# define ROTATION_MOUSE 20
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

#define PATH_MMAP_PLAYER "../assets/minimap_player.xpm"

extern long tot_fps;
extern long nb_fps;

typedef u_int32_t t_pixel32;

enum e_orientation
{
	e_north = 0,
	e_east,
	e_south,
	e_west,
	e_down,
	e_up,
	e_wall
};

enum e_texture
{
	e_north_wall = 0,
	e_east_wall,
	e_south_wall,
	e_west_wall,
	e_floor,
	e_ceiling,
	e_door_close,
	e_door_open,
	e_total
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
}	t_ray;

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
	bool		is_wall;
	t_sprite	sprite[6];
	int			door_percent;
}	t_map;

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

// --------------SOUND-------------------
typedef struct s_sound
{
	ao_device			*device;
	ao_sample_format	format;
	char				*buffer;
	long				buf_size;
}	t_sound;

typedef struct	s_sound_thread
{
	t_sound			sound;
	int				player_angle;
	t_vector2		emitter_pos;
	t_vector2		listener_pos;
	pthread_mutex_t	mut_play_sound;
}	t_sound_thread;

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
	bool			sound_end;
	pthread_mutex_t	sound_end_mut;
	pthread_t		sound_thread;
}	t_game;

// sound
void		parse_wav_file(int fd, ao_sample_format *format, long *data_size);
void		*sound_manager_thr(t_game *game);

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
t_ray		get_wall_hit(t_fvector2 fpos, t_map **map, float angle, t_vector2 map_size);
double		get_wall_dist(t_game *game, double angle);
void		raycasting(t_game *game);
t_vector2	get_sign(double angle);


enum e_orientation	get_wall_orientation(t_fvector2 player, t_fvector2 wall);
t_image		*get_image(t_game	*game, enum e_orientation orient, t_fvector2 wall);
int			ft_close(t_game *game);

// draw
void	draw_vert(t_game *game, int x, t_ray ray, double height);

// image_operations.c
void	draw_image_on_image_alpha(t_image *dest, t_image *src, t_vector2 offset_dest);

// bettermlx.c
t_image	*btmlx_new_image(void *mlx_ptr, t_vector2 size);
t_image	*btmlx_xpm_file_to_image(void *mlx, char *path,
			t_vector2 dst_size);

// Minimap
void	zoom_hook_handle(t_minimap *minimap, double delta_time);
void	draw_minimap(t_game *game);
void	generate_minimap_bounds(t_game *game);
bool	init_minimap(t_game *game);
void	draw_rotated_image(t_image *img_dest, t_image *img_src, t_vector2 pos, float angle);

long int	time_to_long(struct timespec *time);

#endif