/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 15:46:25 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/04 15:41:46 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

char	*ft_strjoin(char *str, char *str1)
{
	char	*res_f;
	size_t	len;
	size_t	i;

	i = 0;
	len = strlen(str1);
	if (str != NULL)
		len += strlen(str);
	res_f = ft_calloc(sizeof(char), len + 1);
	if (res_f == NULL)
		return (NULL);
	while (str != NULL && str[i] != '\0')
	{
		res_f[i] = str[i];
		i++;
	}
	len = 0;
	while (str1 != NULL && str1[len] != '\0')
	{
		res_f[i] = str1[len];
		i++;
		len++;
	}
	res_f[i] = 0;
	return (res_f);
}

char	*ft_strjoin_slash(char *str, char *str1, bool add_slash)
{
	char	*res;
	size_t	len;
	size_t	i;

	i = 0;
	len = strlen(str) + strlen(str1) + add_slash;
	res = ft_calloc(sizeof(char), len + 1);
	if (res == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		res[i] = str[i];
		i++;
	}
	if (add_slash)
		res[i++] = '/';
	len = 0;
	while (str1[len] != '\0')
	{
		res[i] = str1[len];
		i++;
		len++;
	}
	res[i] = 0;
	return (res);
}
