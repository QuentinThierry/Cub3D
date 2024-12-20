/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pause_menu.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:57:18 by qthierry          #+#    #+#             */
/*   Updated: 2023/12/04 15:55:13 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

// with background
void	draw_centered_text_at_y(t_game *game,
	t_image *image, int y, const char *text)
{
	int			i;
	t_vector2	pos;

	if (game->size_letter.y + y > image->size.y)
		return ;
	pos.x = WIN_X / 2. - (ft_strlen(text) * game->size_letter.x) / 2;
	draw_alpha_rectangle(image,
		(t_vector2){pos.x - game->size_letter.x / 2, y},
		(t_vector2){(ft_strlen(text) + 1) * game->size_letter.x,
		game->size_letter.y});
	if (ft_strlen(text) * game->size_letter.x > image->size.x)
		pos.x = 0;
	pos.y = y;
	i = 0;
	while (text[i])
	{
		if ((i + 1) * game->size_letter.x + pos.x > image->size.x)
			return ;
		if (text[i] == ' ')
		{
			i++;
			continue ;
		}
		draw_image_with_transparence(image->addr + (pos.y * image->size_line
				+ (pos.x + (int)(game->size_letter.x * i)) * 4), game->font,
			(t_vector2){(game->size_letter.x * (text[i] - '!')), 0},
			(t_vector2){game->size_letter.x, game->size_letter.y});
		i++;
	}
}

void	draw_text_at_with_backgroud(t_game *game,
	t_image *image, t_vector2 pos, const char *text)
{
	int	i;

	if (game->size_letter.y + pos.y > image->size.y)
		return ;
	draw_alpha_rectangle(image,
		(t_vector2){pos.x - game->size_letter.x / 2, pos.y},
		(t_vector2){(ft_strlen(text) + 1) * game->size_letter.x,
		game->size_letter.y});
	i = 0;
	while (text[i])
	{
		if ((i + 1) * game->size_letter.x + pos.x > image->size.x)
			return ;
		if (text[i] == ' ')
		{
			i++;
			continue ;
		}
		else if (text[i] < 32 || text[i] > 126)
			draw_image_with_transparence(image->addr + (pos.y * image->size_line
					+ (pos.x + (int)(game->size_letter.x * i)) * 4), game->font,
				(t_vector2){(game->size_letter.x * ('?' - '!')), 0},
				(t_vector2){game->size_letter.x, game->size_letter.y});
		else
			draw_image_with_transparence(image->addr + (pos.y * image->size_line
					+ (pos.x + (int)(game->size_letter.x * i)) * 4), game->font,
				(t_vector2){(game->size_letter.x * (text[i] - '!')), 0},
				(t_vector2){game->size_letter.x, game->size_letter.y});
		i++;
	}
}

void	draw_text_in_button(t_game *game, t_image *image, t_button *button)
{
	int			i;
	t_vector2	start_pos;

	i = 0;
	start_pos.y = button->pos.y + button->size.y / 2. - game->size_letter.y / 2;
	if (start_pos.y + game->size_letter.y > button->pos.y + button->size.y)
		return ;
	start_pos.x = button->pos.x + button->size.x / 2.
		- (ft_strlen(button->text) * game->size_letter.x) / 2;
	if (start_pos.x < button->pos.x)
		start_pos.x = button->pos.x;
	while (button->text[i])
	{
		if (button->text[i] == ' ')
		{
			i++;
			continue ;
		}
		if ((i + 1) * game->size_letter.x + start_pos.x > button->pos.x + button->size.x)
			return ;
		draw_image_with_transparence(image->addr + (start_pos.y * image->size_line
				+ (start_pos.x + (int)(game->size_letter.x * i)) * 4), game->font,
			(t_vector2){(game->size_letter.x * (button->text[i] - '!')), 0},
			(t_vector2){game->size_letter.x, game->size_letter.y});
		i++;
	}
}

void	draw_button(t_button *button, t_image *image)
{
	t_image			*button_image;
	register int	y;

	if (button->is_hovered)
		button_image = button->hovered_image;
	else
		button_image = button->base_image;
	y = 0;
	while (y < button->size.y)
	{
		ft_memcpy(image->addr + (button->pos.y + y) * image->size_line + button->pos.x * 4,
			button_image->addr + y * button_image->size_line, button_image->size_line);
		y++;
	}
}

void	adjust_animation_time(t_game *game)
{
	int				i;
	int				j;
	int				k;
	long			time_diff;
	struct timespec	time_now;

	clock_gettime(CLOCK_REALTIME, &time_now);
	time_diff = time_to_long(&time_now) - game->menu->time_start_menu;
	i = 0;
	while (i < game->map_size.y)
	{
		j = 0;
		while (j < game->map_size.x)
		{
			k = 0;
			while (k < 6)
			{
				if (game->map[i][j].type != 0 && game->map[i][j].sprite[k].index != -1 && 
					game->map[i][j].sprite[k].frame != -1)
					game->map[i][j].sprite[k].time += time_diff;
				k++;
			}
			j++;
		}
		i++;
	}
	*game->last_time = time_now;
}

void update_subtitle(t_game *game)
{
	struct timespec	time;

	if (game->music_array[1].is_subtitle == true
		&& game->music_array[1].map_cell != NULL
		&& game->music_array[1].map_cell->narrator != NULL)
	{
		clock_gettime(CLOCK_REALTIME, &time);
		game->music_array[1].map_cell->narrator->time += (time_to_long(&time)
				- game->menu->time_start_menu);
	}
}

void	resume_menu(t_game *game, t_menu *menu)
{
	if (menu->state == PAUSE_MENU)
	{
		game->fov = MIN_FOV + (MAX_FOV - MIN_FOV) * game->menu->option_menu.slider_fov.percent;
		game->constants[0] = (WIN_X / 2.) / tan((game->fov / 2.) * TO_RADIAN);
		game->constants[1] = tanf((game->fov / 2.0) * TO_RADIAN);
		game->constants[2] = cos((game->fov / 2.0) * TO_RADIAN);
		SetMasterVolume(game->menu->option_menu.slider_sound.percent);
		mlx_hook(game->win, 2, (1L << 0), (void *)key_press_hook, game);
		mlx_hook(game->win, 3, (1L << 1), (void *)key_release_hook, game);
		mlx_hook(game->win, 5, (1L << 3), NULL, NULL);
		mlx_hook(game->win, 6, (1L << 6), mouse_hook, game);
		mlx_hook(game->win, 8, (1L << 5), mouse_leave, game);
		mlx_mouse_hook(game->win, mouse_click, game);
		mlx_loop_hook(game->mlx_ptr, on_update, game);
		update_subtitle(game);
		if (ft_strcmp(game->menu->pause_menu.play_button.text, "RESUME") == 0)
			adjust_animation_time(game);
		else
			game->menu->pause_menu.play_button.text = "RESUME";
		init_mouse(game);
	}
	else if (menu->state == OPTION_MENU)
		menu->state = PAUSE_MENU;
}

void	check_mouse_is_in_button(t_button *button, int x, int y)
{
	button->is_hovered = (x > button->pos.x
			&& x < button->pos.x + button->size.x
			&& y > button->pos.y
			&& y < button->pos.y + button->size.y);
}

void	draw_pause_menu(t_game *game, t_pause_menu *pause_menu)
{
	int	x_mouse;
	int	y_mouse;

	mlx_mouse_get_pos(game->mlx_ptr, game->win, &x_mouse, &y_mouse);
	check_mouse_is_in_button(&pause_menu->play_button, x_mouse, y_mouse);
	draw_button(&pause_menu->play_button, game->menu->image);
	draw_text_in_button(game, game->menu->image, &pause_menu->play_button);
	check_mouse_is_in_button(&pause_menu->option_button, x_mouse, y_mouse);
	draw_button(&pause_menu->option_button, game->menu->image);
	draw_text_in_button(game, game->menu->image, &pause_menu->option_button);
	check_mouse_is_in_button(&pause_menu->quit_button, x_mouse, y_mouse);
	draw_button(&pause_menu->quit_button, game->menu->image);
	draw_text_in_button(game, game->menu->image, &pause_menu->quit_button);
}

void	set_pause_menu_mode(t_game *game)
{
	struct timespec	time_start;

	clock_gettime(CLOCK_REALTIME, &time_start);
	game->menu->time_start_menu = time_to_long(&time_start);
	
	mlx_hook(game->win, 2, (1L << 0), (void *)menu_key_press, game);
	mlx_hook(game->win, 3, (1L << 1), (void *)menu_key_release, game);
	mlx_hook(game->win, 4, (1L << 2), (void *)menu_mouse_down_hook, game);
	mlx_hook(game->win, 5, (1L << 3), (void *)menu_mouse_up_hook, game);
	mlx_hook(game->win, 8, (1L << 5), NULL, NULL);
	mlx_hook(game->win, 6, (1L << 6), NULL, NULL);
	mlx_mouse_show(game->mlx_ptr, game->win);
	mlx_loop_hook(game->mlx_ptr, menu_loop_hook, game);
	apply_menu_dark_filter(game->image, DARK_PERCENT_PAUSE);
	blur_image(game->menu->background_image,
		game->image, game->menu->h_rgb_blur_buffer,
		game->menu->v_rgb_blur_buffer);
	game->menu->state = PAUSE_MENU;
}
