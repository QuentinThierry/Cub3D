/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_sprite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 18:28:18 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 18:28:33 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

static t_sprite	*_get_image_south(t_map **map, t_ray ray, float *dist,
			int *type)
{
	t_vector2	wall;

	wall.x = (int)ray.hit.x;
	wall.y = (int)ray.hit.y;
	if (ray.orient == e_south)
		wall.y = (int)ceil(ray.hit.y);
	*type = map[wall.y - 1][wall.x].type;
	if ((map[wall.y - 1][wall.x].type & DOOR) == DOOR)
		return (*dist = get_texture_door(ray),
			&(map[wall.y - 1][wall.x].sprite[e_door_image]));
	else if ((map[wall.y - 1][wall.x].type & DOOR_SOUTH) == DOOR_SOUTH)
	{
		if (ray.hit.x < (int)ray.hit.x + 0.5)
			return (*type = map[wall.y][wall.x].type,
				*dist = (int)ray.hit.x + 0.5 - ray.hit.x,
				&(map[wall.y][wall.x].sprite[e_door_image]));
	}
	else if ((map[wall.y - 1][wall.x].type & DOOR_SOUTH_END) == DOOR_SOUTH_END)
		if (ray.hit.x >= (int)ray.hit.x + 0.5)
			return (*type = map[wall.y][wall.x].type,
				*dist = ray.hit.x - ((int)ray.hit.x + 0.5),
				&(map[wall.y][wall.x].sprite[e_door_image]));
	return (&(map[wall.y - 1][wall.x].sprite[ray.orient]));
}

static t_sprite	*_get_image_north(t_map **map, t_ray ray, float *dist,
				int *type)
{
	t_vector2	wall;

	wall.x = (int)ray.hit.x;
	wall.y = (int)ray.hit.y;
	*type = map[wall.y][wall.x].type;
	if ((map[wall.y][wall.x].type & DOOR) == DOOR)
		return (*dist = get_texture_door(ray),
			&(map[wall.y][wall.x].sprite[e_door_image]));
	else if ((map[wall.y][wall.x].type & DOOR_NORTH) == DOOR_NORTH)
	{
		if (ray.hit.x < (int)ray.hit.x + 0.5)
			return (*type = map[wall.y - 1][wall.x].type,
				*dist = 1 - ((int)ray.hit.x - ray.hit.x + 0.5),
				&(map[wall.y - 1][wall.x].sprite[e_door_image]));
	}
	else if ((map[wall.y][wall.x].type & DOOR_NORTH_END) == DOOR_NORTH_END)
		if (ray.hit.x >= (int)ray.hit.x + 0.5)
			return (*type = map[wall.y - 1][wall.x].type,
				*dist = 1 -(ray.hit.x - ((int)ray.hit.x + 0.5)),
				&(map[wall.y - 1][wall.x].sprite[e_door_image]));
	return (&(map[wall.y][wall.x].sprite[ray.orient]));
}

static t_sprite	*_get_image_west(t_map **map, t_ray ray, float *dist, int *type)
{
	t_vector2	wall;

	wall.x = (int)ray.hit.x;
	wall.y = (int)ray.hit.y;
	*type = map[wall.y][wall.x].type;
	if ((map[wall.y][wall.x].type & DOOR) == DOOR)
		return (*dist = get_texture_door(ray),
			&(map[wall.y][wall.x].sprite[e_door_image]));
	else if ((map[wall.y][wall.x].type & DOOR_WEST) == DOOR_WEST)
	{
		if (ray.hit.y > (int)ray.hit.y + 0.5)
			return (*type = map[wall.y][wall.x - 1].type,
				*dist = (int)ray.hit.y + 1 - ray.hit.y + 0.5,
				&(map[wall.y][wall.x - 1].sprite[e_door_image]));
	}
	else if ((map[wall.y][wall.x].type & DOOR_WEST_END) == DOOR_WEST_END)
		if (ray.hit.y <= (int)ray.hit.y + 0.5)
			return (*type = map[wall.y][wall.x - 1].type,
				*dist = (ray.hit.y - (int)ray.hit.y + 0.5),
				&(map[wall.y][wall.x - 1].sprite[e_door_image]));
	return (&(map[wall.y][wall.x].sprite[ray.orient]));
}

static t_sprite	*_get_image_east(t_map **map, t_ray ray, float *dist, int *type)
{
	t_vector2	wall;

	wall.x = (int)ray.hit.x;
	wall.y = (int)ray.hit.y;
	if (ray.orient == e_east)
		wall.x = (int)ceil(ray.hit.x);
	*type = map[wall.y][wall.x - 1].type;
	if ((map[wall.y][wall.x - 1].type & DOOR) == DOOR)
		return (*dist = get_texture_door(ray),
			&(map[wall.y][wall.x - 1].sprite[e_door_image]));
	else if ((map[wall.y][wall.x - 1].type & DOOR_EAST) == DOOR_EAST)
	{
		if (ray.hit.y > (int)ray.hit.y + 0.5)
			return (*type = map[wall.y][wall.x].type,
				*dist = ray.hit.y - (int)ray.hit.y - 0.5,
				&(map[wall.y][wall.x].sprite[e_door_image]));
	}
	else if ((map[wall.y][wall.x - 1].type & DOOR_EAST_END) == DOOR_EAST_END)
		if (ray.hit.y <= (int)ray.hit.y + 0.5)
			return (*type = map[wall.y][wall.x].type,
				*dist = 0.5 - (ray.hit.y - (int)ray.hit.y),
				&(map[wall.y][wall.x].sprite[e_door_image]));
	return (&(map[wall.y][wall.x - 1].sprite[ray.orient]));
}

t_sprite	*get_sprite(t_map **map, t_ray ray, float *dist, int *type)
{
	if (ray.orient == e_south)
		return (_get_image_south(map, ray, dist, type));
	else if (ray.orient == e_north)
		return (_get_image_north(map, ray, dist, type));
	else if (ray.orient == e_west)
		return (_get_image_west(map, ray, dist, type));
	else if (ray.orient == e_east)
		return (_get_image_east(map, ray, dist, type));
	return (NULL);
}
