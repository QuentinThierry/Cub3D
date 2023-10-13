/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:19:43 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/13 13:02:39 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

/**
 * @brief print char * if number is 1 else print perror
 * 
 * @param error 
 * @param print 
 */
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
