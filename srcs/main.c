/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:39:39 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/18 18:10:31 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "front_desk.h"

int	main(void)
{
	t_front_desk	*front_desk;

	front_desk = create_front_desk();
	if (!front_desk)
		exit(1);
	while (1)
	{
		set_line(front_desk);
		if (!front_desk->parser->line)
			continue ;
		set_cmds(front_desk);
		// if (!front_desk->executor->cmds)
		// 	// TODO error handling parser error;
		front_desk->executor->excute(front_desk->executor);
		free(front_desk->parser->line);
	}
	free_front_desk(front_desk);
	exit(0);
}
