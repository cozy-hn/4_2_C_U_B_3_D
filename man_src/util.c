/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiko <jiko@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 05:12:08 by sumjo             #+#    #+#             */
/*   Updated: 2024/02/27 17:54:37 by jiko             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	get_time(void)
{
	struct timeval	time;
	int				ret;

	ret = gettimeofday(&time, NULL);
	if (ret == -1)
		return (0);
	return (time.tv_sec * 1000000 + time.tv_usec);
}

int	ft_exit(int ret, char *str)
{
	if (str)
		ft_putstr_fd(str, 2);
	exit(ret);
}

int	ft_strlen_doble(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}
