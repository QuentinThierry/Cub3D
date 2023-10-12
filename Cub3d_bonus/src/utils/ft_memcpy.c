/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 10:22:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/12 20:17:39 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	register u_int32_t	i;
	register u_int32_t	size;

	size = sizeof(u_int32_t);
	i = 0;
	if (n > size)
	{
		while (i < n - size)
		{
			*(u_int32_t *)(dest + i) = *(u_int32_t *)(src + i);
			i += size;
		}
	}
	while (i != n)
	{
		*(unsigned char *)(dest + i) = *(unsigned char *)(src + i);
		i++;
	}
	return (dest);
}
