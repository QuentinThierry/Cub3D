/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:45:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 17:55:05 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"
#include "../../includes/get_next_line.h"

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

/**
 * @brief Get the dimension of the map in the file
 * 
 * @param fd fd to read the map and until the end of the file
 * @param line last line read in the file with gnl
 * @param error set on true if error occurs during the function NOT USE FOR
 * 		THE MOMENT
 * @return t_vector2 width and lenght of the map
 */
t_vector2	get_dimension_maps(int fd, char *line, bool *error)
{
	t_vector2	len;

	*error = false;
	len.y = 0;
	len.x = 0;
	while (line != NULL)
	{
		if (line[0] == '\n')
			break ;
		remove_end_whitespace(line);
		if (len.x < (int)ft_strlen(line))
			len.x = ft_strlen(line);
		len.y++;
		free(line);
		line = get_next_line(fd);
	}
	while (line != NULL)
	{
		if (line[0] != '\n')
			*error = true;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (len);
}

/**
 * @brief calloc the 2D map with the lenght of tha map
 */
t_map	**init_map(t_vector2 len)
{
	int		i;
	t_map	**res;

	i = 0;
	res = ft_calloc(len.y, sizeof(t_map *));
	if (res == NULL)
		return (print_error(NULL, 0), NULL);
	while (i < len.y)
	{
		res[i] = ft_calloc(len.x, sizeof(t_map));
		if (res[i] == NULL)
			return (print_error(NULL, 0), free_tab((void *)res, len.y), NULL);
		i++;
	}
	return (res);
}

/**
 * @brief Parse the second part of the file that contain the map with 1 or 0 ...
 * 
 * @param fd fd of th file
 * @param filename name of the file
 * @param game struct to add the map
 * @param nb_line number of line already read with get_next_line in the file open
 * @param line last line read with get_next_line
 * @return true On  Succes
 * @return false ERROR, error already print
 */
bool	parse_map(int fd, char *filename, t_game *game, int nb_line, char *line)
{
	int		y;
	int		i;
	t_map	**maps;
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
		return (false);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), free_map((void *)maps, game->map_size), false);
	line = get_next_line(fd);
	while (line != NULL && i < nb_line)
	{
		free(line);
		line = get_next_line(fd);
		i++;
	}
	while (line != NULL && y < game->map_size.y)
	{
		if (!ft_fill_wall(game, line, maps[y], game->map_size))
			return (free(line), free_map((void *)maps, game->map_size), false);
		line = get_next_line(fd);
		y++;
	}
	free(line);
	game->map = maps;
	if (!fill_object_and_doors(game))
		return (false);
	return (true);
}

/**
 * @brief Initialize the player with the info of the map. When the player is find
 *		he will be replace be an "0" on the map. Check is more than one player or none player
 * 		Calloc the player
 *
 * @param game 
 * @return true 
 * @return false 
 */
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
			if (game->map[index.y][index.x].symbol == 'N'
				|| game->map[index.y][index.x].symbol == 'S'
				|| game->map[index.y][index.x].symbol == 'W'
				|| game->map[index.y][index.x].symbol == 'E')
			{
				if (is_player)
					return (print_error("Too much players\n", 1), free(player), false);
				is_player = true;
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
	player->has_item = false;
	game->player = player;
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
		return (close(fd), print_error("Need a file not a directory\n", 1), false);
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
	{
		free(line);
		i++;
		line = get_next_line(fd);
	}
	if (line == NULL)
		return (close(fd), print_error("Empty map\n", 1), false);
	if (!parse_map(fd, filename, game, i, line))
		return (close(fd), false);
	close(fd);
	if (!find_player(game))
		return (false);
	return (true);
}
