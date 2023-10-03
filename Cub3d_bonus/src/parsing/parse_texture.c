/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 01:50:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/03 16:37:15 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

/**
 * @brief read the config file and stock the usefull information
 * 
 * @param animation 
 * @param index 
 * @return true 
 * @return false 
 */
bool	ft_read_config(t_animation *animation, int index)
{
	int		fd;
	char	*buffer;
	bool	error;
	
	error = false;
	fd = open(animation->filename[index], O_RDONLY);
	if (fd == -1)
		return (print_error(NULL, 0), false);
	buffer = get_next_line(fd);
	if (buffer != NULL)
		animation->time_sprite = ft_atoi(buffer);
	free(buffer);
	if (animation->time_sprite <= 0)
		error = true;
	buffer = get_next_line(fd);
	if (buffer != NULL)
		animation->time_animation = ft_atoi(buffer);
	free(buffer);
	if (animation->time_animation <= 0)
		error = true;
	buffer = get_next_line(fd);
	while (buffer != NULL)
	{
		if (!(buffer[0] == '\n' || buffer[0] == '\0'))
			error = true;
		free(buffer);
		buffer = get_next_line(fd);
	}
	close(fd);
	if (error == true)
		print_error("Wrong format of config file\n", 1);
	return (!error);
}

void	swap(char **str, int a, int b)
{
	char *tmp;

	tmp = str[a];
	str[a] = str[b];
	str[b] = tmp;
}

/**
 * @brief sort the filenstrncmpames of animation by alphbetic order and return if
 *		"config.cfg" is present. "config.cfg" will have the first position
 * 
 * @param anim 
 * @return true 
 * @return false 
 */
bool	sort_animation(t_animation *anim)
{
	int		i;
	int		j;
	bool	has_config;

	i = 0;
	has_config = false;
	while (i < anim->nb_sprite)
	{
		if (ft_strlen(anim->filename[i]) >= 10 && ft_strcmp(anim->filename[i]
			+ (ft_strlen(anim->filename[i]) - 10) , "config.cfg") == 0)
		{
			swap(anim->filename, i, 0);
			has_config = true;
			break ;
		}
		i++;
	}
	i = 1;
	while (i + 1 < anim->nb_sprite)
	{
		j = 1;
		while (j + 1 < anim->nb_sprite)
		{
			if (ft_strcmp(anim->filename[j], anim->filename[j + 1]) > 0)
				swap(anim->filename, j, j + 1);
			j++;
		}
		i++;
	}
	return (has_config);
}

/**
 * @brief read directory to parse the animation find in the file.
 *		Skip hidden files and directories
 * 		Malloc tab of animation and animation's filename
 *
 * @param dir 
 * @param texture 
 * @param dirname 
 * @return true 
 * @return false 
 */
bool	ft_read_anim(DIR *dir, t_texture *texture, char *dirname)
{
	struct dirent	*buffer;
	char			*filename;
	bool			add_slash;
	bool			has_config;

	texture->animation = ft_realloc(texture->animation
		, sizeof(t_animation) * (texture->nb_animation)
		,sizeof(t_animation) * (texture->nb_animation + 1));
	if (texture->animation == NULL)
			return (print_error(NULL, 0), closedir(dir), false);
	if (ft_strlen(dirname) > 0 && dirname[ft_strlen(dirname) - 1] == '/')
		add_slash = false;
	else
		add_slash = true;
	buffer = readdir(dir);
	while (buffer != NULL)
	{
		if (buffer->d_name[0] == '.')
		{
			buffer = readdir(dir);
			continue ;
		}
		filename = ft_strjoin_slash(dirname, buffer->d_name, add_slash);
		if (filename == NULL)
			return (print_error(NULL, 0), closedir(dir), false);
		texture->animation[texture->nb_animation].filename = ft_realloc(
			texture->animation[texture->nb_animation].filename
			, sizeof(char *) * (texture->animation[texture->nb_animation].nb_sprite)
			, sizeof(char *) * (texture->animation[texture->nb_animation].nb_sprite + 1));
		if (texture->animation[texture->nb_animation].filename == NULL)
			return (print_error(NULL, 0), closedir(dir), false);
		texture->total++;
		texture->animation[texture->nb_animation].filename[texture->animation[texture->nb_animation].nb_sprite] = filename;
		texture->animation[texture->nb_animation].nb_sprite++;
		buffer = readdir(dir);
	}
	texture->total--;
	free(dirname);
	dirname = NULL;
	closedir(dir);
	has_config = sort_animation(&(texture->animation[texture->nb_animation]));
	if (has_config == false)
		print_error("Missing the file config.cfg for the animations\n", 1);
	texture->nb_animation++;
	return (has_config);
}

