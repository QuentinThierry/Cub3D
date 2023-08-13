/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 10:22:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/13 19:17:08 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// void	*ft_memcpy(void *dest, const void *src, size_t n)
// {
// 	size_t	i;

// 	i = 0;
// 	if (dest == 0 && src == 0)
// 		return (0);
// 	while (i < n)
// 	{
// 		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
// 		i++;
// 	}
// 	return (dest);
// }

__attribute__((always_inline))
inline void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	register size_t			i;
	register const size_t	size = sizeof(size_t);

	i = 0;
	if (n > size)
	{
		while (i < n - size)
		{
			*(size_t *)(dest + i) = *(size_t *)(src + i);
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

// inline void	*ft_memcpy(void *dest, const void *src, const size_t n);

// __attribute__((always_inline))
// inline void	*ft_memcpy(void *dest, const void *src, const size_t n)
// {
// 	register size_t			i;
// 	register const size_t			size = sizeof(size_t);
// 	const size_t			*src_cpy = src;
// 	size_t					*dest_cpy = dest;

// 	i = 0;
// 	if (n > size)
// 	{
// 		while (i < n - size)
// 		{
// 			*dest_cpy = *src_cpy;
// 			dest_cpy++;
// 			src_cpy++;
// 			i += size;
// 		}
// 	}
// 	src = src_cpy;
// 	dest = dest_cpy;
// 	while (i != n)
// 	{
// 		*(unsigned char *)(dest++) = *(unsigned char *)(src++);
// 		i++;
// 	}
// 	return (dest_cpy);
// }
