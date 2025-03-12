
#ifndef SHELL_STATE_H
# define SHELL_STATE_H

# include "minishell.h"
# include "error_handler.h"
# include "environment.h"

typedef struct s_shell_state
{
	bool			running;
	int				last_status;
	t_error_handler	*error_handler;
	t_list			*env_list;
}	t_shell_state;

t_shell_state	*create_shell_state(char **environ);
void			free_shell_state(t_shell_state *shell_state);
#endif