/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_filename.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 18:31:01 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/15 18:31:13 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

char	*get_filename(char *str, int *i)
{
	int		len;
	char	*filename;
	char	tmp;

	tmp = -1;
	*i += skip_whitespace(str + *i);
	if (str[*i] == '\0')
		return (print_error("Empty texture\n", 1), NULL);
	len = find_next_wsp(str + *i, 0);
	if (len >= 0 && (str[*i + len] == ' ' || str[*i + len] == '\t'
			|| str[*i + len] == '\v' || str[*i + len] == '\n'
			|| str[*i + len] == '\f' || str[*i + len] == '\r'))
	{
		tmp = str[*i + len];
		str[*i + len] = '\0';
	}
	filename = ft_strdup(str + *i);
	if (filename == NULL)
		return (print_error("malloc failed\n", 1), NULL);
	if (tmp != -1)
		str[*i + len] = tmp;
	*i += len + 1;
	return (filename);
}