/**
 * @brief read directory to add each filename find or call an other function for
 *		animation's directory. Skip hidden files and directories
 * 		Malloc filname_d's texture
 *
 * @param dir 
 * @param texture 
 * @return true 
 * @return false 
 */
bool	ft_read_dir(DIR *dir, t_texture *texture)
{
	struct dirent	*buffer;
	DIR				*tmp;
	char			*filename;
	bool			add_slash;

	texture->nb_file = 0;
	if (ft_strlen(texture->filename) > 0 && texture->filename[ft_strlen(texture->filename) - 1] == '/')
		add_slash = false;
	else
		add_slash = true;
	buffer = readdir(dir);
	while (buffer != NULL)
	{
		if (buffer->d_name[0] == '.')
		{
			buffer = readdir(dir);
			continue ;
		}
		filename = ft_strjoin_slash(texture->filename, buffer->d_name, add_slash);
		if (filename == NULL)
			return (print_error(NULL, 0), closedir(dir), false);
		tmp = opendir(filename);
		if (tmp != NULL)
		{
			if (!ft_read_anim(tmp, texture, filename))
				return (closedir(dir), false);
		}
		else
		{
			texture->total++;
			texture->filename_d = ft_realloc(texture->filename_d
				, sizeof(char *) * (texture->nb_file) , sizeof(char *) * (texture->nb_file + 1));
			if (texture->filename == NULL)
				return (print_error(NULL, 0), closedir(dir), false);
			texture->filename_d[texture->nb_file] = filename;
			texture->nb_file++;
		}
		buffer = readdir(dir);
	}
	free(texture->filename);
	texture->filename = NULL;
	closedir(dir);
	return (true);
}

