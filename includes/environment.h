/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuuchiya <yuuchiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:41:06 by yuuchiya          #+#    #+#             */
/*   Updated: 2025/03/25 11:43:34 by yuuchiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include "minishell.h"

typedef struct s_environment
{
	char	*key;
	char	*value;
}	t_env;

t_list	*create_env_list(char **envp);
void	free_env(void *content);
void	free_env_list(t_list *env_list);
char	*ft_strndup(const char *s, size_t n);
// access
char	*get_env_value(t_list *env_list, char *key);
int		set_env_value(t_list *env_list, char *key, char *value);
int		add_env(t_list *env_list, char *key, char *value);
int		remove_env(t_list **env_list, char *key);
// utils
char	**env_list_to_array(t_list *env_list);
void	print_env(t_list *env_list, bool is_export);

#endif