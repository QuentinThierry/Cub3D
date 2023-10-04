/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/04 14:54:34 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"


static bool	_init_value_malloc(t_game *game)
{
	game->fov = DFL_FOV;
	game->constants[0] = (WIN_X / 2.) / tanf((DFL_FOV / 2.) * TO_RADIAN);
	game->constants[1] = tanf((DFL_FOV / 2.0) * TO_RADIAN);
	game->constants[2] = cosf((DFL_FOV / 2.0) * TO_RADIAN);
	game->nb_file = 6;
	game->filename = ft_calloc(game->nb_file, sizeof(t_texture));
	if (game->filename == NULL)
		return (print_error(NULL, 0), false);
	game->dist_tab = ft_calloc(WIN_X, sizeof(float));
	if (game->dist_tab == NULL)
		return (print_error(NULL, 0), false);
	game->height_tab = ft_calloc(WIN_X, sizeof(float));
	if (game->height_tab == NULL)
		return (print_error(NULL, 0), false);
	return (true);
}

bool	init_game(t_game *game, char *filename)
{
	if (!_init_value_malloc(game))
		return (ft_close(game), false);
	if (!parse_file(filename, game))
		return (ft_close(game), false);
	if (!check_map(game))
		return (ft_close(game), false);
	if (!init_mlx(game))
		return (print_error(NULL, 0), ft_close(game), false);
	if (!init_audio(game, game->file_music, game->nb_music))
		return (ft_close(game), false);
	if (!loading_screen(game))
		return (print_error(NULL, 0), ft_close(game), false);
	if (!init_end_screen(game))
		return (print_error(NULL, 0), ft_close(game), false);
	if (!init_minimap(game))
		return (print_error(NULL, 0), ft_close(game), false);
	if (!init_pause_menu(game))
		return (print_error(NULL, 0), ft_close(game), false);
	return (true);
}

static void	_init_hook(t_game *game)
{
	mlx_do_key_autorepeatoff(game->mlx_ptr);
	mlx_hook(game->win, 2, (1L << 0), (void *)key_press_hook, game);
	mlx_hook(game->win, 3, (1L << 1), (void *)key_release_hook, game);
	mlx_hook(game->win, 4, (1L<< 2), mouse_click, game);
	mlx_hook(game->win, 5, (1L << 3), NULL, game);
	mlx_hook(game->win, 6, (1L << 6) , mouse_hook, game);
	mlx_hook(game->win, 8, (1L << 5), mouse_leave, game);
	mlx_hook(game->win, 17, (1L << 8), ft_close, game);
	mlx_loop_hook(game->mlx_ptr, on_update, game);
}

bool	init_mlx(t_game *game)
{
	game->mlx_ptr = mlx_init();
	if (game->mlx_ptr == NULL)
		return (false);
	game->win = mlx_new_window(game->mlx_ptr, WIN_X, WIN_Y, "cub3D");
	if (game->win == NULL)
		return (false);
	init_mouse(game);
	game->image = ft_calloc(1, sizeof(t_image));
	if (game->image == NULL)
		return (false);
	game->image->img = mlx_new_image(game->mlx_ptr, WIN_X, WIN_Y);
	if (game->image->img == NULL)
		return (false);
	game->image->addr = mlx_get_data_addr(game->image->img,
		&game->image->opp, &game->image->size_line, &game->image->endian);
	if (game->image->opp != 32)
		return (false);
	game->image->size = (t_vector2){WIN_X, WIN_Y};
	game->image->opp /= 8;
	_init_hook(game);
	return (true);
}

void	move_mouse(t_game *game)
{
	mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
	game->player->mouse_pos.x = WIN_X / 2;
	game->player->mouse_pos.y = WIN_Y / 2;
}

void	init_mouse(t_game *game)
{
	mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
	// mlx_mouse_hide(game->mlx_ptr, game->win);
	game->player->mouse_pos.x = WIN_X / 2;
	game->player->mouse_pos.y = WIN_Y / 2;
}

bool	load_image(t_game *game, t_image *img, char *filename, t_animation *anim)
{
	img->img = mlx_xpm_file_to_image(game->mlx_ptr, filename,
			&(img->size.x), &(img->size.y));
	if (img->img == NULL)
		return (false);
	img->addr = mlx_get_data_addr(img->img,
		&img->opp, &img->size_line, &img->endian);
	if (img->addr == NULL)
		return (false);
	img->opp /= 8;
	if (!update_loading_screen(game, game->loading_screen))
		return (false);
	if (anim == NULL)
		return (true);
	img->time_animation = anim->time_animation;
	img->time_frame = anim->time_sprite;
	img->nb_total_frame = anim->nb_sprite - 1;
	return (true);
}

