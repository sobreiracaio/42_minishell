/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:37:59 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 01:21:23 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	wait_children(t_shell *shell)
{
	if (waitpid(shell->pid, &g_exit, 0) != -1) // Espera o processo filho terminar.
	{
		if (WIFEXITED(g_exit)) // Se o processo filho terminou normalmente.
			g_exit = WEXITSTATUS(g_exit); // Obtém o status de saída do processo filho.
		else if (WIFSIGNALED(g_exit)) // Se o processo filho terminou devido a um sinal.
			g_exit = WTERMSIG(g_exit) + 128; // Obtém o sinal que causou a terminação do processo filho.
	}
	while (wait(0) != -1) // Espera todos os processos filhos terminarem.
		;
	if (g_exit == 130) // Se o sinal for SIGINT.
		shell->status = RESTORE; // O status é RESTORE. Senão o status é CONTINUE. Qual a diferença? O status RESTORE é usado para restaurar o prompt após um sinal SIGINT. O status CONTINUE é usado para continuar a execução do shell. Em termos práticos o que acontece? Se o utilizador carregar ctrl+c, o shell continua a correr. Se o utilizador carregar ctrl+d, o shell fecha. Porquê? Porque o ctrl+d é um sinal de EOF. O que é EOF? EOF é o fim do ficheiro. Se o utilizador carregar ctrl+d, o shell fecha. Se o utilizador carregar ctrl+c, o shell continua a correr. Porquê? Porque o ctrl+c é um sinal de interrupção. Se o utilizador carregar ctrl+c, o shell continua a correr.
}

static void	close_fds_and_sig_handler(int fd[2], int sig)
{
	if (sig)
		signal_handler(sig); // Se o sinal for diferente de zero, chama a função signal_handler.
	check(close(fd[0]), "close error", 127);
	check(close(fd[1]), "close error", 127);
}

// Esta função executa um comando de pipe. Ela cria um pipe, chama a função check_fork para criar um processo filho e executa o comando da esquerda. Em seguida, ela chama a função wait_children para esperar o processo filho terminar. Após a execução do comando da esquerda, ela chama a função check_fork para criar um novo processo filho e executa o comando da direita. Por fim, ela chama a função wait_children para esperar o processo filho terminar.
void	run_pipe(t_shell *shell, t_pipe *cmd)
{
	int	fd[2];

	check(pipe(fd), "pipe error", 127);
	shell->pid = check_fork();
	if (shell->pid == 0)
	{
		check(dup2(fd[1], STDOUT_FILENO), "dup2 error", 127);
		close_fds_and_sig_handler(fd, SIGIGNORE);
		run_cmd(shell, cmd->left); // Executa o comando da esquerda.
		free_exit(shell);
	}
	if (cmd->left->type == HERE_DOC) // Se o tipo do comando da esquerda for HERE_DOC.
		wait_children(shell); // Espera o processo filho terminar.
	if (shell->status == CONTINUE) // Se o status for CONTINUE.
		shell->pid = check_fork(); // Cria um novo processo filho, que será o comando da direita.
	if (shell->pid == 0)
	{
		check(dup2(fd[0], STDIN_FILENO), "dup2 error", 127);
		close_fds_and_sig_handler(fd, SIGIGNORE);
		run_cmd(shell, cmd->right); // Executa o comando da direita.
		free_exit(shell);
	}
	close_fds_and_sig_handler(fd, 0); // Fecha os descritores de arquivo.
	wait_children(shell); // Espera o processo filho terminar.
}
