/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:09:38 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/17 14:39:11 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define PATH_MAX 4

int	main(void)
{
	char	*absolute_path_buff;

	absolute_path_buff = malloc(sizeof(char) * PATH_MAX);
	if (getcwd(absolute_path_buff, PATH_MAX) == NULL)
		return (printf("minishell: %s\n", strerror(errno)), 1);
	if (!absolute_path_buff)
		return (printf("buff is null\n"), 1);
	write(STDOUT_FILENO, absolute_path_buff, strlen(absolute_path_buff));
	write(STDOUT_FILENO, "\n", 1);
	free(absolute_path_buff);
}

