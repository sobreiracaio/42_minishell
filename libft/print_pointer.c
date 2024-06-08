/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:37:09 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/08 19:37:11 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	first_nibble(unsigned long long addr)
{
	int	i;
	int	hex;
	int	count;

	count = -1;
	i = sizeof(addr) * 2 - 1;
	hex = 0;
	while (!hex)
	{
		hex = (addr >> (i * 4)) & 0xf;
		i--;
		count++;
	}
	return (count);
}

int	print_pointer(unsigned long long addr, int *flags)
{
	int		nibble;
	int		size;
	int		i;
	char	*hex;

	if (!addr)
		return (print_str("(nil)", flags));
	size = sizeof(addr) * 2 - first_nibble(addr) + 2;
	hex = malloc(size + 1);
	if (!hex)
		return (0);
	hex[size] = '\0';
	i = size - 2;
	while (i--)
	{
		nibble = (addr >> (i * 4)) & 0xf;
		hex[size - 1 - i] = HEXBASE[nibble];
	}
	ft_memcpy(hex, "0x", 2);
	i = print_str(hex, flags);
	free(hex);
	return (i);
}
