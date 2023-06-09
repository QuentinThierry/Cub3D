/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:14:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/08 19:04:56 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <strings.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdbool.h>

# include "ao.h"
#include "../includes/minilibx-linux/mlx.h"

void parse_wav_file(int fd, ao_sample_format *format, long *data_size);

#endif