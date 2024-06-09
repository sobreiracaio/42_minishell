/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:39:35 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/09 21:39:15 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void	ms_unset(t_shell *shell, t_exec *cmd)
{
	int		i;
	t_env	*tmp;
	t_env	*prev;

	i = 0;
	while (cmd->argv[++i])
	{
		if (*cmd->argv[i] && valid_unset_var(shell, cmd->argv[i]))
		{
			tmp = shell->env_list_unsorted;
			prev = NULL;
			while (tmp)
			{
				if (!ft_strcmp(tmp->key, cmd->argv[i]))
				{
					if (prev)
						prev->next = tmp->next;
					else
						shell->env_list_unsorted = tmp->next;
					free(tmp->key);
					free(tmp->value);
					free(tmp);
					break;
				}
				prev = tmp;
				tmp = tmp->next;
			}
		}
	}
	if (shell->status == CONTINUE)
		g_exit = 0;
}
