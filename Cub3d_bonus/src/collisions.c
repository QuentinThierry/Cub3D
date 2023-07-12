/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 01:05:58 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/13 01:04:11 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"
t_fvector2	get_collision(t_fvector2 fpos, char **map, t_fvector2 new_pos, t_vector2 map_size);


void	check_colliding(t_player *player, t_fvector2 new_pos, char **map, t_vector2 map_size)
{
	t_fvector2 tmp;
	// if (fabs(player->f_pos.x - new_pos.x) > CHUNK_SIZE)
	// 	printf("error\n");
	// if (fabs(player->f_pos.y - new_pos.y) > CHUNK_SIZE)
	// 	printf("error\n");
	tmp.x = new_pos.x / CHUNK_SIZE;
	tmp.y = new_pos.y / CHUNK_SIZE;
	player->f_real_pos = get_collision(player->f_real_pos, map, tmp, map_size);
	// printf("player x:%f		y:%f	res	x:%f	y:%f		dest x:%f		y:%f\n"
	// 		, player->f_real_pos.x, player->f_real_pos.y, res.x, res.y, new_pos.x / CHUNK_SIZE, new_pos.y / CHUNK_SIZE);
	player->f_pos.x = player->f_real_pos.x * CHUNK_SIZE;
	player->f_pos.y = player->f_real_pos.y * CHUNK_SIZE;
	player->pos.x = (int)player->f_pos.x;
	player->pos.y = (int)player->f_pos.y;
}

	// if (map[(int)(player->f_pos.y / CHUNK_SIZE)][(int)(new_pos.x / CHUNK_SIZE)] == '1')
	// {
	// 	if (player->f_pos.x - new_pos.x <= 0)
	// 		player->f_pos.x = ((int)(new_pos.x / CHUNK_SIZE)) * CHUNK_SIZE - 0.1;
	// 	else
	// 		player->f_pos.x = ((int)(new_pos.x / CHUNK_SIZE) + 1) * CHUNK_SIZE + 0.1;
	// }
	// else
	// 	player->f_pos.x = new_pos.x;
	// if (map[(int)(new_pos.y / CHUNK_SIZE)][(int)(player->f_pos.x / CHUNK_SIZE)] == '1')
	// {
	// 	if (player->f_pos.y - new_pos.y <= 0)
	// 		player->f_pos.y = ((int)(new_pos.y / CHUNK_SIZE)) * CHUNK_SIZE - 0.1;
	// 	else
	// 		player->f_pos.y = ((int)(new_pos.y / CHUNK_SIZE) + 1) * CHUNK_SIZE + 0.1;
	// }
	// else
	// 	player->f_pos.y = new_pos.y;

	// player->pos.x = (int)player->f_pos.x;
	// player->pos.y = (int)player->f_pos.y;
	// player->f_real_pos.x = player->f_pos.x / CHUNK_SIZE;
	// player->f_real_pos.y = player->f_pos.y / CHUNK_SIZE;


// xy (1, 1)
static inline t_fvector2	_get_collision_se(t_fvector2 fpos,
								char **map, t_fvector2 new_pos, t_vector2 map_size)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	float		angle;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_fvector2){angle, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x > new_pos.x)
				return (new_pos);
			if (map[(int)(comp.y)][map_pos.x] == '1')
				return ((t_fvector2){map_pos.x - 0.001, comp.y - (0.001 / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else 
		{
			if (map_pos.y > new_pos.y)
				return (new_pos);
			if (map[map_pos.y][(int)(comp.x)] == '1')
				return ((t_fvector2){comp.x - (0.001 / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y - 0.001});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (1, -1)
static inline t_fvector2	_get_collision_ne(t_fvector2 fpos,
								char **map, t_fvector2 new_pos, t_vector2 map_size)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	float		angle;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_vector2){(int)fpos.x + 1, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x > new_pos.x)
				return (new_pos);
			if (map[(int)(comp.y)][map_pos.x] == '1')
				return ((t_fvector2){map_pos.x - 0.001, comp.y + (0.001 / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))});
			comp.y += step.y;
			map_pos.x += 1;
		}
		else
		{
			if (map_pos.y < new_pos.y)
				return (new_pos);
			if (map[map_pos.y - 1][((int)(comp.x))] == '1')
				return ((t_fvector2){comp.x - (0.001 / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y + 0.001});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

// xy (-1, 1)
static inline t_fvector2	_get_collision_sw(t_fvector2 fpos,
								char **map, t_fvector2 new_pos, t_vector2 map_size)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	float		angle;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y + 1};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle;
	step = (t_fvector2){angle * -1, 1 / angle};
	while (true)
	{
		if (map_pos.y >= comp.y)
		{
			if (map_pos.x < new_pos.x)
				return (new_pos);
			if (map[(int)(comp.y)][map_pos.x - 1] == '1')
				return ((t_fvector2){map_pos.x + 0.001, comp.y - (0.001 / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y > new_pos.y)
				return (new_pos);
			if (map[map_pos.y][((int)(comp.x))] == '1')
				return ((t_fvector2){comp.x + (0.001 / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y - 0.001});
			comp.x += step.x;
			map_pos.y += 1;
		}
	}
}

// xy (-1, -1)
static inline t_fvector2	_get_collision_nw(t_fvector2 fpos,
								char **map, t_fvector2 new_pos)
{
	t_fvector2	step;
	t_fvector2	comp;
	t_vector2	map_pos;
	float		angle;

	angle = fabs(fpos.x - new_pos.x) / fabs(fpos.y - new_pos.y);
	map_pos = (t_vector2){(int)fpos.x, (int)fpos.y};
	comp.x = fpos.x + fabs(fpos.y - map_pos.y) * angle * -1;
	comp.y = fpos.y + fabs(fpos.x - map_pos.x) / angle * -1;
	step = (t_fvector2){angle * -1, 1 / angle * -1};
	while (true)
	{
		if (map_pos.y <= comp.y)
		{
			if (map_pos.x < new_pos.x)
				return (new_pos);
			if (map[(int)(comp.y)][map_pos.x - 1] == '1')
				return ((t_fvector2){map_pos.x + 0.001, comp.y + (0.001 / fabs(fpos.x - map_pos.x) * fabs(fpos.y - comp.y))});
			comp.y += step.y;
			map_pos.x += -1;
		}
		else
		{
			if (map_pos.y < new_pos.y)
				return (new_pos);
			if (map[map_pos.y - 1][((int)(comp.x))] == '1')
				return ((t_fvector2){comp.x + (0.001 / fabs(fpos.y - map_pos.y) * fabs(fpos.x - comp.x)), map_pos.y + 0.001});
			comp.x += step.x;
			map_pos.y += -1;
		}
	}
}

inline t_fvector2	get_collision(t_fvector2 fpos, char **map, t_fvector2 new_pos, t_vector2 map_size)
{
	t_vector2	sign;

	// sign = get_sign(angle);
	// angle = fabs(tan(angle * TO_RADIAN));
	if (fpos.x - new_pos.x < 0)
		sign.x = 1;
	else
		sign.x = -1;
	if (fpos.y - new_pos.y < 0)
		sign.y = 1;
	else
		sign.y = -1;
	if (sign.x == 1 && sign.y == 1)
		return (_get_collision_se(fpos, map, new_pos, map_size));
	else if (sign.x == 1 && sign.y == -1)
		return (_get_collision_ne(fpos, map, new_pos, map_size));
	else if (sign.x == -1 && sign.y == 1)
		return (_get_collision_sw(fpos, map, new_pos, map_size));
	else
		return (_get_collision_nw(fpos, map, new_pos));
	return ((t_fvector2){0});
}
