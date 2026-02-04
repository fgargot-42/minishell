/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:51:53 by fgargot           #+#    #+#             */
/*   Updated: 2025/12/11 21:04:41 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>

int		ft_print_c(va_list args);
int		ft_print_s(va_list args);
int		ft_print_p(va_list args);
int		ft_print_d(va_list args);
int		ft_print_i(va_list args);
int		ft_print_u(va_list args);
int		ft_print_xlower(va_list args);
int		ft_print_xupper(va_list args);
char	*ft_itoa_hex(long nb);

#endif
