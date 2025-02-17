/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:36:34 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/17 18:41:28 by yuuchiya         ###   ########.fr       */
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

#define PATH_MAX 4096

int	main(int argc, char **argv)
{
	char	*absolute_path_buff;

	if (argc != 2)
		return (printf("minishell: cd: too many arguments\n"), 1);
	absolute_path_buff = malloc(sizeof(char) * PATH_MAX);
	if (getcwd(absolute_path_buff, PATH_MAX) == NULL)
		return (printf("minishell: %s\n", strerror(errno)), 1);
	if (!absolute_path_buff)
		return (printf("buff is null\n"), 1);
	if (chdir(argv[1]) == -1)
	{
		printf("minishell: cd: %s: %s\n", argv[1], strerror(errno));
		free(absolute_path_buff);
		return (1);
	}
	free(absolute_path_buff);
	return (0);
}