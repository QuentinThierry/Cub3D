/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:45:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 20:28:36 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"
#include "../../includes/get_next_line.h"

bool	parse_map(char *filename, t_game *game, int nb_line);
bool	create_map(t_game *game, int fd, char *line);

void	remove_end_whitespace(char *str)
{
	int	i;

	i = ft_strlen(str) - 1;
	if (i <= 0)
		return ;
	while (i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\v'
			|| str[i] == '\n' || str[i] == '\f' || str[i] == '\r'))
		i--;
	if (i + 1 < ft_strlen(str))
		str[i + 1] = '\0';
	return ;
}

static bool	_set_player(t_game *game, t_player *player, t_vector2 index,
				bool *is_player)
{
	if (*is_player)
		return (print_error("Too much players\n", 1), false);
	*is_player = true;
	player->f_pos.x = index.x + 1 / 2.0;
	player->f_pos.y = index.y + 1 / 2.0;
	if (game->map[index.y][index.x].symbol == 'N')
		player->angle = 0;
	else if (game->map[index.y][index.x].symbol == 'E')
		player->angle = 90;
	else if (game->map[index.y][index.x].symbol == 'S')
		player->angle = 180;
	else if (game->map[index.y][index.x].symbol == 'W')
		player->angle = 270;
	game->map[index.y][index.x].symbol = '0';
		player->speed = SPEED;
	player->has_item = false;
	return (true);
}

/**
 * @brief Initialize the player with the info of the map. When the player is
 *		find he will be replace be an "0" on the map. Check is more than one
 *		player or none player 
 *		Calloc the player
 *
 * @param game 
 * @return true 
 * @return false 
 */
bool	find_player(t_game *game)
{
	t_vector2	index;
	bool		is_player;

	is_player = false;
	game->player = ft_calloc(1, sizeof(t_player));
	if (game->player == NULL)
		return (print_error(NULL, 0), false);
	index.y = -1;
	while (++index.y < game->map_size.y)
	{
		index.x = -1;
		while (++index.x < game->map_size.x)
		{
			if (game->map[index.y][index.x].symbol == 'N'
				|| game->map[index.y][index.x].symbol == 'S'
				|| game->map[index.y][index.x].symbol == 'W'
				|| game->map[index.y][index.x].symbol == 'E')
				if (!_set_player(game, game->player, index, &is_player))
					return (false);
		}
	}
	if (is_player == false)
		print_error("No player found on the map\n", 1);
	return (is_player);
}

/**
 * @brief Verify if the filename end with ".cub" and is not a directory
 * 
 * @param filename name of the file
 * @return true On Success
 * @return false ERROR, error already print
 */
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
		return (close(fd), print_error("Need a file not a directory\n", 1),
			false);
	return (true);
}

/**
 * @brief collect the info on the file to initialize the struct game
 * 
 * @param filename name of the file which contain the map
 * @param game struct to initialyse with the info in the file
 * @return true on Success
 * @return false ERROR during parsing, error already print
 */
bool	parse_file(char *filename, t_game *game)
{
	int		fd;
	int		i;
	char	*line;

	srand(time(0));
	if (!check_filename(filename))
		return (false);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), false);
	if (!parse_texture(fd, game, &i, &line))
		return (close(fd), false);
	while (line != NULL && line[0] == '\n')
		(free(line), i++, line = get_next_line(fd));
	if (line == NULL)
		return (close(fd), print_error("Empty map\n", 1), false);
	create_map(game, fd, line);
	if (game->map == NULL)
		return (false);
	if (!parse_map(filename, game, i))
		return (false);
	if (!find_player(game))
		return (false);
	return (true);
}
