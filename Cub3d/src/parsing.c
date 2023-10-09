/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:45:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 18:44:39 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../includes/get_next_line.h"

char	**init_map(t_vector2 len)
{
	int		i;
	char	**res;

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
		return (print_error("Empty map\n", 1), false);
	if (error == true)
		return (print_error("Unknown element in map\n", 1), false);
	maps = init_map(game->map_size);
	if (maps == NULL)
		return (print_error(NULL, 0), false);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), free_tab(maps, game->map_size.y), false);
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
		ft_memset(maps[y] + (ft_strlen(line)), ' ',
			game->map_size.x - ft_strlen(line));
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
	while (index.y < game->map_size.y)
	{
		index.x = 0;
		while (index.x < game->map_size.x)
		{
			if (game->map[index.y][index.x] == 'N'
				|| game->map[index.y][index.x] == 'S'
				|| game->map[index.y][index.x] == 'W'
				|| game->map[index.y][index.x] == 'E')
			{
				if (is_player)
					return (print_error("Too much players\n", 1), false);
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
	game->player = player;
	if (is_player == false)
		print_error("No player found on the map\n", 1);
	return (is_player);
}

int	find_next_wsp(char *line, int i)
{
	while (line[i] != '\0' && !(line[i] == ' ' || line[i] == '\t'
			|| line[i] == '\v' || line[i] == '\n' || line[i] == '\f'
			|| line[i] == '\r'))
		i++;
	return (i);
}

bool	check_filename(char *filename)
{
	int	len;
	int	fd;

	len = ft_strlen(filename);
	if (len < 4)
		return (print_error("Wrong name of file\n", 1), false);
	if (ft_strncmp(filename + (len - 4), ".cub", 4) != 0)
		return (print_error("Wrong name of file\n", 1), false);
	fd = open(filename, O_DIRECTORY);
	if (fd != -1)
		return (close(fd),
			print_error("Need a file not a directory\n", 1), false);
	return (true);
}

bool	parse_file(char *filename, t_game *game)
{
	int		fd;
	int		i;
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
