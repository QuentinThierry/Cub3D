/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:50:23 by qthierry          #+#    #+#             */
/*   Updated: 2023/10/05 15:54:00 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

__attribute__((always_inline))
static inline unsigned int	get_color_at(char *addr, int size_line, t_vector2 pos)
{
	return (*(int *)(addr + (pos.y * size_line + pos.x * 4)));
}

__attribute__((always_inline))
static inline void	my_mlx_pixel_put(char *addr, int size_line, t_vector2 pos, int color)
{
	*(int *)(addr + (pos.y * size_line + pos.x * 4)) = color;
}

__attribute__((always_inline))
static inline float	get_dist(t_dvector2 fpos, t_dvector2 wall)
{
	return (((wall.x - fpos.x) * (wall.x - fpos.x) + (wall.y - fpos.y) * (wall.y - fpos.y)));
}

__attribute__((always_inline))
static inline unsigned int	dark_with_dist(unsigned int color, float dark_quantity)
{
	float	color_quantity;

	color_quantity = 1 - dark_quantity;
	return (((unsigned char)(((color >> 16) & 0xFF) * color_quantity + ((DARK_COLOR >> 16) & 0xff) * dark_quantity) << 16)
		| ((unsigned char)(((color >> 8) & 0xFF) * color_quantity + ((DARK_COLOR >> 8) & 0xff) * dark_quantity) << 8)
		| (unsigned char)((color & 0xFF) * color_quantity + (DARK_COLOR & 0xff) * dark_quantity));
}

__attribute__((always_inline))
static inline bool	is_in_map(t_dvector2 pos, t_map **map, t_vector2 size_map)
{
	if (pos.x < size_map.x && pos.x >= 0 && pos.y < size_map.y && pos.y >= 0
		&& map[(int)pos.y][(int)pos.x].symbol != ' ')
		return (true);
	return (false);
}

__attribute__((always_inline))
static inline void	draw_pixel_line(t_game *game, register t_dvector2 map_point, register t_fvector2 step_dir, int y_screen)
{
	register int	i;
	const t_image	*game_image = game->image;
	t_vector2		last_map_pos;
	t_image			*image;
	t_image			*image2;
	register float	dark_quantity;
	register int	color_ceiling;
	register int	color_floor;
	register float	dist;

	i = 0;
	last_map_pos.x = -(int)map_point.x;
	last_map_pos.y = -(int)map_point.y;
	while (i < WIN_X)
	{
		if ((game->height_tab[i] > y_screen && game->end->status == -1) || !is_in_map(map_point, game->map, game->map_size))
		{
			map_point.x += step_dir.x;
			map_point.y += step_dir.y;
			i++;
			continue ;
		}
		if (game->map[(int)map_point.y][(int)map_point.x].sprite[e_ceiling].index != -1)
		{
			if (last_map_pos.x != (int)map_point.x || last_map_pos.y != (int)map_point.y)
			{
				last_map_pos.x = (int)map_point.x;
				last_map_pos.y = (int)map_point.y;
				image = get_image_non_wall(game, map_point, e_ceiling);
				image2 = get_image_non_wall(game, map_point, e_floor);
			}
			dist = get_dist(game->player->f_real_pos, map_point);
			if (dist >= DIST_MIN_DARK * DIST_MIN_DARK)
			{
				dark_quantity = (-DIST_MIN_DARK + sqrtf(dist)) / (DIST_MAX_DARK - DIST_MIN_DARK);
				if (dark_quantity >= 1)
				{
					color_ceiling = DARK_COLOR;
					color_floor = DARK_COLOR;
				}
				else
				{
					color_ceiling = dark_with_dist(get_color_at(image->addr, image->size_line,
								(t_vector2){(map_point.x - last_map_pos.x) * image->size.x,
								(map_point.y - last_map_pos.y) * image->size.y}), dark_quantity);
					color_floor = dark_with_dist(get_color_at(image2->addr, image2->size_line,
								(t_vector2){(map_point.x - last_map_pos.x) * image2->size.x,
								(map_point.y - last_map_pos.y) * image2->size.y}), dark_quantity);
				}
			}
			else
			{
				dark_quantity = 0;
				color_ceiling = get_color_at(image->addr, image->size_line,
						(t_vector2){(map_point.x - last_map_pos.x) * image->size.x,
						(map_point.y - last_map_pos.y) * image->size.y});
				color_floor = get_color_at(image2->addr, image2->size_line,
						(t_vector2){(map_point.x - last_map_pos.x) * image2->size.x,
						(map_point.y - last_map_pos.y) * image2->size.y});
			}
			my_mlx_pixel_put(game_image->addr, game_image->size_line,
				(t_vector2){i, WIN_Y / 2 - y_screen}, color_ceiling);
			my_mlx_pixel_put(game_image->addr, game_image->size_line,
				(t_vector2){i, WIN_Y / 2 + y_screen - 1}, color_floor);
		}
		map_point.x += step_dir.x;
		map_point.y += step_dir.y;
		i++;
	}
}

void	draw_line_ceiling(t_game *game, t_fvector2 xdist_yscreen, t_fvector2 cos_sin, t_fvector2 hit)
{
	float				dist_to_left;
	float				h;
	t_fvector2			a;
	t_dvector2			map_point;
	t_dvector2			left_p;

	dist_to_left = fabsf(game->constants[TAN_HALF_FOV] * xdist_yscreen.x);
	h = xdist_yscreen.x / game->constants[COS_HALF_FOV];
	left_p.x = cos_sin.x * h;
	left_p.y = cos_sin.y * h;
	a.y = ((hit.y - left_p.y) / dist_to_left) * fabsf(dist_to_left * 2) / WIN_X;
	a.x = ((hit.x - left_p.x) / dist_to_left) * fabsf(dist_to_left * 2) / WIN_X;
	map_point.x = game->player->f_real_pos.x + left_p.x;
	map_point.y = game->player->f_real_pos.y + left_p.y;
	draw_pixel_line(game, map_point, a, xdist_yscreen.y);
}

// if odd WIN_Y, draw 1 floor pixel more than ceiling
void	draw_ceiling(t_game *game)
{
	t_fvector2	hit;
	t_fvector2	cos_sin1;
	t_fvector2	cos_sin2;
	int			y_screen;
	float		x_dist;

	y_screen = WIN_Y / 2.;
	cos_sin2.x = cosf((game->player->angle - 90 - game->fov / 2.) * TO_RADIAN);
	cos_sin2.y = sinf((game->player->angle - 90 - game->fov / 2.) * TO_RADIAN);
	cos_sin1.x = cosf((game->player->angle - 90) * TO_RADIAN);
	cos_sin1.y = sinf((game->player->angle - 90) * TO_RADIAN);
	while (y_screen > 0)
	{
		x_dist = (0.5 * (game->constants[0] / y_screen));
		hit.x = cos_sin1.x * x_dist;
		hit.y = cos_sin1.y * x_dist;
		draw_line_ceiling(game, (t_fvector2){x_dist, y_screen}, cos_sin2, hit);
		y_screen--;
	}
}
