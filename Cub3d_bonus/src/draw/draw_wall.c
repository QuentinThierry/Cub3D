/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 17:24:19 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/14 18:55:44 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	draw_pixel_dark(int end, t_image *image, t_vector2 pos_screen);
void	draw_near_pixel(t_draw_infos *infos, t_image *image,
			int end, t_vector2 pos);

static int	set_infos_struct(t_draw_infos *infos, double height, t_ray ray,
		int x_door)
{
	int		y;

	infos->delta_y_img = infos->image->size.y / height;
	infos->img_pos.y = 0;
	y = WIN_Y / 2.0 - (int)(height / 2);
	if (y < 0)
	{
		infos->img_pos.y = -y * infos->delta_y_img;
		y = 0;
	}
	if (ray.orient == e_north || ray.orient == e_south)
		infos->img_pos.x = (ray.hit.x - (int)ray.hit.x) * infos->image->size.x;
	else
		infos->img_pos.x = (ray.hit.y - (int)ray.hit.y) * infos->image->size.x;
	if (ray.orient == e_west || ray.orient == e_south)
		infos->img_pos.x = infos->image->size.x - infos->img_pos.x - 1;
	if (x_door != -1)
		infos->img_pos.x = x_door;
	return (y);
}

void	draw_vert(t_game *game, int x, t_ray ray, double height)
{
	t_draw_infos		infos;
	register int		y;
	int					end;
	int					x_door;

	infos = (t_draw_infos){0};
	if (height == 0)
		return ;
	if (((int)height & 1) == 1)
		height++;
	end = WIN_Y / 2.0 + ((int)height - (int)(height / 2));
	if (end > WIN_Y)
		end = WIN_Y;
	infos.image = get_image_wall(game, ray, &x_door);
	if (infos.image->addr == NULL)
		return (print_error("Invalid image\n", 1), (void)ft_close(game));
	y = set_infos_struct(&infos, height, ray, x_door);
	if (game->dist_tab[x] >= DIST_MIN_DARK)
		infos.dark_quantity = (-DIST_MIN_DARK + game->dist_tab[x])
			/ (DIST_MAX_DARK - DIST_MIN_DARK);
	if (infos.dark_quantity >= 1)
		return (draw_pixel_dark(end, game->image, (t_vector2){x, y}));
	draw_near_pixel(&infos, game->image, end, (t_vector2){x, y});
}
