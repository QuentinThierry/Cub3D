/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:19:43 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 14:56:40 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	print_error(char *error, int print)
{
	if (print == 1)
		printf("Error\n%s", error);
	else
	{
		printf("Error\n");
		perror("");
	}
}
