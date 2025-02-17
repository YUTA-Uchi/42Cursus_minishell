/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:10:27 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/17 14:39:00 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	last_status = 5;

bool	is_numeric(char *s)
{
	if (!isdigit(*s))
		return (false);
	while (*s)
	{
		if (!isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

int	main(int argc, char**argv)
{
	//unsigned long	res;
	char			*status_num;

	if (argv[1] == NULL)
		exit(last_status);
	if (argc > 2)
	{
		printf("too many variables...\n");
		return (1);
	}
	status_num = argv[1];
	if (is_numeric(status_num))
	{
		errno = 0;
		// TODO unsigned int (0~255) or not
		exit(atoi(status_num));
	}
	printf("numeric argument required");
	exit(255);
}
