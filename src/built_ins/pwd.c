/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:49:24 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 02:05:52 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// A função ms_pwd verifica se o comando tem argumentos. Se o comando tiver argumentos, a função imprime um erro. Se o comando não tiver argumentos, a função verifica se o argumento é um flag. Se o argumento for uma flag, a função imprime um erro. Se o argumento não for uma flag, a função verifica se o argumento é um diretório. Se o argumento for um diretório, a função verifica se o diretório existe. Se o diretório existir, a função imprime o diretório. Se o diretório não existir, a função imprime um erro.
void	ms_pwd(t_shell *shell, t_exec *cmd)
{
	char	*path;

	if (cmd->argv[1] && cmd->argv[1][0] == '-')
	{
		print_error(shell, cmd->argv[0], "flags not supported", 2);
		return ;
	}
	path = getcwd(NULL, 0);
	if (!path)
		print_error(shell, cmd->argv[0], "couldn't get current directory", 2);
	ft_putendl_fd(path, STDOUT_FILENO);
	free(path);
	if (shell->status == CONTINUE)
		g_exit = 0;
}
