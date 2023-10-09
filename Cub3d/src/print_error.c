/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:19:43 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 17:46:07 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	print_error(char *error, int print)
{
	if (print == 1)
		printf("Error\n%s", error);
	else
	{
		printf("Error\n");
		perror("");
	}
}

void	printf_texture(t_game *game)
{
	printf("no : %s\n", game->filename[e_north_wall]);
	printf("su : %s\n", game->filename[e_south_wall]);
	printf("ea : %s\n", game->filename[e_east_wall]);
	printf("we : %s\n", game->filename[e_west_wall]);
	printf("floor : %x\n", game->floor_color);
	printf("ceiling : %x\n", game->ceiling_color);
}
