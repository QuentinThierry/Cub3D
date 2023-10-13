/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 13:06:31 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

int	skip_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' || str[i] == '\n'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	return (i);
}

/**
 * @brief Get the number of different texture without the "config.cfg"
 * 
 * @param texture 
 * @param len 
 * @return int 
 */
int	get_len_texture(t_texture *texture, int len)
{
	int	i;
	int	j;
	int	res;

	res = 0;
	i = 0;
	while (i < len)
	{
		if (texture[i].filename != NULL)
			res++;
		else
		{
			res += texture[i].nb_file;
			j = 0;
			while (j < texture[i].nb_animation)
			{
				res += texture[i].animation[j].nb_sprite - 1;
				j++;
			}
		}
		i++;
	}
	return (res);
}

t_object	*find_empty_object(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->nb_objects)
	{
		if (game->object_array[i]->map_pos.x == -1
			&& game->object_array[i]->map_pos.x == -1)
			return (game->object_array[i]);
		i++;
	}
	return (NULL);
}

__attribute__((always_inline))
static inline void	my_mlx_pixel_put_sec(t_image *image, int x, int y,
						t_pixel32 color)
{
	if (y < 0 || x < 0 || x >= image->size.x || y >= image->size.y)
		return ;
	*(int *)(image->addr + (y * image->size_line + x * 4)) = color;
}

void	draw_rectangle(t_image *image, t_vector2 pos, t_vector2 size,
			t_pixel32 color)
{
	int	x;
	int	y;

	y = pos.y;
	while (y < size.y + pos.y)
	{
		x = pos.x;
		while (x < size.x + pos.x)
		{
			my_mlx_pixel_put_sec(image, x, y, color);
			x++;
		}
		y++;
	}
}