static t_image	*_resize_img(t_image *src,
					t_vector2 dst_size, t_image *dst)
{
	t_dvector2	ratio;
	int			x;
	int			y;

	ratio.x = (double)(src->size.x) / (double)(dst_size.x);
	ratio.y = (double)(src->size.y) / (double)(dst_size.y);
	y = -1;
	while (++y < dst_size.y)
	{
		x = -1;
		while (++x < dst_size.x)
		{
			*(unsigned int *)(dst->addr + (int)(y) *dst->size_line
					+ (int)(x) *(dst->opp))
				= *(unsigned int *)(src->addr + (int)(y * ratio.y)
					*src->size_line
					+ (int)(x * ratio.x) *(src->opp));
		}
	}
	return (dst);
}

bool	load_resize_image(t_game *game, t_image *img, char *filename, t_vector2 dst_size)
{
	t_image dst;

	img->img = mlx_xpm_file_to_image(game->mlx_ptr, filename,
			&(img->size.x), &(img->size.y));
	if (img->img == NULL)
		return (false);
	img->addr = mlx_get_data_addr(img->img,
		&img->opp, &img->size_line, &img->endian);
	if (img->addr == NULL)
		return (false);
	img->opp /= 8;
	if ((img->size.x == dst_size.x && img->size.y == dst_size.y)
		|| (dst_size.x == 0 || dst_size.y == 0))
		return (true);

	dst.img = mlx_new_image(game->mlx_ptr, dst_size.x, dst_size.y);
	if (!dst.img)
		return (false);
	dst.addr = mlx_get_data_addr(dst.img,
		&dst.opp, &dst.size_line, &dst.endian);
	if (dst.addr == NULL)
		return (false);
	dst.opp /= 8;
	dst.size = dst_size;
	dst = *_resize_img(img, dst_size, &dst);
	mlx_destroy_image(game->mlx_ptr, img->img);
	*img = dst;
	if (!update_loading_screen(game, game->loading_screen))
		return (false);
	return (true);
}

bool	load_image_tab(t_game *game, bool *print_error)
{
	t_image		*tab_image;
	int			i;
	int			j;
	int			h;
	int			index;

	*print_error = true;
	game->subtitle_font = btmlx_xpm_file_to_image_bilinear_resize(game->mlx_ptr, LOADING_FONT
			, (t_vector2){WIN_X / 3 * 2, WIN_Y / 16 / 3 * 2});
	if (game->subtitle_font == NULL)
		return (false);
	game->subtitle_size_letter.x = game->subtitle_font->size.x * WIDTH_LETTER / WIDTH_ALPHA;
	game->subtitle_size_letter.y = game->subtitle_font->size.y;
	game->nb_images = get_len_texture(game->filename, game->nb_file);
	game->tab_images = ft_calloc(game->nb_images, sizeof(t_image));
	if (game->tab_images == NULL)
		return (-1);
	i = 0;
	index = 0;
	tab_image = game->tab_images;
	while (i < game->nb_file)
	{
		if (game->filename[i].filename != NULL)
		{
			if (game->filename[i].orient == e_object_interactive_hand)
			{
				if (!load_resize_image(game, &(tab_image[index]), game->filename[i].filename, (t_vector2){WIN_X / 3, WIN_X / 3}))
					return (false);
			}
			else if (!load_image(game, &(tab_image[index]), game->filename[i].filename, NULL))
				return (false);
			index++;
		}
		else
		{
			j = 0;
			while (j < game->filename[i].nb_file)
			{
				if (game->filename[i].orient == e_object_interactive_hand)
				{
					if (!load_resize_image(game, &(tab_image[index]), game->filename[i].filename_d[j], (t_vector2){WIN_X / 3, WIN_X / 3}))
						return (false);
				}
				else if (!load_image(game, &(tab_image[index]), game->filename[i].filename_d[j], NULL))
					return (false);
				j++;
				index++;
			}
			j = 0;
			while (j < game->filename[i].nb_animation)
			{
				h = 1;
				if (!ft_read_config(&(game->filename[i].animation[j]), 0))
					return (*print_error = false, false);
				while (h < game->filename[i].animation[j].nb_sprite)
				{
					if (game->filename[i].orient == e_object_interactive_hand)
					{
						if (!load_resize_image(game, &(tab_image[index]), game->filename[i].animation[j].filename[h], (t_vector2){WIN_X / 3, WIN_X / 3}))
							return (false);
						tab_image[index].time_animation = game->filename[i].animation[j].time_animation;
						tab_image[index].time_frame = game->filename[i].animation[j].time_sprite;
						tab_image[index].nb_total_frame = game->filename[i].animation[j].nb_sprite - 1;
					}
					else if (!load_image(game, &(tab_image[index]), game->filename[i].animation[j].filename[h], &(game->filename[i].animation[j])))
						return (false);
					index++;
					h++;
				}
				j++;
			}
		}
		i++;
	}
	return (true);
}
