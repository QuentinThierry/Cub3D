/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/09 21:03:37 by qthierry         ###   ########.fr       */
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

#include "minilibx-linux/mlx.h"
#include "get_next_line.h"

#include <X11/X.h>

#define WIN_X 1080 //1920 - 918
#define WIN_Y 1080 //1080 - 468
#define CHUNK_SIZE 50
#define FOV 120
#define MOUV 1
#define SPEED 100
#define SPRINT_BOOST 100
#define ROTATION_KEYBOARD 125
#define ROTATION_MOUSE 20
#define MAX_VOLUME 1.0
#define THREED 1
#define TO_RADIAN .0174532

extern long tot_fps;
extern long nb_fps;

enum e_orientation
{
	e_north = 0,
	e_east,
	e_south,
	e_west
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

typedef	struct s_player{
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
}	t_image;

typedef struct s_game
{
	t_image			*image;
	void			*mlx_ptr;
	void			*win;
	t_image			**tab_images;
	unsigned int	floor;
	unsigned int	ceiling;
	char			**map;
	t_vector2		map_size;
	t_player		*player;
	double			delta_time;
	char			**filename;
	const double	*constants;
}	t_game;

// ------ Utils------
void	*ft_calloc(size_t nmemb, size_t size);
enum e_orientation	get_wall_orientation(t_game *game, t_player player, t_fvector2 wall);
t_image	*get_image(t_game	*game, enum e_orientation orient);
int skip_whitespace(char *str);
t_vector2	get_dimension_maps(int fd, int nb_line, char *line, bool *error);
void	remove_new_line(char *str);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	printf_texture(t_game *game);
void	free_tab(char **str, t_vector2 size);
void	free_str(char **str);

int			init_mlx(t_game *game);
int			load_image(t_game *game);
int			key_press_hook(int key, t_game *game);
int			key_release_hook(int key, t_player *player);
int			mouse_hook(int x, int y, t_game *game);
int			mouse_leave(t_game *game);
void		player_move(t_player *player, double delta_time, char **map, t_vector2 map_size);
void		draw_vert(t_game *game, int x, t_fvector2 wall, double dist);
void		quadrillage(t_game *game);
double		get_wall_dist(t_game *game, double angle);
void		raycasting(t_game *game);
int			on_update(t_game *game);
t_vector2	get_sign(double angle);
int			ft_close(t_game *game);
int			parse_file(char *filename, t_game *game);
void		print_map(t_game *game);
t_fvector2	get_wall_hit(t_fvector2 fpos, char **map, float angle, t_vector2 map_size);
bool		check_map(t_game *game);

// --------2D--------
t_fvector2	get_wall_hit_2d(t_game *gavoidme, double angle);
void		raycasting_2d(t_game *game);
void		quadrillage(t_game *game);
bool		find_player(t_game *game);

void	check_colliding(t_player *player, t_fvector2 new_pos, char **map);

#endif