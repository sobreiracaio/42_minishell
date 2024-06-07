/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 08:55:01 by luide-so          #+#    #+#             */
/*   Updated: 2024/06/07 02:12:37 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*pdest;
	const char	*psrc;

	if (!dest && !src)
		return (NULL);
	pdest = dest;
	psrc = src;
	if (pdest < psrc)
		while (n--)
			*pdest++ = *psrc++;
	else
	{
		pdest += n;
		psrc += n;
		while (n--)
			*--pdest = *--psrc;
	}
	return (dest);
}
