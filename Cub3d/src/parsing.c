/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:45:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 15:21:40 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	**init_map(t_vector2 len)
{
	int		i;
	char **res;
	
	i = 0;
	res = ft_calloc(len.y, sizeof(char *));
	if (res == NULL)
		return (NULL);
	while (i < len.y)
	{
		res[i] = ft_calloc(len.x, sizeof(char));
		if (res[i] == NULL)
			return (free_tab(res, len.y), NULL);
		i++;
	}
	return (res);
}

bool	parse_map(int fd, char *filename, t_game *game, int nb_line, char *line)
{
	int		y;
	int		i;
	char	**maps;
	bool	error;
	
	y = 0;
	i = 0;
	game->map_size = get_dimension_maps(fd, line, &error);
	if (game->map_size.x == 0 || game->map_size.y == 0)
		return (print_error("Error : Empty map\n", 1), false);
	if (error == true)
		return (print_error("Error : Unknown element in map\n", 1),false);
	maps = init_map(game->map_size);
	if (maps == NULL)
		return (print_error(NULL, 0), false);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), false);
	line = get_next_line(fd);
	while (line != NULL && i < nb_line)
	{
		free(line);
		line = get_next_line(fd);
		i++;
	}
	while (line != NULL && y < game->map_size.y)
	{
		remove_new_line(line);
		ft_memcpy(maps[y], line, ft_strlen(line));
		ft_memset(maps[y] + (ft_strlen(line)), ' ', game->map_size.x - ft_strlen(line));
		free(line);
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
		return (print_error(NULL, 0), false);
	while(index.y < game->map_size.y)
	{
		index.x = 0;
		while(index.x < game->map_size.x)
		{
			if (game->map[index.y][index.x] == 'N'
				|| game->map[index.y][index.x] == 'S'
				|| game->map[index.y][index.x] == 'W'
				|| game->map[index.y][index.x] == 'E')
			{
				if (is_player)
					return (print_error("Error : Too much players\n", 1), false);
				is_player = true;
				player->pos.x = index.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->pos.y = index.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->f_pos.x = index.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->f_pos.y = index.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
				player->f_real_pos.x = index.x + 1 / 2.0;
				player->f_real_pos.y = index.y + 1 / 2.0;
				if (game->map[index.y][index.x] == 'N')
					player->angle = 0;
				else if (game->map[index.y][index.x] == 'E')
					player->angle = 90;
				else if (game->map[index.y][index.x] == 'S')
					player->angle = 180;
				else if (game->map[index.y][index.x] == 'W')
					player->angle = 270;
				game->map[index.y][index.x] = '0';
			}
			index.x++;
		}
		index.y++;
	}
	player->speed = SPEED;
	game->player = player;
	return (is_player);
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
				return (print_error("Error : Color not between 0 and 255\n", 1), false);
			i++;
		}
		color = (color << 8) + tmp;
		nb_color++;
		i += skip_whitespace(str + i);
		if (str[i] == '\0' || nb_color == 3)
			break;
		if (str[i] != ',')
			return (print_error("Error : wrong format of color\n", 1), false);
		i += 1 + skip_whitespace(str + i + 1);
	}
	if (nb_color != 3 || str[i] != '\0')
		return (print_error("Error : Wrong format of color\n", 1), false);
	if (texture == 'F')
		game->floor_color = color;
	else
		game->ceiling_color = color;
	return (true);
}

bool	find_texture(t_game *game, char *str, enum e_images orient)
{
	char *filename;
	int	i;
	int	len;

	i = skip_whitespace(str);
	len = ft_strlen(str + i);
	if (len >= 1 && str[i + len - 1] == '\n')
		str[i + len - 1] = '\0';
	filename = ft_strdup(str + i);
	if (filename == NULL)
		return (print_error("Error : malloc failed\n", 1),false);
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

bool	cmp_texture(char *line, t_game *game, int i)
{
	int	next_wsp;

	next_wsp = find_next_wsp(line, i);
	if (next_wsp - i == 1)
	{
		if (line[i] == 'F')
			return (find_color(line + i + 1, game, 'F'));
		else if (line[i] == 'C')
			return (find_color(line + i + 1, game, 'C'));
	}
	else if (next_wsp - i == 2)
	{
		if (ft_strncmp(line + i, "NO", 2) == 0)
			return (find_texture(game, line + i + 2, e_north_wall));
		else if (ft_strncmp(line + i, "SO", 2) == 0)
			return (find_texture(game, line + i + 2, e_south_wall));
		else if (ft_strncmp(line + i, "WE", 2) == 0)
			return (find_texture(game, line + i + 2, e_west_wall));
		else if (ft_strncmp(line + i, "EA", 2) == 0)
			return (find_texture(game, line + i + 2, e_east_wall));
	}
	return (print_error("Error : invalid identifier\n", 1), false);
}

// ? attention missing mandatory texture
bool	parse_texture(int fd, t_game *game, int *nb_line, char **rest)
{
	char	*line;
	int		i;
	int		cpt_texture;
	
	cpt_texture = 0;
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
		if (ft_strlen(line + i) < 1)
			return (print_error("Error : Line to short\n", 1), free(line), false);
		if (!cmp_texture(line, game, i))
			return (free(line), false);
		cpt_texture++;
		free(line);
		line = get_next_line(fd);
		if (cpt_texture == 6)
			break ;
	}
	*rest = line;
	return (true);
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

bool	check_filename(char *filename)
{
	int len;
	int	fd;

	len = ft_strlen(filename);
	if (len < 4)
		return (print_error("Error : Wrong name of file\n", 1), false);
	if (ft_strncmp(filename + (len - 4 ), ".cub", 4) != 0)
		return (print_error("Error : Wrong name of file\n", 1), false);
	fd = open(filename, O_DIRECTORY);
	if (fd != -1)
		return (close(fd), print_error("Error : Need a file not a directory\n", 1), false);
	return (true);
}

bool	parse_file(char *filename, t_game *game)
{
	int fd;
	int i;
	char	*line;
	
	if (!check_filename(filename))
		return (false);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), false);
	if (!parse_texture(fd, game, &i, &line))
		return (close(fd), false);
	while (line != NULL && line[0] == '\n')
	{
		free(line);
		i++;
		line = get_next_line(fd);
	}
	if (line == NULL)
		return (close(fd), print_error("Empty map\n", 1), false);
	if (!parse_map(fd, filename, game, i, line))
		return (close(fd), false);
	close (fd);
	if (!find_player(game))
		return (false);
	return (true);
}
