/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/13 17:30:50 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <strings.h>
# include <unistd.h>
# include <stdbool.h>
# include <math.h>
# include <time.h>

# include "ao.h"
#include "../includes/minilibx-linux/mlx.h"
#include "../includes/get_next_line.h"

#include <X11/X.h>

#define WIN_X 400
#define WIN_Y 300
#define CHUNK_SIZE 50
#define FOV 60

typedef struct s_vector2
{
	int	x;
	int	y;
}	t_vector2;

typedef struct s_fvector2
{
	float	x;
	float	y;
}	t_fvector2;

typedef	struct s_player{
	t_vector2	pos;
	float	angle;
	float	f_x;
	float	f_y;
}	t_player;

typedef struct s_image
{
	void	*ptr;
	char	*data;
	int		bpp;
	int		size_line;
	int		endian;
}	t_image;

typedef struct s_game
{
	void		*mlx_ptr;
	void		*win;
	char		**maps;
	t_image		image;
	t_player	player;
}	t_game;



void parse_wav_file(int fd, ao_sample_format *format, long *data_size);
int	key_press_hook(int key, t_game *game);
void	print_map(char **maps);
void	my_mlx_pixel_put(t_image *img, int x, int y, int color);
void	draw_vert(t_game *game, int x, int y1, int y2);
void	quadrillage(t_game *game);
int	get_wall_dist(t_game *game, float angle);
void	raycasting(t_game *game);
t_player	find_player(char **maps);
int	on_update(t_game *game);

#endif