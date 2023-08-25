/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 18:04:11 by jvigny            #+#    #+#             */
/*   Updated: 2023/08/25 20:37:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

char	*ft_strdup(const char *s)
{
	char	*res;
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen((char *)s);
	res = ft_calloc(sizeof(char), len + 1);
	if (res == 0)
		return (0);
	while (i < len)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = 0;
	return (res);
}
