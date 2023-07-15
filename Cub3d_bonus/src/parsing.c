/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:45:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/16 00:58:28 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

/**
 * @brief calloc the 2D map with the lenght of tha map
 */
t_map	**init_map(t_vector2 len)
{
	int		i;
	t_map **res;
	
	i = 0;
	res = ft_calloc(len.y, sizeof(t_map *));
	if (res == NULL)
		return (perror("Error"), NULL);
	while (i < len.y)
	{
		res[i] = ft_calloc(len.x, sizeof(t_map));
		if (res[i] == NULL)
			return (perror("Error"), free_tab((void *)res, len), NULL);
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
		return (printf("Error : Empty map\n"), false);
	if (error == true)
		return (printf("Error : Unknown element in map\n"), false);
	maps = init_map(game->map_size);
	if (maps == NULL)
		return (false);
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
		if (!ft_fill_wall(game, line, maps[y], game->map_size))
			return (free(line), false);
		line = get_next_line(fd);
		y++;
	}
	free(line);
	game->map = maps;
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
	if (is_player == false)
		printf("Error : No player found on the map\n");
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

// bool	find_color(char *str, t_game *game, char texture)
// {
// 	int				i;
// 	int				nb_color;
// 	int				tmp;
// 	unsigned int	color;
	
// 	nb_color = 0;
// 	color = 0;
// 	i = skip_whitespace(str);
// 	while (str[i] != 0)
// 	{
// 		tmp = 0;
// 		while (str[i] >= '0' && str[i] <= '9')
// 		{
// 			tmp = tmp * 10 + str[i] - '0';
// 			if (tmp > 255)
// 				return (printf("Error : Color not between 0 and 255\n"), false);
// 			i++;
// 		}
// 		color = (color << 8) + tmp;
// 		nb_color++;
// 		i += skip_whitespace(str + i);
// 		if (str[i] == '\0' || nb_color == 3)
// 			break;
// 		if (str[i] != ',')
// 			return (printf("Error : wrong format of color\n"), false);
// 		i += 1 + skip_whitespace(str + i + 1);
// 	}
// 	if (nb_color != 3 || str[i] != '\0')
// 		return (printf("Error : Wrong format of color\n"), false);
// 	if (texture == 'F')
// 		game->floor = color;
// 	else
// 		game->ceiling = color;
// 	return (true);
// }

/**
 * @brief complete the tab of filename with the new texture name, its orientation,
 *		its symbol on the map
 * 		malloc the tab and the filename
 * @param game struct to add the texture
 * @param str name of the texture to add
 * @param index offset on the tab to add the texture
 * @param orient orientation of the texture
 * @return true On success
 * @return false ERROR, error already print
 */
bool	find_texture(t_game *game, char *str, int index, enum e_orientation orient)
{
	char *filename;
	int	i;
	int	len;

	if (index >= game->nb_sprite)
	{
		game->filename = ft_realloc(game->filename
			, sizeof(t_texture) * game->nb_sprite, sizeof(t_texture) * (index + 1));
		if (game->filename == NULL)
			return (perror("Error"), false);
		game->nb_sprite = index + 1;
	}
	i = skip_whitespace(str);
	if (str[i] == '\0')
		return (printf("Error : Empty texture\n"), false);
	len = strlen(str + i);
	if (len >= 1 && str[i + len - 1] == '\n')
		str[i + len - 1] = '\0';
	filename = strdup(str + i);
	if (filename == NULL)
		return (printf("Error : malloc failed\n"),false);
	game->filename[index].filename = filename;
	game->filename[index].orient = orient;
	if (index >= e_door_close)
		game->filename[index].symbol = *(str - 1);
	else if (index == e_floor || index == e_ceiling)
		game->filename[index].symbol = '0';
	else
		game->filename[index].symbol = '1';
	return (true);
}

/**
 * @brief return the index of the next whitespace or the '\0' if none has been find
 */
int	find_next_wsp(char *line , int i)
{
	while (line[i] != '\0' && !(line[i] == ' ' || line[i] == '\t'
			|| line[i] == '\v' || line[i] == '\n' || line[i] == '\f'
			|| line[i] == '\r'))
		i++;
	return (i);
}

/**
 * @brief compare the identifier of the texture to assign at the right place
 * 
 * @param line string with the identifier + texture separate by a space
 * @param game struct to add the texture
 * @param i index to the begin of the indentifier
 * @param is_end if the world "MAP" is find is_end is set at true
 * @return true On success
 * @return false ERROR, error already print
 */
bool	cmp_texture(char *line, t_game *game, int i, bool *is_end)
{
	int	next_wsp;

	next_wsp = find_next_wsp(line, i);
	if (next_wsp - i == 1)
	{
		if (line[i] == 'F')
			return (find_texture(game, line + i + 1, e_floor, e_down));
		else if (line[i] == 'C')
			return (find_texture(game, line + i + 1, e_ceiling, e_up));
		else if (line[i] == 'c')
			return (find_texture(game, line + i + 1, e_door_close, e_wall));
		else if (line[i] == 'o')
			return (find_texture(game, line + i + 1, e_door_open, e_wall));
		else
		 	return (find_texture(game, line + i + 1, game->nb_sprite, e_wall));
	}
	else if (next_wsp - i == 2)
	{
		if (strncmp(line + i, "NO", 2) == 0)
			return (find_texture(game, line + i + 2, e_north_wall, e_north));
		else if (strncmp(line + i, "SO", 2) == 0)
			return (find_texture(game, line + i + 2, e_south_wall, e_south));
		else if (strncmp(line + i, "WE", 2) == 0)
			return (find_texture(game, line + i + 2, e_west_wall, e_west));
		else if (strncmp(line + i, "EA", 2) == 0)
			return (find_texture(game, line + i + 2, e_east_wall, e_east));
	}
	else if (next_wsp - i == 3)
	{
		if (strncmp(line + i, "MAP\n", 4) == 0 || strncmp(line + i, "MAP\0", 4) == 0)
			return (*is_end = true, true);
		if (strncmp(line + i, "N_", 2) == 0)
			return (find_texture(game, line + i + 3, game->nb_sprite, e_north));
		else if (strncmp(line + i, "S_", 2) == 0)
			return (find_texture(game, line + i + 3, game->nb_sprite, e_south));
		else if (strncmp(line + i, "W_", 2) == 0)
			return (find_texture(game, line + i + 3, game->nb_sprite, e_west));
		else if (strncmp(line + i, "E_", 2) == 0)
			return (find_texture(game, line + i + 3, game->nb_sprite, e_east));
		else if (strncmp(line + i, "F_", 2) == 0)
			return (find_texture(game, line + i + 3, game->nb_sprite, e_down));
		else if (strncmp(line + i, "C_", 2) == 0)
			return (find_texture(game, line + i + 3, game->nb_sprite, e_up));
	}
	return (printf("Error : invalid identifier\n"), false);
}

/**
 * @brief Parse the first part of the file that contain the name of the texture
 * 
 * @param fd of the file, not -1
 * @param game struct to add the texture
 * @param nb_line pointer to the number of lines, that will be read
 * @param rest pointer to the last string, that will be read
 * @return true On sucess
 * @return false ERROR, error already print
 */
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
	int i = 0;
	while (i < game->nb_sprite)
	{
		printf("texture %s	char : %c	orientation : %d	index : %d\n"
				, game->filename[i].filename, game->filename[i].symbol, game->filename[i].orient, i);
		i++;
	}
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
	int fd;
	int i;
	char	*line;
	
	if (!check_filename(filename))
		return (false);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("Error"), false);
	if (!parse_texture(fd, game, &i, &line))
		return (close(fd), false);
	while (line != NULL && line[0] == '\n')
	{
		free(line);
		i++;
		line = get_next_line(fd);
	}
	if (line == NULL)
		return (close(fd), printf("Error : Empty map\n"), false);
	if (!parse_map(fd, filename, game, i, line))
		return (close(fd), false);
	close(fd);
	if (!find_player(game))
		return (false);
	return (true);
}
