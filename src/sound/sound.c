/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:17:06 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/08 19:17:04 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

