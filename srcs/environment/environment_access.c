/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_access.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:32:56 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 11:50:00 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

char	*get_env_value(t_list *env_list, char *key)
{
	t_list	*current;
	t_env	*env;
	size_t	key_len;

	current = env_list;
	while (current)
	{
		env = (t_env *)(current->content);
		if (ft_strlen(key) > ft_strlen(env->key))
			key_len = ft_strlen(key);
		else
			key_len = ft_strlen(env->key);
		if (ft_strncmp(env->key, key, key_len) == 0)
			return (env->value);
		current = current->next;
	}
	return (NULL);
}

int	set_env_value(t_list *env_list, char *key, char *value)
{
	t_list	*current;
	t_env	*env;

	current = env_list;
	while (current)
	{
		env = (t_env *)(current->content);
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

int	add_env(t_list *env_list, char *key, char *value)
{
	t_env	*env;

	if (set_env_value(env_list, key, value))
		return (1);
	env = malloc(sizeof(t_env));
	if (!env)
		return (0);
	env->key = ft_strdup(key);
	env->value = ft_strdup(value);
	ft_lstadd_back(&env_list, ft_lstnew(env));
	return (1);
}

int	remove_env(t_list **env_list, char *key)
{
	t_list	*current;
	t_list	*prev;
	t_env	*env;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		env = (t_env *)(current->content);
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			return (ft_lstdelone(current, free_env), 1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
