/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-jes <jode-jes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 09:56:55 by luide-so          #+#    #+#             */
/*   Updated: 2024/05/27 15:24:44 by jode-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_env(t_shell *shell, t_exec *cmd)
{
	if (cmd->argv[1])
		print_error(shell, "env", "too many arguments", 2);
	else
	{
		if (env_get("PATH", shell))
			envp_print(shell);
		else
			print_error(shell, cmd->argv[0], "No such file or directory", 127);
	}
	if (shell->status == CONTINUE)
		g_exit = 0;
}