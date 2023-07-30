/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:33:47 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/30 18:30:59 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

enum e_orientation	get_wall_orientation(t_fvector2 player, t_fvector2 wall)
{
	if ((wall.x - (int)wall.x) != 0)
	{
		if (player.y > wall.y)
		 	return (e_south);
		else
			return (e_north);
	}
	else
	{
		if (player.x > wall.x)
		 	return (e_east);
		else
			return (e_west);
	}
}

static void	update_anim(struct timespec	*time, t_sprite *sprite, t_image *img)
{
	long	delta;
	int		n_time;

	delta = (sprite->time.tv_sec - time->tv_sec) / 1000
			+ (sprite->time.tv_sec - time->tv_sec) * 1000;
	if (sprite->frame == img->nb_total_frame && delta > img->time_animation)
	{
		sprite->frame = 0;
		sprite->time.tv_sec = (img->time_animation / 1000);
		sprite->time.tv_nsec = (img->time_animation * 1000);
		delta = (sprite->time.tv_sec - time->tv_sec) / 1000
				+ (sprite->time.tv_sec - time->tv_sec) * 1000;
	}
	n_time = delta % (img->time_frame * img->nb_total_frame + img->time_animation);
	if (n_time > 0)
	{
		sprite->time.tv_sec = ((img->time_frame * img->nb_total_frame
				+ img->time_animation) / 1000) * n_time;
		sprite->time.tv_nsec = ((img->time_frame * img->nb_total_frame
				+ img->time_animation) * 1000) * n_time;
		delta = (sprite->time.tv_sec - time->tv_sec) / 1000
			+ (sprite->time.tv_sec - time->tv_sec) * 1000;
		n_time = delta % img->time_frame;
	}
	if (n_time >= img->nb_total_frame - sprite->frame)
	{
		sprite->frame = img->nb_total_frame;
		sprite->time.tv_sec = (img->time_frame / 1000) * (img->nb_total_frame - sprite->frame);
		sprite->time.tv_nsec = (img->time_frame * 1000) * (img->nb_total_frame - sprite->frame);
		delta = (sprite->time.tv_sec - time->tv_sec) / 1000
			+ (sprite->time.tv_sec - time->tv_sec) * 1000;
		if (delta > img->time_animation)
		{
			sprite->frame = 0;
			sprite->time.tv_sec = (img->time_animation / 1000);
			sprite->time.tv_nsec = (img->time_animation * 1000);
		}
		delta = (sprite->time.tv_sec - time->tv_sec) / 1000
			+ (sprite->time.tv_sec - time->tv_sec) * 1000;
		n_time = delta % img->time_frame;
	}
	if (n_time < img->nb_total_frame - sprite->frame)
	{
		sprite->frame = (sprite->frame + n_time) % img->nb_total_frame;
		sprite->time.tv_sec = (img->time_frame / 1000) * n_time;
		sprite->time.tv_nsec = (img->time_frame * 1000) * n_time;
	};
}

t_image	*get_image(t_game *game, enum e_orientation orient, t_fvector2 wall)
{
	t_sprite img;
	
	if (orient == e_south)
		img = game->map[(int)wall.y - 1][(int)wall.x].sprite[orient];
	else if (orient == e_north || orient == e_west)
		img = game->map[(int)wall.y][(int)wall.x].sprite[orient];
	if (orient == e_east)
		img = game->map[(int)wall.y][(int)wall.x - 1].sprite[orient];
	if (img.frame == -1)
		return (&(game->tab_images[img.index]));
	else		//animation
	{
		if (img.time.tv_nsec == 0 && img.time.tv_nsec == 0)
		{
			img.time = game->time;
			return (&(game->tab_images[img.index + img.frame]));
		}
		else if (img.time.tv_sec + (game->tab_images[img.index].time_frame / 1000) < game->time.tv_sec
			|| img.time.tv_nsec + (game->tab_images[img.index].time_frame * 1000) < game->time.tv_nsec)
			update_anim(&game->time, &img, &game->tab_images[img.index]);
		return (&(game->tab_images[img.index + img.frame]));
	}
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

bool	check_symbol(char *str)
{
	int	len;
	int	i;

	i = 0;
	len = ft_strlen(str);
	while (i < len)
	{
		if (i == len - 1 && str[len - 1] == '\n')
			str[len - 1] = '\0';
		else if (!(str[i] == ' ' || str[i] == '0' || str[i] == '1' || str[i] == 'o'
				|| str[i] == 'c' || str[i] == 'N' || str[i] == 'E'
				|| str[i] == 'S' || str[i] == 'W'))
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Get the dimension of the map in the file
 * 
 * @param fd fd to read the map and until the end of the file
 * @param line last line read in the file with gnl
 * @param error set on true if error occurs during the function NOT USE FOR THE MOMENT
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
			break;
		// if (!check_symbol(line))
		// 	*error = true;
		else if (len.x < (int)ft_strlen(line))
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
 * @brief return true if the symbol is a wall on the table of texture
 * 
 * @param symbol 
 * @param tab 
 * @param len 
 * @param error
 */
bool	is_wall(char symbol, t_texture *tab, int len, bool *error)
{
	int i;

	i = 0;
	*error = false;
	while (i < len)
	{
		if (tab[i].symbol == symbol)
		{
			if (tab[i].orient == e_wall || tab[i].orient == e_north || tab[i].orient == e_south
					|| tab[i].orient == e_east || tab[i].orient == e_west)
				return (true);
			else
				return (false);
		}
		i++;
	}
	return (*error = true, false);
}

/**
 * @brief Get the number of different texture without the "config.cfg"
 * 
 * @param texture 
 * @param len 
 * @return int 
 */
int	get_len_texture(t_texture *texture, int len)
{
	int i;
	int j;
	int	res;

	res = 0;
	i = 0;
	while (i < len)
	{
		if (texture[i].filename != NULL)
			res++;
		else
		{
			res += texture[i].nb_file;
			j = 0;
			while (j < texture[i].nb_animation)
			{
				res += texture[i].animation[j].nb_sprite - 1;
				j++;
			}
		}
		i++;
	}
	return (res);
}

int	ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}