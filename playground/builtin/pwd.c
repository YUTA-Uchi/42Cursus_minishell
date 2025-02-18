/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:09:38 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/18 15:06:56 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../../libft/libft.h"

#define PATH_MAX 4096

int	main(void)
{
	char	*absolute_path_buff;

	absolute_path_buff = malloc(sizeof(char) * PATH_MAX);
	if (getcwd(absolute_path_buff, PATH_MAX) == NULL)
		return (ft_printf(STDERR_FILENO, "minishell: %s\n", strerror(errno)), 1);
	if (!absolute_path_buff)
		return (ft_printf(STDERR_FILENO, "buff is null\n"), 1);
	ft_printf(STDOUT_FILENO, "%s\n", absolute_path_buff);
	free(absolute_path_buff);
}

