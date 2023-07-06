/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/07 00:14:11 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

enum e_orientation	get_wall_orientation2(t_game *game, t_player player, t_fvector2 wall)
{
	int	x;
	int	y;

	x = wall.x;
	y = wall.y;
	if ((wall.x - x) >= 0.99)
		x += 1;
	if ((wall.y - y) >= 0.99)
		y += 1;
	printf("orientation		%c	%c\n			%c	%c\n\n\n\n\n\n\n\n\n", game->maps[y - 1][x - 1]
		, game->maps[y - 1][x], game->maps[y][x - 1]
		, game->maps[y][x]);
	if (game->maps[y][x] == '1')
	{
		if (game->maps[y - 1][x] == '1' && game->maps[y - 1][x - 1] == '0'
			&& game->maps[y][x - 1] == '0')
			return (e_east);
		if (game->maps[y][x - 1] == '1' && game->maps[y - 1][x - 1] == '0'
			&& game->maps[y - 1][x] == '0')
			return (e_south);
		if (game->maps[y - 1][x] == '1' && game->maps[y][x - 1] == '1'
			&& game->maps[y - 1][x - 1] == '0')
			return (e_south);
		if (game->maps[y - 1][x] == '0' && game->maps[y][x - 1] == '0'
			&& game->maps[y - 1][x - 1] == '0')
			return (e_east);
	}
	if (game->maps[y - 1][x] == '1')
	{
		if (game->maps[y - 1][x - 1] == '1' && game->maps[y][x] == '0'
			&& game->maps[y][x - 1] == '0')
			return (e_north);
		if (game->maps[y][x] == '1' && game->maps[y - 1][x - 1] == '1'
			&& game->maps[y][x - 1] == '0')
			return (e_east);
	}
	if (game->maps[y - 1][x - 1] == '1')
	{
		if (game->maps[y][x - 1] == '1' && game->maps[y - 1][x - 1] == '0'
			&& game->maps[y - 1][x] == '0')
			return (e_west);
		if (game->maps[y][x - 1] == '1' && game->maps[y - 1][x] == '1'
			&& game->maps[y][x] == '0')
			return (e_north);
	}
	return (e_west);
}

enum e_orientation	get_wall_orientation(t_game *game, t_player player, t_fvector2 wall)
{
	// printf("x : %f		y : %f\n", wall.x, wall.y);
	if (!((wall.x - (int)wall.x) <= 0.0001 || (wall.x - (int)wall.x) >= 0.9999))
	{
		if (player.f_real_pos.y > wall.y)
			return (e_north);
		else
		 	return (e_south);
	}
	else
	{
		if ((wall.y - (int)wall.y) <= 0.0001 || (wall.y - (int)wall.y) >= 0.9999)
			return (get_wall_orientation2(game, player, wall));
		if (player.f_real_pos.x > wall.x)
			return (e_west);
		else
		 	return (e_east);
	}
}

t_image	*get_image(t_game *game, enum e_orientation orient)
{
	return (game->tab_images[orient]);
}


int skip_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' || str[i] == '\n'
			|| str[i] == '\f' || str[i] == '\r')
		i++;
	return (i);
}

void	remove_new_line(char *str)
{
	int	len;

	len = strlen(str);
	if (len >= 1 && str[len - 1] == '\n')
		str[len - 1] = '\0';
}

t_vector2	get_dimension_maps(int fd, int nb_line, char *line)
{
	int	i;
	t_vector2	len;

	len.y = 0;
	len.x = 0;
	while (line != NULL)
	{
		i = skip_whitespace(line);
		if (line[0] == '\n' || line[0] == '\0' || line[i] == '\0')
			break;
		remove_new_line(line);
		if (len.x < (int)strlen(line))
			len.x = strlen(line);
		len.y++;
		free(line);
		line = get_next_line(fd);
	}
	while (line != NULL)
	{
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (len);
}

void	free_tab(char **str, t_vector2 size)
{
	int	i;

	i = 0;
	while (i < size.y)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_str(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}
