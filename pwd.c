/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:09:38 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/15 19:23:42 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define PATH_MAX 4096

int	main(void)
{
	char	*absolute_path_buff;

	absolute_path_buff = malloc(sizeof(char) * PATH_MAX);
	getcwd(absolute_path_buff, PATH_MAX);
	if (!absolute_path_buff)
		return (1);
	write(STDOUT_FILENO, absolute_path_buff, strlen(absolute_path_buff));
	write(STDOUT_FILENO, "\n", 1);
	free(absolute_path_buff);
}

