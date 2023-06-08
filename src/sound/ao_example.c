/*
 *
 * ao_example.c
 *
 *     Written by Stan Seibert - July 2001
 *
 * Legal Terms:
 *
 *     This source file is released into the public domain.  It is
 *     distributed without any warranty; without even the implied
 *     warranty * of merchantability or fitness for a particular
 *     purpose.
 *
 * Function:
 *
 *     This program opens the default driver and plays a 440 Hz tone for
 *     one second.
 *
 * Compilation command line (for Linux systems):
 *
 *     gcc -o ao_example ao_example.c -lao -ldl -lm
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ao.h"

#define BUF_SIZE 4096

void parsing(int fd, ao_sample_format *format, long *data_size)
{
	unsigned char buffer[44];

	read(fd, buffer, 44);
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
	
	printf("bits : %d	format : %d			channels: %d 	rate : %d\n", format->bits, format->byte_format, format->channels
	, format->rate);
}


int main(int argc, char **argv)
{
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	char *buffer;
	long buf_size;

	ao_initialize();

	/* -- Setup for default driver -- */

	default_driver = ao_default_driver_id();
	// perror("default driver");

	memset(&format, 0, sizeof(format));
	// int pid = fork();
	int fd = open("stereo.wav", O_RDONLY);

	parsing(fd, &format, &buf_size);

	/* -- Open driver -- */
	device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return 1;
	}
	printf("size : %ld\n", buf_size);
	buffer = calloc(buf_size,
			sizeof(char));
	read(fd, buffer, buf_size);
	int i = 0;
	// if (pid == 0)
		// usleep(50000);
	while (i < buf_size / 2)
	{
		if (i % 2 == (0)) // right
			((short *)buffer)[i] *= 0* i * 2 / (float) buf_size;
		else 
			((short *)buffer)[i] *= (float)(1 - (float)(i * 2/ (float)buf_size));
		i++;
	}
	ao_play(device, buffer, buf_size);

	/* -- Close and shutdown -- */
	ao_close(device);
	free(buffer);
	close(fd);

	ao_shutdown();

  return (0);
}
// 