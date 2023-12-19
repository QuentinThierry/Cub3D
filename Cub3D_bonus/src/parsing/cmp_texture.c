/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 20:53:37 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 20:55:37 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	find_texture(t_game *game, char *str, int index,
			enum e_orientation orient);

static bool	_check_type_for_3(t_game *game, char *line, int i, bool *is_end)
{
	if (ft_strncmp(line + i, "MAP\n", 4) == 0
		|| ft_strncmp(line + i, "MAP\0", 4) == 0)
		return (*is_end = true, true);
	if (ft_strncmp(line + i, "N_", 2) == 0)
		return (find_texture(game, line + i + 3, game->nb_file, e_north));
	else if (ft_strncmp(line + i, "S_", 2) == 0)
		return (find_texture(game, line + i + 3, game->nb_file, e_south));
	else if (ft_strncmp(line + i, "W_", 2) == 0)
		return (find_texture(game, line + i + 3, game->nb_file, e_west));
	else if (ft_strncmp(line + i, "E_", 2) == 0)
		return (find_texture(game, line + i + 3, game->nb_file, e_east));
	else if (ft_strncmp(line + i, "F_", 2) == 0)
		return (find_texture(game, line + i + 3, game->nb_file, e_floor));
	else if (ft_strncmp(line + i, "C_", 2) == 0)
		return (find_texture(game, line + i + 3, game->nb_file, e_ceiling));
	else if (ft_strncmp(line + i, "D_", 2) == 0)
		return (find_texture(game, line + i + 3, game->nb_file, e_door));
	else if (ft_strncmp(line + i, "T_", 2) == 0)
		return (find_texture(game, line + i + 3, game->nb_file, e_exit));
	else if (ft_strncmp(line + i, "M_", 2) == 0)
		return (find_music(game, line + i + 3, e_music, 0));
	else if (ft_strncmp(line + i, "H_", 2) == 0)
		return (find_music(game, line + i + 3, e_narrator, 0));
	return (print_error("invalid identifier\n", 1), false);
}

static bool	_check_type_for_4(t_game *game, char *line, int i)
{
	if (ft_strncmp(line + i, "OE_", 3) == 0)
		return (find_texture(game, line + i + 4, game->nb_file,
				e_object_entity));
	else if (ft_strncmp(line + i, "OW_", 3) == 0)
		return (find_texture(game, line + i + 4, game->nb_file,
				e_object_wall));
	else if (ft_strncmp(line + i, "OI_", 3) == 0)
		return (find_texture(game, line + i + 4, game->nb_file,
				e_object_interactive));
	else if (ft_strncmp(line + i, "MR_", 3) == 0)
		return (find_music(game, line + i + 4, e_music_receptacle, 0));
	else if (ft_strncmp(line + i, "MO_", 3) == 0)
		return (find_music(game, line + i + 4, e_music_object, 0));
	else if (ft_strncmp(line + i, "HR_", 3) == 0)
		return (find_music(game, line + i + 4, e_narrator_receptacle, 0));
	return (print_error("invalid identifier\n", 1), false);
}

static bool	_check_type_for_5(t_game *game, char *line, int i)
{
	if (ft_strncmp(line + i, "R_", 2) == 0)
		return (find_texture(game, line + i + 5, game->nb_file,
				e_receptacle_empty));
	if (ft_strncmp(line + i, "D_", 2) == 0)
		return (find_texture(game, line + i + 5, game->nb_file, e_door_lock));
	return (print_error("invalid identifier\n", 1), false);
}

static bool	_check_type_for_mandatory(t_game *game, char *line, int i)
{
	if (ft_strncmp(line + i, "NO", 2) == 0)
		return (find_texture(game, line + i + 2, e_north, e_north));
	else if (ft_strncmp(line + i, "SO", 2) == 0)
		return (find_texture(game, line + i + 2, e_south, e_south));
	else if (ft_strncmp(line + i, "WE", 2) == 0)
		return (find_texture(game, line + i + 2, e_west, e_west));
	else if (ft_strncmp(line + i, "EA", 2) == 0)
		return (find_texture(game, line + i + 2, e_east, e_east));
	return (print_error("invalid identifier\n", 1), false);
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
			return (find_texture(game, line + i + 1, e_floor, e_floor));
		else if (line[i] == 'C')
			return (find_texture(game, line + i + 1, e_ceiling, e_ceiling));
		else
			return (find_texture(game, line + i + 1, game->nb_file, e_wall));
	}
	if (next_wsp - i == 2)
		return (_check_type_for_mandatory(game, line, i));
	if (next_wsp - i == 3)
		return (_check_type_for_3(game, line, i, is_end));
	if (next_wsp - i == 4)
		return (_check_type_for_4(game, line, i));
	if (next_wsp - i == 5)
		return (_check_type_for_5(game, line, i));
	return (print_error("invalid identifier\n", 1), false);
}
