/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:36:34 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/18 15:04:53 by yuuchiya         ###   ########.fr       */
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
#include "libft.h"

#define PATH_MAX 4096

int	main(int argc, char **argv)
{
	char		*absolute_path_buff;
	char		*current_path;

	if (argc > 2)
		return (printf("minishell: cd: too many arguments\n"), 1);
	if (argc == 1)
	{
		absolute_path_buff = getenv("HOME");
		if (absolute_path_buff == NULL)
			return (printf("minishell: cd: HOME not set\n"), 1);
	}
	else
		absolute_path_buff = argv[1];
	if (chdir(absolute_path_buff) == -1)
	{
		printf("minishell: cd: %s: %s\n", absolute_path_buff, strerror(errno));
		return (1);
	}
	current_path = (char *)malloc(PATH_MAX);
	getcwd(current_path, PATH_MAX);
	printf("%s\n", current_path);
	free(current_path);
	return (0);
}
