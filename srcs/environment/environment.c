/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:52:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 11:41:43 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*new_str;
	size_t	i;

	new_str = (char *)malloc(n + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < n)
	{
		new_str[i] = s[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

t_list	*create_env_list(char **envp)
{
	t_list	*env_list;
	t_env	*env;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		env = malloc(sizeof(t_env));
		if (!env)
			return (NULL);
		env->key = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
		env->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		ft_lstadd_back(&env_list, ft_lstnew(env));
		i++;
	}
	return (env_list);
}

void	free_env(void *content)
{
	t_env	*env;

	env = (t_env *)(content);
	free(env->key);
	free(env->value);
	free(env);
}

void	free_env_list(t_list *env_list)
{
	ft_lstclear(&env_list, free_env);
}
