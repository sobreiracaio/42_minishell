/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:36:24 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 01:46:31 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// A função check_flag verifica se existe alguma flag no comando. Se existir, a função verifica se a flag é '-n'. Se a flag for '-n', a função retorna 1. Se a flag não for '-n', a função retorna 0.
static int	check_flag(t_exec *cmd, int *arg_index)
{
	int		flag;
	int		i;
	char	*arg;

	flag = 0;
	arg = cmd->argv[++*arg_index];
	while (arg)
	{
		if (arg[0] == '-' && arg[1] == 'n') // Se a flag for '-n'. Aqui verifica exatamente -n e não somente -.
		{
			i = 1;
			while (arg[i])
			{
				if (arg[i] != 'n')
					return (flag);
				i++;
			}
			flag = 1;
		}
		else
			break ;
		arg = cmd->argv[++*arg_index]; // Próximo argumento.
	}
	return (flag);
}

// A função ms_echo verifica primeiramente se existe alguma flag no comando. Se existir, a função verifica se a flag é '-n'. Se a flag for '-n', a função não imprime uma nova linha. Se a flag não for '-n', a função imprime uma nova linha. Se não existir nenhuma flag, a função imprime uma nova linha.
void	ms_echo(t_exec *cmd)
{
	int	i;
	int	flag;

	i = 0;
	flag = check_flag(cmd, &i);
	if (cmd->argv[i]) // Se o comando tiver argumentos.
	{
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO); // Imprime o argumento.
		while (cmd->argv[++i]) // Enquanto houver argumentos.
		{
			ft_putchar_fd(' ', STDOUT_FILENO); // Imprime um espaço.
			ft_putstr_fd(cmd->argv[i], STDOUT_FILENO); // Imprime o argumento.
		}
	}
	if (!flag) // Se não existir nenhuma flag.
		ft_putchar_fd('\n', STDOUT_FILENO); // Imprime uma nova linha.
	g_exit = 0;
}
