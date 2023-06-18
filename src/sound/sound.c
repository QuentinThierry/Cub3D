/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:17:06 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/18 14:56:39 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

typedef struct s_sound
{
	ao_device			*device;
	ao_sample_format	format;
	char				*buffer;
	long				buf_size;
}	t_sound;

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
	// perror("Error6");
	close(fd);
	return (sound);
}

// creer une struct pour chaque son
void	play_sound(t_sound *sound, t_vector2 listen_pos, t_vector2 emit_pos, bool *error)
{
	int i;

	(void)listen_pos;
	(void)emit_pos;
	i = 0;
	while (i < sound->buf_size / 2)
	{
		if (i % 2 == (1)) // right
			((short *)sound->buffer)[i] = 0;//* i * 2 / (float) sound->buf_size;
		// else 
		// 	((short *)sound->buffer)[i] *= (float)(1 - (float)(i * 2/ (float)sound->buf_size));
		i++;
	}
	*error = ao_play(sound->device, sound->buffer, sound->buf_size);
	if (*error == 0)
		return (*error = true, perror("Error7"));
}

int sound(void)
{
	bool	error;
	t_sound	sound;

	error = false;
	ao_initialize();

	// mlx_loop(mlx_ptr);
	
	sound = init_sound("assets/sounds/stereo.wav", &error);
	if (error == true)
		return (perror("truc1"), 1);
	play_sound(&sound, (t_vector2){0 ,0}, (t_vector2){0 ,0}, &error);
	if (error == true)
		return (perror("truc2"), 1);
	
	/* -- Close and shutdown -- */
	free(sound.buffer);
	ao_close(sound.device);
	ao_shutdown();
	return (0);
}


