/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:45:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 19:40:33 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../includes/get_next_line.h"

static void	init_player(char **map, t_player *player, bool *is_player,
		t_vector2 pos)
{
	*is_player = true;
	player->pos.x = pos.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
	player->pos.y = pos.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
	player->f_pos.x = pos.x * CHUNK_SIZE + CHUNK_SIZE / 2.0;
	player->f_pos.y = pos.y * CHUNK_SIZE + CHUNK_SIZE / 2.0;
	player->f_real_pos.x = pos.x + 1 / 2.0;
	player->f_real_pos.y = pos.y + 1 / 2.0;
	if (map[pos.y][pos.x] == 'N')
		player->angle = 0;
	else if (map[pos.y][pos.x] == 'E')
		player->angle = 90;
	else if (map[pos.y][pos.x] == 'S')
		player->angle = 180;
	else if (map[pos.y][pos.x] == 'W')
		player->angle = 270;
	map[pos.y][pos.x] = '0';
	player->speed = SPEED;
}

static bool	is_player(char symbol)
{
	return (symbol == 'N' || symbol == 'S' || symbol == 'W' || symbol == 'E');
}

bool	find_player(t_game *game)
{
	t_vector2	index;
	bool		has_player;

	has_player = false;
	game->player = ft_calloc(1, sizeof(t_player));
	if (game->player == NULL)
		return (print_error(NULL, 0), false);
	index.y = -1;
	while (++(index.y) < game->map_size.y)
	{
		index.x = -1;
		while (++(index.x) < game->map_size.x)
		{
			if (is_player(game->map[index.y][index.x]))
			{
				if (has_player)
					return (print_error("Too much players\n", 1), false);
				init_player(game->map, game->player, &has_player, index);
			}
		}
	}
	if (has_player == false)
		print_error("No player found on the map\n", 1);
	return (has_player);
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
		(free(line), i++);
		line = get_next_line(fd);
	}
	if (line == NULL)
		return (close(fd), print_error("Empty map\n", 1), false);
	if (!create_map(game, fd, line))
		return (false);
	if (!parse_map(filename, game, i))
		return (false);
	if (!find_player(game))
		return (false);
	return (true);
}
