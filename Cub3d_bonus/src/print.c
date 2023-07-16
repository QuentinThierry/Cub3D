/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 01:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/16 01:49:26 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	printf_texture(t_game *game)
{
	int i = 0;
	while (i < game->nb_sprite)
	{
		printf("texture %s	char : %c	orientation : %d	index : %d\n"
				, game->filename[i].filename, game->filename[i].symbol, game->filename[i].orient, i);
		i++;
	}
}

void	print_map(t_game *game)
{
	int y = 0;
	int x = 0;
	
	while(y < game->map_size.y)
	{
		x = 0;
		while(x < game->map_size.x)
		{
			// write(1, "|", 1);
			write(1, &(game->map[y][x].symbol), 1);
			// printf("%d",game->map[y][x].sprite[e_north].index);
			// printf("%d",game->map[y][x].sprite[e_east].index);
			// printf("%d",game->map[y][x].sprite[e_south].index);
			// printf("%d",game->map[y][x].sprite[e_west].index);
			// printf("%d",game->map[y][x].sprite[e_down].index);
			// printf("%d",game->map[y][x].sprite[e_up].index);
			// fflush(stdout);
			// write(1, "|", 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
}
