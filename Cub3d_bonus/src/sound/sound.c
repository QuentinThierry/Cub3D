/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:17:06 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/06 22:50:07 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	free_sound(t_sound *sound)
{
	free(sound->buffer);
	ao_close(sound->device);
}

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


t_sound	init_track(const char *sound_path, bool *error)
{
	t_sound	sound;
	int		fd;
	int driver;

	*error = false;
	bzero(&sound, sizeof(t_sound));
	fd = open(sound_path, O_RDONLY);
	if (fd < 0)
		return (perror("Error"), *error = true, (t_sound){0});
	parse_wav_file(fd, &sound.format, &sound.buf_size);
	if (sound.buf_size == 0)
		return (close(fd), perror("Error"), *error = true, (t_sound){0});
	sound.buffer = calloc(sound.buf_size, sizeof(char));
	if (!sound.buffer)
		return (close(fd), perror("Error"), *error = true, (t_sound){0});
	if (read(fd, sound.buffer, sound.buf_size) <= 0)
		return (free(sound.buffer), close(fd), perror("Error"), *error = true, (t_sound){0});
	driver = ao_default_driver_id();
	sound.device = ao_open_live(driver, &sound.format, NULL);
	close(fd);
	if (!sound.device)
		return (free(sound.buffer), perror("Error"), *error = true, (t_sound){0});
	return (sound);
}

void	*load_track(t_sound_thread *so_thread, double player_angle)
{
	int				i;
	double			angle;
	double			dist;
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
		angle = (double)delta.y / delta.x;
	angle = atanf(angle) * 180 / M_PI + 90 + ((delta.x < 0) * 180);
	angle -= player_angle;
	if (angle < 0)
		angle += 360;
	else if (angle >= 360)
		angle -= 360;
	dist = sqrtf(delta.x * delta.x + delta.y + delta.y);
	volume.x = MAX_VOLUME / dist;
	volume.y = MAX_VOLUME / dist;
	new_buffer = ft_calloc(sound.buf_size, sizeof(char));
	if (!new_buffer)
		return (NULL);
	ft_memcpy(new_buffer, sound.buffer, sound.buf_size * sizeof(char));
	i = 0;
	while (i < so_thread->sound.buf_size / 2)
	{
		// left
		((short *)new_buffer)[i] = ((short *)sound.buffer)[i] * (1 - (fabs(angle - 270) / 180)) * volume.x;
		i++;
		// right
		((short *)new_buffer)[i] = ((short *)sound.buffer)[i] * (1 - (fabs(angle - 90) / 180)) * volume.y;
		i++;
	}
	return (new_buffer);
}

void	*ambient_music_thr(t_game *game)
{
	bool			error;
	t_sound_thread	sound_thread;
	t_sound			sound;
	char			*buffer;

	sound = init_track("assets/sounds/app.wav", &error);
	if (error)
		return (perror("Sound error"), NULL);
	sound_thread = (t_sound_thread)
	{
		sound,
		0,
		(t_vector2){0, 0},
		(t_vector2){0, 0},
		PTHREAD_MUTEX_INITIALIZER
	};
	buffer = load_track(&sound_thread, 0);
	free(sound.buffer);
	sound.buffer = NULL;
	if (!buffer)
		return (ao_close(sound.device), perror("Sound error"), NULL);
	while (true)
	{
		printf("new play\n");
		pthread_mutex_lock(&game->sound_end_mut);
		if (game->sound_end == 1)
			return (pthread_mutex_unlock(&game->sound_end_mut),
				free(buffer), free_sound(&sound), NULL);
		pthread_mutex_unlock(&game->sound_end_mut);
		ao_play(sound.device, buffer, sound.buf_size);
	}
}

void	*sound_manager_thr(t_game *game)
{
	pthread_t	ambient_thread;

	ao_initialize();
	pthread_create(&ambient_thread, NULL, (void *)ambient_music_thr, game);
	pthread_detach(ambient_thread);
	while (true)
	{
		usleep(1000);
		pthread_mutex_lock(&game->sound_end_mut);
		if (game->sound_end == 1)
			break ;
		pthread_mutex_unlock(&game->sound_end_mut);
	}
	printf("canceling\n");
	ao_shutdown();
	return (0);
}
