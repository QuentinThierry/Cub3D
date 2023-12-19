/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/12/19 19:09:59 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdbool.h>
# include <math.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>

# include "minilibx-linux/mlx.h"

# define WIN_X 1080 //1920 - 918
# define WIN_Y 1080 //1080 - 468
# define CHUNK_SIZE 50
# define FOV 85
# define MOUV 1
# define SPEED 1.25
# define SPRINT_BOOST 100
# define ROTATION 1
# define TO_RADIAN .0174532
# define ESC 65307
# define LEFT_ARROW 65361
# define RIGHT_ARROW 65363
# define SHIFT 65505

enum e_orientation
{
	e_north = 0,
	e_east,
	e_south,
	e_west
};

enum e_images
{
	e_north_wall,
	e_east_wall,
	e_south_wall,
	e_west_wall,
	e_ceiling,
	e_floor
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

typedef struct s_map_arg
{
	char		**map;
	t_vector2	map_size;
}	t_map_arg;

typedef struct s_player
{
	t_vector2	pos;
	t_fvector2	f_pos;
	t_fvector2	f_real_pos;
	double		angle;
	t_vector2	dir;
	int			view;
}	t_player;

typedef struct s_image
{
	void		*img;
	char		*addr;
	int			opp;
	int			size_line;
	int			endian;
	t_vector2	size;
}	t_image;

typedef struct s_draw
{
	t_fvector2	img;
	double		delta_y_img;
	int			y_begin;
	int			y_end;
}	t_draw;

typedef struct s_game
{
	t_image			*image;
	void			*mlx_ptr;
	void			*win;
	t_image			**tab_images;
	unsigned int	floor_color;
	unsigned int	ceiling_color;
	char			**map;
	t_vector2		map_size;
	t_player		*player;
	char			**filename;
	double			consts;
}	t_game;

// ------ Utils------
void				*ft_calloc(size_t nmemb, size_t size);
extern void			*ft_memcpy(void *dest, const void *src, const size_t n);
int					ft_strlen(char *str);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strdup(const char *s);
void				*ft_memset(void *s, int c, size_t n);

// ------ Free ------
int					ft_close(t_game *game);
void				free_tab(char **str, int sizey);

//------- Hook ------
int					key_press_hook(int key, t_game *game);
int					key_release_hook(int key, t_player *player);
void				player_move(t_player *player);
int					on_update(t_game *game);

// ------ Init ------
void				remove_end_whitespace(char *str);
bool				parse_map(char *filename, t_game *game, int nb_line);
bool				create_map(t_game *game, int fd, char *line);
bool				find_color(char *str, t_game *game, char texture);
bool				init_all(t_game *game);
int					find_next_wsp(char *line, int i);
bool				parse_file(char *filename, t_game *game);
bool				parse_texture(int fd, t_game *game, int *nb_line,
						char **rest);
t_vector2			get_dimension_maps(int fd, char *line, bool *error);
bool				find_player(t_game *game);
int					skip_whitespace(char *str);
void				remove_new_line(char *str);
bool				check_map(t_game *game);
void				bettermlx_get_data_addr(t_image *image, t_vector2 size);

// ------ Raycasting ----
void				raycasting(t_game *game);
enum e_orientation	get_wall_orientation(t_player player, t_fvector2 wall);
void				draw_vert(t_game *game, int x, t_fvector2 wall,
						double dist);
t_fvector2			get_wall_hit(t_fvector2 fpos, char **map, float angle,
						t_vector2 map_size);
t_fvector2			get_wall_hit_se(t_fvector2 fpos,
						char **map, float angle, t_vector2 map_size);
t_fvector2			get_wall_hit_ne(t_fvector2 fpos,
						char **map, float angle, t_vector2 map_size);
t_fvector2			get_wall_hit_sw(t_fvector2 fpos,
						char **map, float angle, t_vector2 map_size);
t_fvector2			get_wall_hit_nw(t_fvector2 fpos,
						char **map, float angle);

// ------- Print -------
void				print_error(char *error, int print);
void				print_map(t_game *game);
void				printf_texture(t_game *game);

#endif