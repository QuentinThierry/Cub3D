/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:45:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/15 18:38:01 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

t_wall	**init_map(t_vector2 len)
{
	int		i;
	t_wall **res;
	
	i = 0;
	res = ft_calloc(len.y, sizeof(t_wall *));
	if (res == NULL)
		return (NULL);
	while (i < len.y)
	{
		res[i] = ft_calloc(len.x, sizeof(t_wall));
		if (res[i] == NULL)
			return (free_tab((void *)res, len), NULL);
		i++;
	}
	return (res);
}

bool	parse_map(int fd, char *filename, t_game *game, int nb_line, char *line)
{
	int		y;
	int		i;
	t_wall	**maps;
	bool	error;
	
	y = 0;
	i = 0;
	game->map_size = get_dimension_maps(fd, line, &error);
	if (game->map_size.x == 0 || game->map_size.y == 0)
		return (printf("Error : Empty map\n"), false);
	if (error == true)
		return (printf("Error : Unknown element in map\n"),false);
	maps = init_map(game->map_size);
	if (maps == NULL)
		return (perror("Error"), false);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("Error"), false);
	line = get_next_line(fd);
	while (line != NULL && i < nb_line)
	{
		free(line);
		line = get_next_line(fd);
		i++;
	}
	while (line != NULL && y < game->map_size.y)
	{
		ft_fill_wall(line, maps[y], game->map_size);
		// remove_new_line(line);
		// ft_memcpy(maps[y], line, strlen(line));
		// memset(maps[y] + (strlen(line)), ' ', game->map_size.x - strlen(line));
		// free(line);
		line = get_next_line(fd);
		y++;
	}
	free(line);
	game->map = maps;
	return (true);
}

bool	find_player(t_game *game)
{
	t_player	*player;
	t_vector2	index;
	bool		is_player;

	index.y = 0;
	index.x = 0;
	is_player = false;
	player = ft_calloc(1, sizeof(t_player));
	if (player == NULL)
		return (perror("Error"), false);
	while(index.y < game->map_size.y)
	{
		index.x = 0;
		while(index.x < game->map_size.x)
		{
			if (game->map[index.y][index.x].symbol == 'N'
				|| game->map[index.y][index.x].symbol == 'S'
				|| game->map[index.y][index.x].symbol == 'W'
				|| game->map[index.y][index.x].symbol == 'E')
			{
				if (is_player)
					return (printf("Error : Too much players\n"), false);
				is_player = true;
				player->pos.x = index.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->pos.y = index.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->f_pos.x = index.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->f_pos.y = index.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->f_real_pos.x = index.x + 1 / 2.0;
				player->f_real_pos.y = index.y + 1 / 2.0;
				if (game->map[index.y][index.x].symbol == 'N')
					player->angle = 0;
				else if (game->map[index.y][index.x].symbol == 'E')
					player->angle = 90;
				else if (game->map[index.y][index.x].symbol == 'S')
					player->angle = 180;
				else if (game->map[index.y][index.x].symbol == 'W')
					player->angle = 270;
				game->map[index.y][index.x].symbol = '0';
			}
			index.x++;
		}
		index.y++;
	}
	player->speed = SPEED;
	game->player = player;
	return (is_player);
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

bool	find_texture(t_game *game, char *str, enum e_texture orient)
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

int	find_next_wsp(char *line , int i)
{
	while (line[i] != '\0' && !(line[i] == ' ' || line[i] == '\t'
			|| line[i] == '\v' || line[i] == '\n' || line[i] == '\f'
			|| line[i] == '\r'))
		i++;
	return (i);
}

bool	cmp_texture(char *line, t_game *game, int i, bool *is_end)
{
	int	next_wsp;

	next_wsp = find_next_wsp(line, i);
	if (next_wsp - i == 1)
	{
		if (line[i] == 'F')
			return (find_color(line + i + 1, game, 'F'));
		else if (line[i] == 'C')
			return (find_color(line + i + 1, game, 'C'));
		else if (line[i] == 'c')
			return (find_texture(game, line + i + 1, e_door_close));
		else if (line[i] == 'o')
			return (find_texture(game, line + i + 1, e_door_open));
	}
	else if (next_wsp - i == 2)
	{
		if (strncmp(line + i, "NO", 2) == 0)
			return (find_texture(game, line + i + 2, e_north_wall));
		else if (strncmp(line + i, "SO", 2) == 0)
			return (find_texture(game, line + i + 2, e_south_wall));
		else if (strncmp(line + i, "WE", 2) == 0)
			return (find_texture(game, line + i + 2, e_west_wall));
		else if (strncmp(line + i, "EA", 2) == 0)
			return (find_texture(game, line + i + 2, e_east_wall));
	}
	else if (next_wsp - i == 3)
		if (strncmp(line + i, "MAP\n", 4) == 0 || strncmp(line + i, "MAP\0", 4) == 0)
			return (*is_end = true, true);
	return (printf("Error : invalid identifier\n"), false);
}

bool	parse_texture(int fd, t_game *game, int *nb_line, char **rest)
{
	char	*line;
	int		i;
	bool	is_end;

	is_end = false;
	(*nb_line) = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		(*nb_line)++;
		if (line[0] == '\n')
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		i = skip_whitespace(line);
		if (strlen(line + i) < 1)
			return (printf("Error : Line to short\n"), free(line), false);
		if (!cmp_texture(line, game, i, &is_end))
			return (free(line), false);
		free(line);
		line = get_next_line(fd);
		if (is_end == true)
			break ;
	}
	*rest = line;
	return (true);
}

void	printf_texture(t_game *game)
{
	printf("no : %s\n", game->filename[e_north]);
	printf("su : %s\n", game->filename[e_south]);
	printf("ea : %s\n", game->filename[e_east]);
	printf("we : %s\n", game->filename[e_west]);
	printf("floor : %x\n", game->floor);
	printf("ceiling : %x\n", game->ceiling);
}

int	parse_file(char *filename, t_game *game)
{
	int fd;
	int i;
	char	*line;
	
	if (!check_filename(filename))
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("Error"), -1);
	if (!parse_texture(fd, game, &i, &line))
		return (-1);
	while (line != NULL && line[0] == '\n')
	{
		free(line);
		i++;
		line = get_next_line(fd);
	}
	if (line == NULL)
		return (printf("Error : Empty map\n"), -1);
	if (!parse_map(fd, filename, game, i, line))
		return (-1);
	close (fd);
	if (!find_player(game))
		return (-1);
	return (0);
}
