/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 01:50:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/27 12:52:37 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

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
	
	printf("'%s'\n",animation->filename[index]);
	fd = open(animation->filename[index], O_RDONLY);
	if (fd == -1)
		return (perror("Error"), false);
	// buffer = get_next_line(fd);
	// animation->time_sprite = ft_atoi(buffer);
	// buffer = get_next_line(fd);
	// animation->time_animation = ft_atoi(buffer);
	while (buffer != NULL)
		buffer = get_next_line(fd);
	close(fd);
	animation->nb_sprite--;
	free(animation->filename[animation->nb_sprite]);
	animation->filename[animation->nb_sprite] = NULL;
	return (true);
}

void	swap(char **str, int a, int b)
{
	char *tmp;

	tmp = str[a];
	str[a] = str[b];
	str[b] = tmp;
}

/**
 * @brief sort the filenames of animation by alphbetic order and return if
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
		if (strlen(anim->filename[i]) >= 10 && strcmp(anim->filename[i]
			+ (strlen(anim->filename[i]) - 10) , "config.cfg") == 0)
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
			if (strcmp(anim->filename[j], anim->filename[j + 1]) > 0)
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

	// printf(" ANIM %d : %s\n",texture->nb_animation, dirname);
	texture->animation = ft_realloc(texture->animation
		, sizeof(t_animation) * (texture->nb_animation)
		,sizeof(t_animation) * (texture->nb_animation + 1));
	if (texture->animation == NULL)
			return (perror("Error"), closedir(dir), false);
	if (strlen(dirname) > 0 && dirname[strlen(dirname) - 1] == '/')
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
		// printf(" ANIM %d: filename :%s\n",texture->nb_animation, buffer->d_name);
		filename = ft_strjoin_slash(dirname, buffer->d_name, add_slash);
		if (filename == NULL)
			return (perror("Error"), closedir(dir), false);
		texture->animation[texture->nb_animation].filename = ft_realloc(
			texture->animation[texture->nb_animation].filename
			, sizeof(char *) * (texture->animation[texture->nb_animation].nb_sprite)
			, sizeof(char *) * (texture->animation[texture->nb_animation].nb_sprite + 1));
		if (texture->animation[texture->nb_animation].filename == NULL)
			return (perror("Error"), closedir(dir), false);
		texture->animation[texture->nb_animation].filename[texture->animation[texture->nb_animation].nb_sprite] = filename;
		texture->animation[texture->nb_animation].nb_sprite++;
		buffer = readdir(dir);
	}
	free(dirname);
	dirname = NULL;
	closedir(dir);
	has_config = sort_animation(&(texture->animation[texture->nb_animation]));
	if (has_config == false)
		printf("Error : Missing the file config.cfg for the animations %d\n", has_config);
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

	// printf(" DIR : %s\n", texture->filename);
	texture->nb_file = 0;
	if (strlen(texture->filename) > 0 && texture->filename[strlen(texture->filename) - 1] == '/')
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
		// printf(" DIR : filename : %s\n", buffer->d_name);
		filename = ft_strjoin_slash(texture->filename, buffer->d_name, add_slash);
		if (filename == NULL)
			return (perror("Error"), closedir(dir), false);
		tmp = opendir(filename);
		if (tmp != NULL)
		{
			if (!ft_read_anim(tmp, texture, filename))
				return (closedir(dir), false);
		}
		else
		{
			texture->filename_d = ft_realloc(texture->filename_d
				, sizeof(char *) * (texture->nb_file) , sizeof(char *) * (texture->nb_file + 1));
			if (texture->filename == NULL)
				return (perror("Error"), closedir(dir), false);
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
	game->filename[index].nb_file = 1;
	if (index >= e_door_close)
		game->filename[index].symbol = *(str - 1);
	else if (index == e_floor || index == e_ceiling)
		game->filename[index].symbol = '0';
	else
		game->filename[index].symbol = '1';
	dir = opendir(filename);
	if (dir != NULL)
		return (ft_read_dir(dir, &(game->filename[index])));
	return (true);
}

/**
 * @brief return the index of the next whitespace or the '\0' if none has been find
 */
static int	_find_next_wsp(char *line , int i)
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
static bool	_cmp_texture(char *line, t_game *game, int i, bool *is_end)
{
	int	next_wsp;

	next_wsp = _find_next_wsp(line, i);
	if (next_wsp - i == 1)
	{
		if (line[i] == 'F')
			return (_find_texture(game, line + i + 1, e_floor, e_down));
		else if (line[i] == 'C')
			return (_find_texture(game, line + i + 1, e_ceiling, e_up));
		else if (line[i] == 'c')
			return (_find_texture(game, line + i + 1, e_door_close, e_wall));
		else if (line[i] == 'o')
			return (_find_texture(game, line + i + 1, e_door_open, e_wall));
		else
		 	return (_find_texture(game, line + i + 1, game->nb_sprite, e_wall));
	}
	else if (next_wsp - i == 2)
	{
		if (strncmp(line + i, "NO", 2) == 0)
			return (_find_texture(game, line + i + 2, e_north_wall, e_north));
		else if (strncmp(line + i, "SO", 2) == 0)
			return (_find_texture(game, line + i + 2, e_south_wall, e_south));
		else if (strncmp(line + i, "WE", 2) == 0)
			return (_find_texture(game, line + i + 2, e_west_wall, e_west));
		else if (strncmp(line + i, "EA", 2) == 0)
			return (_find_texture(game, line + i + 2, e_east_wall, e_east));
	}
	else if (next_wsp - i == 3)
	{
		if (strncmp(line + i, "MAP\n", 4) == 0 || strncmp(line + i, "MAP\0", 4) == 0)
			return (*is_end = true, true);
		if (strncmp(line + i, "N_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_sprite, e_north));
		else if (strncmp(line + i, "S_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_sprite, e_south));
		else if (strncmp(line + i, "W_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_sprite, e_west));
		else if (strncmp(line + i, "E_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_sprite, e_east));
		else if (strncmp(line + i, "F_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_sprite, e_down));
		else if (strncmp(line + i, "C_", 2) == 0)
			return (_find_texture(game, line + i + 3, game->nb_sprite, e_up));
	}
	return (printf("Error : invalid identifier %s\n", line), false);
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
		if (!_cmp_texture(line, game, i, &is_end))
			return (free(line), false);
		free(line);
		line = get_next_line(fd);
		if (is_end == true)
			break ;
	}
	*rest = line;
	return (true);
}
