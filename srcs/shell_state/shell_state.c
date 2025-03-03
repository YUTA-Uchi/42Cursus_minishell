
#include "shell_state.h"

t_shell_state	*create_shell_state(void)
{
	t_shell_state	*shell_state;

	shell_state = malloc(sizeof(t_shell_state));
	if (!shell_state)
		return (NULL);
	shell_state->running = true;
	shell_state->last_status = 0;
	return (shell_state);
}

void	free_shell_state(t_shell_state *shell_state)
{
	free(shell_state);
}
