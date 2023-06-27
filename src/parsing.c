/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:45:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/28 01:03:09 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	**parse_map(char *filename, t_vector2 *map_size)	//change parsing
{
	int	fd;
	int y;
	char		**maps = {0};
	
	fd = open(filename, O_RDONLY);
	y = 0;
	maps = ft_calloc(50, sizeof(char *));
	if (maps == NULL)
		return (NULL);
	maps[y] = get_next_line(fd);
	while (maps[y] != NULL)
	{
		maps[y][strlen(maps[y]) - 1] = 0;
		y++;
		maps[y] = get_next_line(fd);
	}
	map_size->x = strlen(maps[y - 1]);
	map_size->y = y;
	// printf("size maps x:%d y : %d\n", map_size->x, map_size->y);
	close(fd);
	return (maps);
}

t_player	*find_player(char **maps)
{
	t_player	*player;
	t_vector2	index;

	index.y = 0;
	index.x = 0;
	player = ft_calloc(1, sizeof(t_player));
	if (player == NULL)
		return (NULL);
	while(maps[index.y] != NULL)
	{
		index.x = 0;
		while(maps[index.y][index.x] != '\0')
		{	
			if (maps[index.y][index.x] == 'N')
			{
				player->angle = 0;
				player->pos.x = index.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->pos.y = index.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->f_pos.x = index.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->f_pos.y = index.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->f_real_pos.x = index.x + 1 / 2.0;
				player->f_real_pos.y = index.y + 1 / 2.0;
			}
			index.x++;
		}
		index.y++;
	}
	return (player);
}

bool	check_filename(char *filename)
{
	int len;
	int	fd;

	len = strlen(filename);
	if (len < 4)
		return (false);
	if (strncmp(filename + (len - 4 ), ".cub", 4) != 0)
		return (false);
	fd = open(filename, O_DIRECTORY);
	if (fd != -1)
		return (close(fd), false);
	return (true);
}

bool	parse_texture(int fd, t_game *game)
{
	char *line;
	
	line = get_next_line(fd);
	while (line != NULL)
	{
		// if (line[0] == '\n')
		
		line = get_next_line(fd);
	}
}

int	parse_file(char *filename, t_game *game)
{
	int fd;
	
	if (!check_filename(filename))
		return (perror("Error"), -1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("Error"), -1);
	parse_texture(fd, game);
	return (0);
}
