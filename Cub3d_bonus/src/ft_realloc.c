/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 19:15:03 by jvigny            #+#    #+#             */
/*   Updated: 2023/07/16 02:43:23 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	*ft_realloc(void *ptr, size_t prev_size, size_t new_size)
{
	void	*new_ptr;

	new_ptr = ft_calloc(new_size, 1);
	if (!new_ptr)
		return (NULL);
	if (prev_size > new_size)
		prev_size = new_size;
	ft_memcpy(new_ptr, ptr, prev_size);
	free(ptr);
	return (new_ptr);
}