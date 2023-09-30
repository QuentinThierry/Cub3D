/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subtitle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 12:38:01 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/30 16:28:58 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static const t_vector2	g_minimap_size =
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE
};

static const t_vector2	g_minimap_pos = 
(t_vector2)
{
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD,
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) - 
	(((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_SIZE) -
	((WIN_X <= WIN_Y) * WIN_X + (WIN_X > WIN_Y) * WIN_Y) * MINIMAP_PAD
};

void	draw_text_at_with_backgroud1(t_game *game, t_image *image, t_vector2 pos, const char *text)
{
	int			i;

	if (game->size_letter.y + pos.y > image->size.y)
		return ;
	draw_alpha_rectangle(image, (t_vector2){pos.x - game->size_letter.x / 2,
		pos.y}, (t_vector2){(ft_strlen(text) + 1) * game->size_letter.x,
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
		draw_image_with_transparence(image->addr + (pos.y * image->size_line + (pos.x
			+ (int)(game->size_letter.x * i)) * 4), game->font,
		(t_vector2){(game->size_letter.x * (text[i] - '!')), 0},
		(t_vector2){game->size_letter.x, game->size_letter.y});
		i++;
	}
}

void print_text(t_game *game, char *str, t_image *alpha, t_vector2 start_pos);

void	print_subtitle(t_game *game, t_map *map_cell)
{
	const t_vector2 pos = {g_minimap_pos.x + g_minimap_size.x + game->size_letter.x
		, g_minimap_pos.y + g_minimap_size.y / 2.};
	const t_vector2 pos1 = {g_minimap_pos.x + g_minimap_size.x + game->size_letter.x
		, g_minimap_pos.y + (g_minimap_size.y / 2.) + game->size_letter.y};
	// printf("draw subtitle\n");
	// draw_text_at_with_backgroud1(game, game->image, pos,"Coucou voici notre Cub3d Coucou voici notre Cub3d Coucou voici notre Cub3d\n");
	// draw_text_at_with_backgroud1(game, game->image, pos1,"Coucou voici notre Cub3d Coucou voici notre Cub3d Coucou voici notre Cub3d\n");
}