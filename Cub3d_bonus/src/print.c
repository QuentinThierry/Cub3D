/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 01:29:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/09/13 16:27:34 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	printf_texture(t_game *game)
{
	int i = 0;
	int j;
	int x;
	while (i < game->nb_file)
	{
		if (game->filename[i].filename != NULL)
			printf("texture %s	char : %c	%c	orientation : %d	index : %d		total : %d\n"
				, game->filename[i].filename, game->filename[i].symbol, game->filename[i].symbol_receptacle
				, game->filename[i].orient, i, game->filename[i].total);
		else
		{
			j = 0;
			printf("char : %c	orientation : %d	index : %d	total : %d\n"
				, game->filename[i].symbol
				, game->filename[i].orient, i, game->filename[i].total);
			while (j < game->filename[i].nb_file)
			{
				printf("	dir :texture %s\n", game->filename[i].filename_d[j]);
				j++;
			}
			j = 0;
			while (j < game->filename[i].nb_animation)
			{
				x = 0;
				while (x < game->filename[i].animation[j].nb_sprite)
				{
					printf("	anim %d :texture %s\n",j, game->filename[i].animation[j].filename[x]);
					x++;
				}
				j++;
			}
		}
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
