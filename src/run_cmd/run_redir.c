/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:38:07 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 01:12:57 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Esta função expande o arquivo de redirecionamento. Como? Ela chama a função expand_arg, que expande o tilde e as variáveis de ambiente. Em seguida, ela chama a função ft_strlen para obter o comprimento do arquivo de redirecionamento. Em seguida, ela chama a função arg_insert_null para inserir um caractere nulo em cada espaço em branco que não está entre aspas. Por fim, ela chama a função trim_quotes para remover as aspas do arquivo de redirecionamento.
static int	expand_file(t_shell *shell, char **file)
{
	int	len;

	expand_arg(shell, file);
	len = ft_strlen(*file);
	arg_insert_null(*file);
	trim_quotes(*file, &len);
	return (1);
}

// Esta função executa um comando de redirecionamento. Ela salva o descritor de arquivo original, abre o arquivo de redirecionamento e executa o comando. Após a execução do comando, o descritor de arquivo é restaurado.
void	run_redir(t_shell *shell, t_redir *cmd)
{
	int	fd;
	int	original_fd;

	original_fd = dup(cmd->fd);
	fd = -2;
	if (expand_file(shell, &cmd->file))
		fd = open(cmd->file, cmd->mode, 0644);
	if (fd == -1)
		print_error(shell, cmd->file, strerror(errno), 1);
	else if (shell->status == CONTINUE)
	{
		dup2(fd, cmd->fd);
		close(fd);
		run_cmd(shell, cmd->cmd);
	}
	check(dup2(original_fd, cmd->fd), "dup2 error", 1);
}
