/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:17:06 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/19 19:51:50 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void parse_wav_file(int fd, ao_sample_format *format, long *data_size)
{
	unsigned char	buffer[44];
	size_t			read_size;

	read_size = read(fd, buffer, 44);
	if (read_size < 44)
	{
		*data_size = 0;
		return ;
	}
	format->bits = buffer[34] |
					(buffer[35] << 8);
	format->byte_format = buffer[20] |
				(buffer[21] << 8);

	format->channels = buffer[22] |
				(buffer[23] << 8);
	format->rate =  buffer[24] |
				(buffer[25] << 8) |
				(buffer[26] << 16) |
				(buffer[27] << 24);
	*data_size = buffer[40] |
				(buffer[41] << 8) |
				(buffer[42] << 16) |
				(buffer[43] << 24);
	printf("bits : %d	format : %d			channels: %d 	rate : %d\n",
		format->bits, format->byte_format, format->channels
	, format->rate);
}


t_sound	init_sound(const char *sound_path, bool *error)
{
	t_sound	sound;
	int		fd;
	int driver;

	*error = false;
	bzero(&sound, sizeof(t_sound));
	fd = open(sound_path, O_RDONLY);
	if (fd < 0)
		return (perror("Error1"), *error = true, (t_sound){0});
	parse_wav_file(fd, &sound.format, &sound.buf_size);
	if (sound.buf_size == 0)
		return (close(fd), perror("Error2"), *error = true, (t_sound){0});
	sound.buffer = calloc(sound.buf_size, sizeof(char));
	if (!sound.buffer)
		return (close(fd), perror("Error3"), *error = true, (t_sound){0});
	if (read(fd, sound.buffer, sound.buf_size) <= 0)
		return (free(sound.buffer), close(fd), perror("Error4"), *error = true, (t_sound){0});
	driver = ao_default_driver_id();
	printf("driver : %d\n", driver);
	sound.device = ao_open_live(driver, &sound.format, NULL);
	if (!sound.device)
		return (free(sound.buffer), close(fd), perror("Error5"), *error = true, (t_sound){0});
	close(fd);
	return (sound);
}

// creer une struct pour chaque son
bool	play_sound(t_sound_thread *so_thread, float player_angle)
{
	int				i;
	float			angle;
	float			dist;
	t_fvector2		volume;
	t_vector2		delta;
	t_sound			sound;
	char			*new_buffer;

	sound = so_thread->sound;
	delta.x = so_thread->emitter_pos.x - so_thread->listener_pos.x;
	delta.y = so_thread->emitter_pos.y - so_thread->listener_pos.y;
	if (delta.x == 0)
		angle = 90 - (180 * (delta.y < 0));
	else
		angle = (float)delta.y / delta.x;
	angle = atanf(angle) * 180 / M_PI + 90 + ((delta.x < 0) * 180);
	angle -= player_angle;
	if (angle < 0)
		angle += 360;
	else if (angle >= 360)
		angle -= 360;
	dist = sqrtf(delta.x * delta.x + delta.y + delta.y);
	volume.x = MAX_VOLUME / dist;
	volume.y = MAX_VOLUME / dist;
	new_buffer = calloc(sound.buf_size, sizeof(char));
	memcpy(new_buffer, sound.buffer, sound.buf_size * sizeof(char));
	if (!new_buffer)
		return (false);
	i = 0;
	while (i < so_thread->sound.buf_size / 2)
	{
		// left
		((short *)new_buffer)[i] = ((short *)sound.buffer)[i] * (1 - (fabsf(angle - 270) / 180)) * volume.x;
		i++;
		// right
		((short *)new_buffer)[i] = ((short *)sound.buffer)[i] * (1 - (fabsf(angle - 90) / 180)) * volume.y;
		i++;
	}
	i = ao_play(sound.device, new_buffer, sound.buf_size);
	if (i == 0)
		return (perror("Error7"), false);
	return (true);
}

void	*idle_sound(void *sound)
{
	t_sound_thread	so_thread;

	so_thread = *(t_sound_thread*)sound;
	while (true)
	{
		play_sound(&so_thread, so_thread.player_angle);
		printf("played\n");
		usleep(1000000);
	}
	return (NULL);
}

int sound(t_game *game)
{
	bool		error;
	t_sound		sound;
	t_vector2	listen_pos;
	t_vector2	emit_pos;
	pthread_t	thread;
	t_sound_thread	so_thread;

	error = false;
	ao_initialize();

	sound = init_sound("assets/sounds/app.wav", &error);
	if (error == true)
		return (perror("truc1"), 1);
	so_thread = (t_sound_thread)
	{
		sound,
		game->player->angle,
		(t_vector2){5, 0},
		(t_vector2){0, 0},
		PTHREAD_MUTEX_INITIALIZER
	};
	pthread_create(&thread, NULL, idle_sound, &so_thread);
	usleep(1000000);
	pthread_detach(thread);
	// pthread_join(thread, NULL);


	// free(sound.buffer);
	// ao_close(sound.device);
	// ao_shutdown();
	return (0);
}


