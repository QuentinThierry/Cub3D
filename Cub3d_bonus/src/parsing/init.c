/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/23 22:17:33 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	init_mlx(t_game *game)
{
	game->image = ft_calloc(1, sizeof(t_image));
	if (game->image == NULL)
		return (false);
	game->mlx_ptr = mlx_init();
	if (game->mlx_ptr == NULL)
		return (false);
	game->win = mlx_new_window(game->mlx_ptr, WIN_X, WIN_Y, "cub3d");
	if (game->win == NULL)
		return (false);
	game->image->img = mlx_new_image(game->mlx_ptr, WIN_X, WIN_Y);
	if (game->win == NULL)
		return (false);
	game->image->addr = mlx_get_data_addr(game->image->img,
		&game->image->opp, &game->image->size_line, &game->image->endian);
	if (game->image->opp != 32)
		return (false); // If mlx returns a number of plane different that 4, stop the program
	game->image->size = (t_vector2){WIN_X, WIN_Y};
	game->image->opp /= 8;
	if (game->win == NULL)
		return (false);
	return (true);
}

void	init_mouse(t_game *game)
{
	mlx_mouse_hide(game->mlx_ptr, game->win);
	mlx_mouse_move(game->mlx_ptr, game->win, WIN_X / 2, WIN_Y / 2);
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

bool	init_keybinds(t_game *game)
{
	game->keybinds = ft_calloc(NB_OPTIONS_BUTTONS, sizeof(int));
	if (!game->keybinds)
		return (false);
	game->keybinds[e_key_left_move] = DFL_KEY_LEFT_MOVE;
	game->keybinds[e_key_right_move] = DFL_KEY_RIGHT_MOVE;
	game->keybinds[e_key_forward_move] = DFL_KEY_FORWARD_MOVE;
	game->keybinds[e_key_backward_move] = DFL_KEY_BACKWARD_MOVE;
	game->keybinds[e_key_left_look] = DFL_KEY_LEFT_LOOK;
	game->keybinds[e_key_right_look] = DFL_KEY_RIGHT_LOOK;
	game->keybinds[e_key_pause] = DFL_KEY_PAUSE;
	game->keybinds[e_key_minimap_zoom] = DFL_KEY_MINIMAP_ZOOM;
	game->keybinds[e_key_minimap_dezoom] = DFL_KEY_MINIMAP_DEZOOM;
	game->keybinds[e_key_interact_door] = DFL_KEY_INTERACT_DOOR;
	game->keybinds[e_key_sprint] = DFL_KEY_SPRINT;
	return (true);
}

bool	init_pause_menu(t_game *game)
{
	int				i;
	t_image			*button_image;
	t_image			*button_hovered_image;
	t_option_menu	*opt_menu;
	t_pause_menu	*pause_menu;

	if (!init_keybinds(game))
		return (false);
	game->menu = ft_calloc(1, sizeof(t_menu));
	if (!game->menu)
		return (false);
	game->menu->h_rgb_blur_buffer = ft_calloc(WIN_X * WIN_Y * 3, sizeof(int));
	if (!game->menu->h_rgb_blur_buffer)
		return (false);
	game->menu->v_rgb_blur_buffer = ft_calloc(WIN_X * WIN_Y * 3, sizeof(int));
	if (!game->menu->v_rgb_blur_buffer)
		return (false);
	game->menu->image = btmlx_new_image(game->mlx_ptr, (t_vector2){WIN_X, WIN_Y});
	if (!game->menu->image)
		return (false);
	game->menu->background_image = btmlx_new_image(game->mlx_ptr, (t_vector2){WIN_X, WIN_Y});
	if (!game->menu->background_image)
		return (false);
	button_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/button.xpm", (t_vector2){200, 80});
	if (!button_image)
		return (false);
	button_hovered_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/button_hovered.xpm", (t_vector2){200, 80});
	if (!button_hovered_image)
		return (false);
	opt_menu = &game->menu->option_menu;
	pause_menu = &game->menu->pause_menu;
	opt_menu->exit_opt_button.base_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/button_exit_option.xpm", (t_vector2){100, 40});
	if (!opt_menu->exit_opt_button.base_image)
		return (false);
	i = 0;
	while (i < NB_OPTIONS_BUTTONS)
	{
		opt_menu->buttons[i].base_image = button_image;
		opt_menu->buttons[i].size = (t_vector2){200, 80};
		opt_menu->buttons[i].hovered_image = button_hovered_image;
		opt_menu->buttons[i].text = get_key_str(game->keybinds[i]);
		if (i < NB_OPTIONS_BUTTONS / 2 + (NB_OPTIONS_BUTTONS & 1))
		{
			opt_menu->buttons[i].pos =
				(t_vector2){200, i * opt_menu->buttons[i].size.y + 20 * (i + 1)};
		}
		else
		{
			opt_menu->buttons[i].pos =
				(t_vector2){WIN_X / 2 + 200, (i - NB_OPTIONS_BUTTONS / 2 - (NB_OPTIONS_BUTTONS & 1))
				* opt_menu->buttons[i].size.y + 20
				* (i - NB_OPTIONS_BUTTONS / 2 + ((NB_OPTIONS_BUTTONS & 1) == 0))};
		}
		i++;
	}
	opt_menu->exit_opt_button.hovered_image = button_hovered_image;
	opt_menu->exit_opt_button.pos = (t_vector2){10, 10};
	opt_menu->exit_opt_button.size = (t_vector2){100, 40};
	opt_menu->exit_opt_button.text = "";


	opt_menu->slider_fov.hor_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/slider_hor.xpm",(t_vector2){100, 20});
	if (!opt_menu->slider_fov.hor_image)
		return (false);
	opt_menu->slider_fov.vert_image = btmlx_xpm_file_to_image(game->mlx_ptr, "./assets/slider_vert.xpm",(t_vector2){20, 33});
	if (!opt_menu->slider_fov.hor_image)
		return (false);
	opt_menu->slider_fov.size = (t_vector2){100, 33};
	opt_menu->slider_fov.pos = (t_vector2){WIN_X / 2, WIN_Y / 2};
	opt_menu->slider_fov.min_percent = 
		-(opt_menu->slider_fov.vert_image->size.x / 2.) /
		opt_menu->slider_fov.hor_image->size.x;
	opt_menu->slider_fov.max_percent = 1 + opt_menu->slider_fov.min_percent;

	pause_menu->play_button.base_image = button_image;
	pause_menu->play_button.hovered_image = button_hovered_image;
	pause_menu->play_button.size = (t_vector2){200, 80};
	pause_menu->play_button.pos =
		(t_vector2){WIN_X / 2 - pause_menu->play_button.size.x / 2,
		WIN_Y / 3 - pause_menu->play_button.size.y / 2};
	pause_menu->play_button.text = "RESUME";

	pause_menu->option_button.base_image = button_image;
	pause_menu->option_button.hovered_image = button_hovered_image;
	pause_menu->option_button.size = (t_vector2){200, 80};
	pause_menu->option_button.pos =
		(t_vector2){WIN_X / 2 - pause_menu->option_button.size.x / 2,
		WIN_Y / 2 - pause_menu->option_button.size.y / 2};
	pause_menu->option_button.text = "OPTIONS";

	pause_menu->quit_button.base_image = button_image;
	pause_menu->quit_button.hovered_image = button_hovered_image;
	pause_menu->quit_button.size = (t_vector2){200, 80};
	pause_menu->quit_button.pos =
		(t_vector2){WIN_X / 2 - pause_menu->quit_button.size.x / 2,
		WIN_Y / 3 * 2 - pause_menu->quit_button.size.y / 2};
	pause_menu->quit_button.text = "QUIT GAME";
	return (true);
}
