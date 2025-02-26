/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:52:02 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/02/26 15:05:52 by yuuchiya         ###   ########.fr       */
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

static void	free_env(void *content)
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

	if (get_env_value(env_list, key))
		return (set_env_value(env_list, key, value));
	env = malloc(sizeof(t_env));
	if (!env)
		return (0);
	env->key = ft_strdup(key);
	env->value = ft_strdup(value);
	ft_lstadd_back(&env_list, ft_lstnew(env));
	return (1);
}

int	remove_env(t_list *env_list, char *key)
{
	t_list	*current;
	t_list	*prev;
	t_env	*env;
	size_t	key_len;

	current = env_list;
	prev = NULL;
	while (current)
	{
		env = (t_env *)(current->content);
		if (ft_strlen(key) > ft_strlen(env->key))
			key_len = ft_strlen(key);
		else
			key_len = ft_strlen(env->key);
		if (ft_strncmp(env->key, key, key_len) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				env_list = current->next;
			return (ft_lstdelone(current, free_env), 1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

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
		temp = ft_strjoin(env->key, "=");
		envp[i] = ft_strjoin(temp, env->value);
		free(temp);
		i++;
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}
