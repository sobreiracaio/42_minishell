/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:04:28 by luide-so          #+#    #+#             */
/*   Updated: 2024/05/29 01:13:57 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

static void	sig_quit(int sig) 
{
	if (sig == SIGINT) // Se o sinal for SIGINT (Ctrl+C)
		write(1, "\n", 1); // Escreve uma nova linha no terminal
	else if (sig == SIGQUIT) // Se o sinal for SIGQUIT (Ctrl+\)
		ft_putendl_fd("Quit (core dumped)", 1); // Imprime "Quit (core dumped)" no terminal
}

static void	sigrestore(int sig) // Função para restaurar o comportamento padrão do shell após SIGINT
{
	(void)sig; // Evita warnings de parâmetro não usado
	g_exit = 130; // Define o código de saída global como 130
	write(1, "\n", 1); // Escreve uma nova linha no terminal
	rl_on_new_line(); // Informa ao readline para iniciar uma nova linha
	rl_replace_line("", 0); // Substitui a linha atual por uma linha vazia
	rl_redisplay(); // Redisplay da linha de comando do readline
}

static void	sig_here_doc(int sig) // Função para tratar SIGINT no contexto de heredoc
{
	if (sig != SIGINT) // Se o sinal não for SIGINT
		return; // Retorna sem fazer nada
	exit(130); // Sai do processo com o código de saída 130
}

static void	sigint_ignore(int sig) // Função para ignorar SIGINT
{
	if (sig == SIGINT) // Se o sinal for SIGINT
	{
		g_exit = 130; // Define o código de saída global como 130
		ft_putstr_fd("\n", 2); // Escreve uma nova linha no terminal
		signal(SIGINT, SIG_IGN); // Define SIGINT para ser ignorado
	}
}

void	signal_handler(int sig) // Função principal de manipulação de sinais
{
	if (sig == SIGRESTORE) // Se o sinal for SIGRESTORE
	{
		signal(SIGINT, sigrestore); // Define o handler para SIGINT como sigrestore
		signal(SIGQUIT, SIG_IGN); // Ignora SIGQUIT
	}
	if (sig == SIGHEREDOC) // Se o sinal for SIGHEREDOC
	{
		signal(SIGINT, sig_here_doc); // Define o handler para SIGINT como sig_here_doc
		signal(SIGQUIT, SIG_IGN); // Ignora SIGQUIT
	}
	if (sig == SIGCHILD) // Se o sinal for SIGCHILD
	{
		signal(SIGINT, sig_quit); // Define o handler para SIGINT como sig_quit
		signal(SIGQUIT, sig_quit); // Define o handler para SIGQUIT como sig_quit
	}
	if (sig == SIGPIPE) // Se o sinal for SIGPIPE
	{
		signal(SIGINT, sigint_ignore); // Define o handler para SIGINT como sigint_ignore
		signal(SIGQUIT, SIG_IGN); // Ignora SIGQUIT
	}
	if (sig == SIGIGNORE) // Se o sinal for SIGIGNORE
	{
		signal(SIGINT, SIG_IGN); // Ignora SIGINT
		signal(SIGQUIT, SIG_IGN); // Ignora SIGQUIT
	}
}
