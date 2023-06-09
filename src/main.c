/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/09 19:47:19 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

typedef	struct s_player{
	int	x;
	int	y;
	float	angle;
	float	f_x;
	float	f_y;
}	t_player;

t_player	find_player(char **maps)
{
	t_player	player;
	t_vector2	index;

	index.y = 0;
	index.x = 0;
	while(maps[index.y] != NULL)
	{
		index.x = 0;
		while(maps[index.y][index.x] != '\0')
		{	
			if (maps[index.y][index.x] == 'N')
			{
				player.angle = 0;
				player.x = index.x;
				player.y = index.y;
			}
			index.x++;
		}
		index.y++;
	}
	return (player);
}

int main(int argc, char **argv)
{
	char	*buff;
	char	*maps[12];
	t_player	player;
	int y,x;
	int p_y,p_x;
	
	int fd;
	
	fd = open("maps/test.cub", O_RDONLY);
	y = 0;
	maps[y] = get_next_line(fd);
	while (maps[y] != NULL)
	{
		y++;
		maps[y] = get_next_line(fd);
	}
	close(fd);
	y = 0;
	player = find_player(maps);
	
	printf("x : %d Y:%d\n", player.x, player.y);
	while(maps[y] != NULL)
	{
		printf("%s\n", maps[y]);
		y++;
	}
	
	return (0);
}
