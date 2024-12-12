/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 00:32:43 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 02:08:30 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Esta função verifica se a variável de ambiente para fazer unset é válida. Se a variável de ambiente for válida, a função retorna true. Se a variável de ambiente não for válida, a função retorna false.
static bool	valid_unset_var(t_shell *shell, char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_')
		return (error_inside(shell, "unset: ", arg, 1));
	while (arg[++i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (error_inside(shell, "unset: ", arg, 1));
	}
	return (true);
}

// A função get_key é chamada na função ms_unset. Se a chave existir, a função retorna a chave. Se a chave não existir, a função retorna NULL.
char	*get_key(char *key, t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env_list_unsorted;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (tmp->key);
		tmp = tmp->next;
	}
	return (NULL);
}

// A função ms_unset é chamada na função run_builtin. Se os argumentos forem válidos, a função remove as variáveis de ambiente. Se os argumentos não forem válidos, a função imprime um erro.
void	ms_unset(t_shell *shell, t_exec *cmd)
{
	int	i;

	i = 0;
	while (cmd->argv[++i])
	{
		if (*cmd->argv[i] && valid_unset_var(shell, cmd->argv[i])) // Se os argumentos forem válidos...
		{
			if (get_key(cmd->argv[i], shell)) // Se a chave existir...
				env_rm(cmd->argv[i], shell); // ... remove a variável de ambiente.
		}
	}
	if (shell->status == CONTINUE)
		g_exit = 0;
}
