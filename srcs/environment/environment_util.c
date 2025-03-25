/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:32:52 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 11:45:34 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

char	**env_list_to_array(t_list *env_list)
{
	char	**envp;
	char	*temp;
	t_list	*current;
	t_env	*env;
	int		i;

	envp = malloc(sizeof(char *) * (ft_lstsize(env_list) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		env = (t_env *)(current->content);
		if (env->value)
		{
			temp = ft_strjoin(env->key, "=");
			envp[i++] = ft_strjoin(temp, env->value);
			free(temp);
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	print_env(t_list *env_list, bool is_export)
{
	t_list	*current;
	t_env	*env;

	current = env_list;
	while (current)
	{
		env = (t_env *)(current->content);
		if (is_export)
		{
			if (env->value)
				ft_printf(STDOUT_FILENO, "declare -x %s=\"%s\"\n"\
					, env->key \
					, env->value);
			else
				ft_printf(STDOUT_FILENO, "declare -x %s\n", env->key);
		}
		else
		{
			if (env->value)
				ft_printf(STDOUT_FILENO, "%s=%s\n", env->key, env->value);
		}
		current = current->next;
	}
}