static bool	_is_existing(t_game *game, int index, char symbol, enum e_orientation orient)
{
	int		i;
	bool	is_wall;

	i = 0;
	if (index == e_floor || index == e_ceiling)
		symbol = '0';
	else if (index == e_north || index == e_east || index == e_south || index == e_west)
		symbol = '1';
	else if (symbol == '1' || symbol == '0')
		return (true);
	if (orient == e_north || orient == e_east || orient == e_south
		|| orient == e_west || orient == e_wall)
		is_wall = true;
	else
	 	is_wall = false;
	while (i < game->nb_file)
	{
		if (orient == e_exit && game->filename[i].orient == e_exit)
			return (true);
		if (game->filename[i].symbol == symbol)
		{
			if (game->filename[i].orient == orient)
				return (true);
			else if (is_wall == true)
			{
				if (game->filename[i].orient == e_floor || game->filename[i].orient == e_ceiling 
					|| game->filename[i].orient == e_door || game->filename[i].orient == e_object_wall
					|| game->filename[i].orient == e_object_entity || game->filename[i].orient == e_door_lock 
					|| game->filename[i].orient == e_object_interactive || game->filename[i].orient == e_receptacle_empty
					|| game->filename[i].orient == e_exit)
					return (true);
					
			}
			else if ((orient == e_object_entity || orient == e_object_wall
				|| orient == e_object_interactive || orient == e_door || orient == e_door_lock
				|| orient == e_exit || orient == e_receptacle_empty)
				&& (game->filename[i].orient == e_object_entity || game->filename[i].orient == e_object_wall
				|| game->filename[i].orient == e_object_interactive || game->filename[i].orient == e_door 
				|| game->filename[i].orient == e_exit || game->filename[i].orient == e_receptacle_empty
				|| game->filename[i].orient == e_door_lock))
				return (true);
			else if ((is_wall == false) 
				&& (game->filename[i].orient == e_north || game->filename[i].orient == e_east 
				|| game->filename[i].orient == e_south || game->filename[i].orient == e_west 
				|| game->filename[i].orient == e_wall))
				return (true);
		}
		i++;
	}
	return (false);
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

bool	multiple_texture(t_game *game, int *index, char * str, enum e_orientation orient)
{
	
	int		cpt;
	int		nb_file;
	int		i;
	int		len;
	DIR		*dir;
	char	*filename;

	i = 0;
	cpt = 0;
	if (orient == e_receptacle_empty || orient == e_door_lock)
		nb_file = 2;
	else
		nb_file = 4;
	while (cpt < nb_file)
	{
		i += skip_whitespace(str + i);
		if (str[i] == '\0')
		{
			if (nb_file == 4 && cpt == 2)
				break ;
			return (print_error("Empty texture\n", 1), false);
		}
		len = find_next_wsp(str + i, 0);
		if (len >= 0 && (str[i + len] == ' ' || str[i + len] == '\t'
			|| str[i + len] == '\v' || str[i + len] == '\n' || str[i + len] == '\f'
			|| str[i + len] == '\r'))
			str[i + len] = '\0';
		filename = ft_strdup(str + i);
		i += len + 1;
		if (filename == NULL)
			return (print_error("malloc failed\n", 1), false);
		if (*index >= game->nb_file)
		{
			game->filename = ft_realloc(game->filename
				, sizeof(t_texture) * game->nb_file, sizeof(t_texture) * (*index + 1));
			if (game->filename == NULL)
				return (print_error(NULL, 0), false);
			game->nb_file = *index + 1;
		}
		if (orient == e_receptacle_empty)
		{
			game->filename[*index].orient = e_receptacle_empty + cpt;
			game->filename[*index].symbol_receptacle = *(str - 3);
		}
		else if (orient == e_door_lock)
		{
			game->filename[*index].orient = e_door_lock + cpt;
			game->filename[*index].symbol_receptacle = *(str - 3);
		}
		else
			game->filename[*index].orient = e_object_interactive + cpt;
		game->filename[*index].nb_file = 1;
		game->filename[*index].symbol = *(str - 1);
		game->filename[*index].filename = filename;
		dir = opendir(filename);
		if (dir != NULL)
			return (ft_read_dir(dir, &game->filename[*index]));
		game->filename[*index].total++;
		(*index)++;
		cpt++;
	}
	return (true);
}

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
static bool	_find_texture(t_game *game, char *str, int index, enum e_orientation orient)
{
	DIR		*dir;
	char	*filename;
	int		i;
	int		len;

	if (_is_existing(game, index, *(str - 1), orient))
		return (print_error("Multiples definition of a texture\n", 1), false);
	if (orient == e_receptacle_empty || orient == e_door_lock || orient == e_object_interactive)
		return (multiple_texture(game, &index, str, orient));
	if (index >= game->nb_file)
	{
		game->filename = ft_realloc(game->filename
			, sizeof(t_texture) * game->nb_file, sizeof(t_texture) * (index + 1));
		if (game->filename == NULL)
			return (print_error(NULL, 0), false);
		game->nb_file = index + 1;
	}
	game->filename[index].orient = orient;
	game->filename[index].nb_file = 1;
	if (index == e_north || index == e_east || index == e_south || index == e_west)
		game->filename[index].symbol = '1';
	else if (index == e_floor || index == e_ceiling)
		game->filename[index].symbol = '0';
	else
		game->filename[index].symbol = *(str - 1);
	i = skip_whitespace(str);
	if (str[i] == '\0')
		return (print_error("Empty texture\n", 1), false);
	len = ft_strlen(str + i);
	if (len >= 1 && str[i + len - 1] == '\n')
		str[i + len - 1] = '\0';
	filename = ft_strdup(str + i);
	if (filename == NULL)
		return (print_error("malloc failed\n", 1), false);
	game->filename[index].filename = filename;
	dir = opendir(filename);
	if (dir != NULL)
		return (ft_read_dir(dir, &(game->filename[index])));
	game->filename[index].total++;
	return (true);
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
static bool	_cmp_texture(char *line, t_game *game, int i, bool *is_end)
{
	int	next_wsp;

	next_wsp = find_next_wsp(line, i);
	if (next_wsp - i == 1)
	{
		if (line[i] == 'F')
			return (_find_texture(game, line + i + 1, e_floor, e_floor));
		else if (line[i] == 'C')
			return (_find_texture(game, line + i + 1, e_ceiling, e_ceiling));
		else
		 	return (_find_texture(game, line + i + 1, game->nb_file, e_wall));
	}
	else if (next_wsp - i == 2)
	{
		if (ft_strncmp(line + i, "NO", 2) == 0)
			return (_find_texture(game, line + i + 2, e_north, e_north));
		else if (ft_strncmp(line + i, "SO", 2) == 0)
			return (_find_texture(game, line + i + 2, e_south, e_south));
		else if (ft_strncmp(line + i, "WE", 2) == 0)
			return (_find_texture(game, line + i + 2, e_west, e_west));
		else if (ft_strncmp(line + i, "EA", 2) == 0)
			return (_find_texture(game, line + i + 2, e_east, e_east));
	}
	else if (next_wsp - i == 3)
	{
		if (ft_strncmp(line + i, "MAP\n", 4) == 0 || ft_strncmp(line + i, "MAP\0", 4) == 0)
			return (*is_end = true, true);
		if (ft_strncmp(line + i, "N_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_file, e_north));
		else if (ft_strncmp(line + i, "S_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_file, e_south));
		else if (ft_strncmp(line + i, "W_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_file, e_west));
		else if (ft_strncmp(line + i, "E_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_file, e_east));
		else if (ft_strncmp(line + i, "F_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_file, e_floor));
		else if (ft_strncmp(line + i, "C_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_file, e_ceiling));
		else if (ft_strncmp(line + i, "D_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_file, e_door));
		else if (ft_strncmp(line + i, "T_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_file, e_exit));
		else if (ft_strncmp(line + i, "M_", 2) == 0)
			return (find_music(game, line + i + 3, e_music, 0));
		else if (ft_strncmp(line + i, "H_", 2) == 0)
			return (find_music(game, line + i + 3, e_narrator, 0));
	}
	else if (next_wsp - i == 4)
	{
		if (ft_strncmp(line + i, "OE_", 3) == 0)
			return (_find_texture(game, line + i + 4, game->nb_file, e_object_entity));
		else if (ft_strncmp(line + i, "OW_", 3) == 0)
			return (_find_texture(game, line + i + 4, game->nb_file, e_object_wall));
		else if (ft_strncmp(line + i, "OI_", 3) == 0)
			return (_find_texture(game, line + i + 4, game->nb_file, e_object_interactive));
		else if (ft_strncmp(line + i, "MR_", 3) == 0)
			return (find_music(game, line + i + 4, e_music_receptacle, 0));
		else if (ft_strncmp(line + i, "MO_", 3) == 0)
			return (find_music(game, line + i + 4, e_music_object, 0));
		else if (ft_strncmp(line + i, "HR_", 3) == 0)
			return (find_music(game, line + i + 4, e_narrator_receptacle, 0));
	}
	else if (next_wsp - i == 5)
	{
		if (ft_strncmp(line + i, "R_", 2) == 0)
			return (_find_texture(game, line + i + 5, game->nb_file, e_receptacle_empty));
		if (ft_strncmp(line + i, "D_", 2) == 0)
			return (_find_texture(game, line + i + 5, game->nb_file, e_door_lock));
	}
	return (print_error("invalid identifier\n", 1), false);
}

bool	check_texture(t_texture	*filename)
{
	int	i;

	i = 0;
	while (i < 6)
	{
		if (filename[i].filename == NULL && filename[i].nb_file <= 0 && filename[i].nb_animation <= 0)
			return (false);
		i++;
	}
	return (true);
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
		if (ft_strlen(line + i) < 1)
			return (print_error("Line to short\n", 1), free(line), false);
		if (!_cmp_texture(line, game, i, &is_end))
			return (free(line), false);
		free(line);
		line = get_next_line(fd);
		if (is_end == true)
			break ;
	}
	*rest = line;
	if (!check_texture(game->filename))
		return (print_error("need the mandatory texture\n", 1),false);
	return (true);
}
