/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:45:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/29 20:08:52 by jvigny           ###   ########.fr       */
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
	maps = ft_calloc(500, sizeof(char *));
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
		return (printf("Error : Wrong name of file\n"), false);
	if (strncmp(filename + (len - 4 ), ".cub", 4) != 0)
		return (printf("Error : Wrong name of file\n"), false);
	fd = open(filename, O_DIRECTORY);
	if (fd != -1)
		return (close(fd), printf("Error : Need a file not a directory\n"), false);
	return (true);
}

bool	find_color(char *str, t_game *game, char texture)
{
	int				i;
	int				nb_color;
	int				tmp;
	unsigned int	color;
	
	nb_color = 0;
	color = 0;
	i = skip_whitespace(str);
	while (str[i] != 0)
	{
		tmp = 0;
		while (str[i] >= '0' && str[i] <= '9')
		{
			tmp = tmp * 10 + str[i] - '0';
			if (tmp > 255)
				return (printf("Error : Color not between 0 and 255\n"), false);
			i++;
		}
		color = (color << 8) + tmp;
		nb_color++;
		i += skip_whitespace(str + i);
		if (str[i] == '\0' || nb_color == 3)
			break;
		if (str[i] != ',')
			return (printf("Error : wrong format of color\n"), false);
		i += 1 + skip_whitespace(str + i + 1);
	}
	if (nb_color != 3 || str[i] != '\0')
		return (printf("Error : Wrong format of color\n"), false);
	if (texture == 'F')
		game->floor = color;
	else
		game->ceiling = color;
	return (true);
}

bool	find_texture(t_game *game, char *str, enum e_orientation orient)
{
	char *filename;
	int	i;
	int	len;

	i = skip_whitespace(str);
	len = strlen(str + i);
	if (len >= 1 && str[i + len - 1] == '\n')
		str[i + len - 1] = '\0';
	filename = strdup(str + i);
	if (filename == NULL)
		return (printf("Error : malloc failed\n"),false);
	game->filename[orient] = filename;
	return (true);
}

bool	cmp_texture(char *line, t_game *game, int i, bool *is_matching)
{
	if (line[i] == 'F')
		return (*is_matching = true, find_color(line + i + 1, game, 'F'));
	else if (line[i] == 'C')
		return (*is_matching = true, find_color(line + i, game, 'C'));
	else if (strncmp(line + i, "NO", 2) == 0)
		return (*is_matching = true, find_texture(game, line + i + 2, e_north));
	else if (strncmp(line + i, "SO", 2) == 0)
		return (*is_matching = true, find_texture(game, line + i + 2, e_south));
	else if (strncmp(line + i, "WE", 2) == 0)
		return (*is_matching = true, find_texture(game, line + i + 2, e_west));
	else if (strncmp(line + i, "EA", 2) == 0)
		return (*is_matching = true, find_texture(game, line + i + 2, e_east));
	else
		return (false);
}

bool	parse_texture(int fd, t_game *game)
{
	char	*line;
	int		i;
	int		len;
	int		cpt_texture;
	bool	is_matching;
	
	cpt_texture = 0;
	line = get_next_line(fd);
	while (line != NULL && cpt_texture < 6)
	{
		if (line[0] == '\n')
		{
			free(line);
			continue ;
		}
		i = skip_whitespace(line);
		len = strlen(line + i);
		if (len < 1)
			return (printf("Error : Line to short\n"), free(line), false);
		is_matching = false;
		if (!cmp_texture(line, game, i, &is_matching) && is_matching == true)
			return (free(line), false);
		cpt_texture++;
		line = get_next_line(fd);
	}
	return (true);
}

int	parse_file(char *filename, t_game *game)
{
	int fd;
	
	if (!check_filename(filename))
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("Error"), -1);
	if (!parse_texture(fd, game))
		return (-1);
	return (0);
}
