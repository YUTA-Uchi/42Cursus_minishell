/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:36:34 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/21 16:08:17 by yuuchiya         ###   ########.fr       */
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
#include <sys/stat.h>

#define PATH_MAX 4096

int	main(int argc, char **argv)
{
	struct stat	*st;

	(void)argc;
	st = malloc(sizeof(struct stat));
	stat(argv[1], st);
	printf("st_mode: %d\n", st->st_mode);
	printf("S_ISDIR: %d\n", S_ISDIR(st->st_mode));
	printf("S_ISREG: %d\n", S_ISREG(st->st_mode));
	free(st);
	return (0);
}
