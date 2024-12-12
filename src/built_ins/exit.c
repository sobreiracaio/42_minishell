/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 22:57:29 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 01:55:21 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// A função ft_isnumber verifica se o argumento é um número. Se o argumento for um número, a função retorna true. Se o argumento não for um número, a função retorna false.
static bool	ft_isnumber(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (false);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
			return (false);
	}
	return (true);
}

// A função islonglong verifica se o argumento é um número long long. Se o argumento for um número long long, a função retorna true. Se o argumento não for um número long long, a função retorna false.
static bool	islonglong(char *str)
{
	int	is_negative;
	int	len;

	if (*str == '-')
	{
		is_negative = 1;
		str++;
	}
	while (*str == '0' || *str == ' ')
		str++;
	if (!ft_isnumber(str))
		return (false);
	len = ft_strlen(str);
	if (len > 19)
		return (false);
	if (is_negative)
		return (ft_strncmp(str, "9223372036854775808", len) <= 0);
	else
		return (ft_strncmp(str, "9223372036854775807", len) <= 0);
}

// A função ms_exit verifica se o comando tem argumentos. Se o comando tiver argumentos, a função imprime um erro. Se o comando não tiver argumentos, a função verifica se o argumento é um número. Se o argumento não for um número, a função imprime um erro. Se o argumento for um número, a função verifica se o número é maior que 9223372036854775807. Se o número for maior que 9223372036854775807, a função imprime um erro. Se o número for menor ou igual a 9223372036854775807, a função atribui o número a g_exit. 
void	ms_exit(t_shell *shell, t_exec *cmd)
{
	char	*err;

	err = NULL;
	if (cmd->argv[2])
		print_error(shell, cmd->argv[0], "too many arguments", 2); // Se o comando tiver mais de um argumento é impresso um erro.
	if (cmd->argv[1]) // Se o comando tiver argumentos.
	{
		if (!islonglong(cmd->argv[1]) && ft_strcmp(cmd->argv[1], "0")) // Se o argumento não for um número long long e for diferente de zero.
		{
			err = ft_strjoin(cmd->argv[1], ": numeric argument required"); // Junta o argumento com ": numeric argument required".
			print_error(shell, cmd->argv[0], err, 2); // Imprime o erro.
			free(err);
		}
		else
			g_exit = ft_atoi(cmd->argv[1]) % 256; // Atribui o argumento a g_exit, e divide por 256 pq vai de 0 a 255.
	}
	shell->status = STOP; // Interrompe o loop principal da shell (que está no main).
}
