
#ifndef SHELL_STATE_H
# define SHELL_STATE_H

# include "minishell.h"

typedef struct s_shell_state
{
	bool	running;
	int		last_status;
}	t_shell_state;

t_shell_state	*create_shell_state(void);
void			free_shell_state(t_shell_state *shell_state);
#endif