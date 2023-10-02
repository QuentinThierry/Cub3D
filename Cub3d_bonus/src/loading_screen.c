/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_screen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 19:04:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/02 13:45:00 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

static const t_vector2	g_size_font = (t_vector2){WIN_X, WIN_Y / 16};
static const t_vector2	g_size_background = (t_vector2){WIN_X, WIN_Y};
static const t_vector2	g_size_loading_bar = (t_vector2){WIN_X / 3, WIN_Y / 16};


__attribute__((always_inline))
static inline unsigned int	get_pix_alpha(unsigned int dest, unsigned int src)
{
	float	color_quantity;

	color_quantity = ((src >> 24) & 0xff) / 255.;
	return (((unsigned char)(((src >> 16) & 0xFF) * color_quantity + ((dest >> 16) & 0xff) * (1 - color_quantity)) << 16)
		| ((unsigned char)(((src >> 8) & 0xFF) * color_quantity + ((dest >> 8) & 0xff) * (1 - color_quantity)) << 8)
		| (unsigned char)((src & 0xFF) * color_quantity + (dest & 0xff) * (1 - color_quantity)));
}
/**
 * @brief 
 * ! NO protection if draw outside of the window
 * 
 * @param dest_addr 
 * @param src 
 * @param begin_src 
 * @param size_src 
 */
void	draw_image_with_transparence(char *dest_addr, t_image *src
			, t_vector2 begin_src, t_vector2 size_src)
{
	int	y;
	int	x;
	int	start_dest;
	int	start_src;
	
	y = 0;
	start_src = begin_src.y * src->size_line + begin_src.x * 4;
	start_dest = 0;
	while (y < size_src.y)
	{
		x = 0;
		while (x < size_src.x * 4)
		{
			*(int*)(dest_addr + start_dest + x) = get_pix_alpha(*(int*)(dest_addr + start_dest + x), *(int*)(src->addr + start_src + x));
			x += 4;
		}
		start_dest += WIN_X * 4;
		start_src += (src->size.x) * 4;
		y++;
	}
}

/**
 * @brief 
 * ! NO protection if draw outside of the window
 * 
 * @param dest_addr 
 * @param src 
 * @param begin_src 
 * @param size_src 
 */
void	draw_image_with_green_sreen(char *dest_addr, t_image *src
			, t_vector2 begin_src, t_vector2 size_src)
{
	int	y;
	int	x;
	int	start_dest;
	int	start_src;
	
	y = 0;
	start_src = begin_src.y * src->size_line + begin_src.x * 4;
	start_dest = 0;
	while (y < size_src.y)
	{
		x = 0;
		while (x < size_src.x * 4)
		{
			if (*(int*)(src->addr + start_src + x) != GREEN_SCREEN)
				*(int*)(dest_addr + start_dest + x) = *(int*)(src->addr + start_src + x);
			x += 4;
		}
		start_dest += WIN_X * 4;
		start_src += (src->size.x) * 4;
		y++;
	}
}

void print_text(t_game *game, char *str, t_image *alpha, t_vector2 start_pos)
{
	int	i;

	i = 0;
	if (start_pos.x < 0)
		start_pos.x = 0;
	if (start_pos.y < 0)
		start_pos.y = 0;
	if (start_pos.y + game->subtitle_size_letter.y > game->image->size.y)
		return ;
	while (str[i])
	{
		if ((i + 1) * game->size_letter.x + start_pos.x > game->image->size.x)
			return ;
		else if (str[i] == 32)
		{
			i++;
			continue ;
		}
		else if (str[i] < 32 || str[i] > 126)
			draw_image_with_transparence(game->image->addr + (start_pos.y * game->image->size_line + (start_pos.x
				+ (int)(game->size_letter.x * i)) * 4) , alpha
				, (t_vector2){(int)(game->size_letter.x * ('?' - '!')), 0}
				, (t_vector2){(int)game->size_letter.x, (int)game->size_letter.y});
		else
			draw_image_with_transparence(game->image->addr + (start_pos.y * game->image->size_line + (start_pos.x
				+ (int)(game->size_letter.x * i)) * 4) , alpha
				, (t_vector2){(int)(game->size_letter.x * (str[i] - '!')), 0}
				, (t_vector2){(int)game->size_letter.x, (int)game->size_letter.y});
		i++;
	}
}

