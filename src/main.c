/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:08 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/08 21:44:02 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

typedef struct s_vector2
{
	int	x;
	int	y;
}	t_vector2;

typedef struct s_sound
{
	ao_device			*device;
	ao_sample_format	format;
	char				*buffer;
	long				buf_size;
}	t_sound;

t_sound	init_sound(const char *sound_path, int driver)
{
	t_sound	sound;
	int		fd;
	ssize_t	read_size;

	bzero(&sound, sizeof(t_sound));
	fd = open(sound_path, O_RDONLY);
	if (fd < 0)
		return (errno = 1, (t_sound){0});
	parse_wav_file(fd, &sound.format, &sound.buf_size);
	if (sound.buf_size == 0)
		return (close(fd), errno = 2, (t_sound){0});
	sound.buffer = calloc(sound.buf_size, sizeof(char));
	if (!sound.buffer)
		return (close(fd), errno = 3, (t_sound){0});
	if (read(fd, sound.buffer, sound.buf_size) <= 0)
		return (free(sound.buffer), close(fd), errno = 4, (t_sound){0});
	driver = ao_default_driver_id();
	sound.device = ao_open_live(driver, &sound.format, NULL);
	if (!sound.device)
		return (free(sound.buffer), close(fd), errno = 5, (t_sound){0});
	// ao_play(sound.device, sound.buffer, sound.buf_size);
	errno = 0;
	close(fd);
	return (sound);
}

// creer une struct pour chaque son
void	play_sound(t_sound *sound, t_vector2 listen_pos, t_vector2 emit_pos)
{
	ao_play(sound->device, sound->buffer, sound->buf_size);
}

int main(int argc, char **argv)
{
	int		driver;
	t_sound	sound;
	t_sound	sound2;
	void	*mlx_ptr;

	ao_initialize();
	mlx_ptr = mlx_init();

	// mlx_loop(mlx_ptr);
	
	
	sound = init_sound("assets/sounds/CantinaBand3.wav", driver);
	sound = init_sound("assets/sounds/stereo.wav", driver);
	if (errno != 0)
		return (perror("truc"), 1);
	play_sound(&sound, (t_vector2){0 ,0}, (t_vector2){0 ,0});
	// play_sound(&sound2, (t_vector2){0 ,0}, (t_vector2){0 ,0});
	// int i = 0;
	// // if (pid == 0)
	// 	// usleep(50000);
	// while (i < buf_size / 2)
	// {
	// 	if (i % 2 == (0)) // right
	// 		((short *)buffer)[i] *= 0* i * 2 / (float) buf_size;
	// 	else 
	// 		((short *)buffer)[i] *= (float)(1 - (float)(i * 2/ (float)buf_size));
	// 	i++;
	// }

	/* -- Close and shutdown -- */
	free(sound.buffer);
	ao_close(sound.device);


	
	ao_shutdown();
	return (0);
}
