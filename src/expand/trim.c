/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:30:07 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/13 00:49:16 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Esta função insere um caratere nulo em cada espaço em branco que não está entre aspas.
void	arg_insert_null(char *arg)
{
	int	quote;

	quote = 0;
	while (*arg)
	{
		if ((*arg == '"' || *arg == '\'') && !quote)
			quote = *arg;
		else if (quote == *arg)
			quote = 0;
		if (ft_strchr(SPACES, *arg) && !quote)
			*arg = '\0';
		arg++;
	}
}

// Esta função remove as aspas de uma string.
void	trim_quotes(char *arg, int *len)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (i < *len)
	{
		if ((arg[i] == '"' || arg[i] == '\'') && !quote) // Se o caractere atual for uma aspa e não houver aspas abertas.
		{
			quote = arg[i];
			memmove(arg + i, arg + i + 1, *len - i); // Move o bloco de memória de arg + i + 1 para arg + i, com um tamanho de *len - i.
			(*len)--;
		}
		else if (quote && arg[i] == quote) // Se houver aspas abertas e o caractere atual for uma aspa.
		{
			quote = 0;
			memmove(arg + i, arg + i + 1, *len - i); // Move o bloco de memória de arg + i + 1 para arg + i, com um tamanho de *len - i.
			(*len)--;
		}
		else
			i++;
	}
}
