/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:10:27 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/26 14:17:50 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../libft/libft.h"

typedef struct s_exit_status
{
	int	status;
	int	exit_code;
}	t_exit_status;
typedef int	(*t_handler)(t_exit_status *, t_list **, char);

int	handler_none(t_exit_status *state, t_list **a, char c)
{
	(void)state;
	(void)c;
	(void)a;
	return (0);
}

int	handler_something(t_exit_status *state, t_list **a, char c)
{
	(void)state;
	(void)c;
	(void)a;
	return (0);
}

int	handler_something2(t_exit_status *state, t_list **a, char c)
{
	(void)state;
	(void)c;
	(void)a;
	return (0);
}

int	handler_something3(t_exit_status *state, t_list **a, char c)
{
	(void)state;
	(void)c;
	(void)a;
	return (0);
}

int	handler_something4(t_exit_status *state, t_list **a, char c)
{
	(void)state;
	(void)c;
	(void)a;
	return (0);
}

int handle(t_exit_status *state, t_list **a, char c)
{
	const t_handler	handlers[4] = {
		handler_none,
		handler_something,
		handler_something2,
		handler_something3
	};
	return (handlers[0](state, a, c));
}

int	main(int argc, char**argv)
{
	return (handle(NULL, NULL, 0));
}