static char	*itoa_join(char *str, int nb)
{
	int		power;
	int		i;
	char	*res;
	
	i = 0;
	power = 100;
	res = ft_calloc(ft_strlen(str) + 5, sizeof(char));
	if (res == NULL)
		return (NULL);
	while (str[i])
	{
		res[i] = str[i];
		i++;
	}
	while (power > 0)
	{
		res[i] = nb / power + '0';
		if (!(res[i] == '0' && ft_strlen(str) == i))
			i++;
		nb = nb % power;
		power = power / 10;
	}
	res[i] = '%';
	return (res);
}

bool	update_loading_screen(t_game *game, t_loading *loading_screen)
{
	char		*text;
	t_vector2	size_bar;
	t_vector2	pos_text;

	loading_screen->nb_image_load++;
	size_bar.x = (float)loading_screen->center->size.x / game->nb_images * loading_screen->nb_image_load;
	size_bar.y = loading_screen->center->size.y;
	game->image->addr = ft_memcpy(game->image->addr
		, loading_screen->background->addr, WIN_X * WIN_Y * 4);
	draw_image_with_green_sreen(game->image->addr 
		+ ((WIN_Y / 2 - loading_screen->bordure->size.y / 2) * game->image->size_line + (WIN_X / 3) * 4)
		, loading_screen->bordure, (t_vector2){0}, g_size_loading_bar);
	draw_image_with_green_sreen(game->image->addr 
		+ ((WIN_Y / 2 - loading_screen->center->size.y / 2) * game->image->size_line + (WIN_X / 3) * 4)
		, loading_screen->center, (t_vector2){0}, size_bar);
	text = "Loading...";
	text = itoa_join(text, (int)((float)loading_screen->nb_image_load / game->nb_images * 100));
	if (text == NULL)
		return (false);
	pos_text = (t_vector2){WIN_X / 2 - (int)(ft_strlen(text) * game->size_letter.x / 2.)
		, WIN_Y / 2 - loading_screen->center->size.y / 2 - game->font->size.y};
	print_text(game, text, game->font, pos_text);
	free(text);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	return (true);
}

static bool	_load_image(t_game *game, t_loading *loading_screen)
{
	game->font = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_FONT, g_size_font);
	if (game->font == NULL)
		return (false);
	game->size_letter.x = game->font->size.x * WIDTH_LETTER / WIDTH_ALPHA;
	game->size_letter.y = game->font->size.y;
	loading_screen->background = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_SCREEN, g_size_background);
	if (loading_screen->background == NULL)
		return (false);
	loading_screen->bordure = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_BORDURE, g_size_loading_bar);
	if (loading_screen->bordure == NULL)
		return (false);
	loading_screen->center = btmlx_xpm_file_to_image(game->mlx_ptr
		, LOADING_CENTER, g_size_loading_bar);
	if (loading_screen->center == NULL)
		return (false);
	return (true);
}

/**
 * @brief load all the picture for the loading screen and draw the loading
 * 	screen
 * 
 * @param game 
 * @return true 
 * @return false 
 */
bool	loading_screen(t_game *game)
{
	t_loading *loading_screen;
	
	game->loading_screen = ft_calloc(1, sizeof(t_loading));
	if (game->loading_screen == NULL)
		return (false);
	loading_screen = game->loading_screen;
	if (!_load_image(game, loading_screen))
		return (false);
	game->image->addr = ft_memcpy(game->image->addr
		, loading_screen->background->addr, WIN_X * WIN_Y * 4);
	draw_image_with_green_sreen(game->image->addr 
		+ ((WIN_Y / 2 - loading_screen->bordure->size.y / 2) * game->image->size_line + (WIN_X / 3) * 4)
		, loading_screen->bordure, (t_vector2){0}, g_size_loading_bar);
	mlx_put_image_to_window(game->mlx_ptr, game->win, game->image->img, 0, 0);
	return (true);
}

/**
 * @brief free the struct loading screen and all the picture and set to NULL
 * 
 * @param game 
 */
void free_loading_screen(t_game *game)
{
	if (game->loading_screen == NULL)
		return ;
	if (game->loading_screen->background != NULL)
	{
		if (game->loading_screen->background->img != NULL)	
			mlx_destroy_image(game->mlx_ptr, game->loading_screen->background->img);
		free(game->loading_screen->background);
	}
	if (game->loading_screen->bordure != NULL)
	{
		if (game->loading_screen->bordure->img != NULL)	
			mlx_destroy_image(game->mlx_ptr, game->loading_screen->bordure->img);
		free(game->loading_screen->bordure);
	}
	if (game->loading_screen->center != NULL)
	{
		if (game->loading_screen->center->img != NULL)	
			mlx_destroy_image(game->mlx_ptr, game->loading_screen->center->img);
		free(game->loading_screen->center);
	}
	free(game->loading_screen);
	game->loading_screen = NULL;
}
