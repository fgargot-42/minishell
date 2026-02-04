# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fgargot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/20 19:31:22 by fgargot           #+#    #+#              #
#    Updated: 2026/01/29 02:56:22 by fgargot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a
CC=cc
CFLAGS=-Wall -Wextra -Werror
INCDIR=includes
SRCDIR=srcs
OBJDIR=objs
SRC=ft_is/ft_isalpha.c \
	ft_is/ft_isdigit.c \
	ft_is/ft_isalnum.c \
	ft_is/ft_isascii.c \
	ft_is/ft_isprint.c \
	ft_mem/ft_memset.c \
	ft_mem/ft_bzero.c \
	ft_mem/ft_memcpy.c \
	ft_mem/ft_memmove.c \
	ft_mem/ft_memchr.c \
	ft_mem/ft_memcmp.c \
	ft_mem/ft_calloc.c \
	ft_str/ft_strlen.c \
	ft_str/ft_strlcpy.c \
	ft_str/ft_strlcat.c \
	ft_str/ft_toupper.c \
	ft_str/ft_tolower.c \
	ft_str/ft_strchr.c \
	ft_str/ft_strrchr.c \
	ft_str/ft_strncmp.c \
	ft_str/ft_strnstr.c \
	ft_str/ft_atoi.c \
	ft_str/ft_strdup.c \
	ft_str/ft_substr.c \
	ft_str/ft_strjoin.c \
	ft_str/ft_strtrim.c \
	ft_str/ft_split.c \
	ft_str/ft_itoa.c \
	ft_str/ft_strmapi.c \
	ft_str/ft_striteri.c \
	ft_putfd/ft_putchar_fd.c \
	ft_putfd/ft_putstr_fd.c \
	ft_putfd/ft_putendl_fd.c \
	ft_putfd/ft_putnbr_fd.c \
	ft_lst/ft_lstnew.c \
	ft_lst/ft_lstadd_front.c \
	ft_lst/ft_lstsize.c \
	ft_lst/ft_lstlast.c \
	ft_lst/ft_lstadd_back.c \
	ft_lst/ft_lstdelone.c \
	ft_lst/ft_lstclear.c \
	ft_lst/ft_lstiter.c \
	ft_lst/ft_lstmap.c \
	ft_lst/ft_lstsort.c \
	ft_gnl/get_next_line.c \
	ft_printf/ft_printf.c \
	ft_printf/ft_convert.c \
	ft_printf/ft_convert2.c \
	ft_printf/ft_printf_utils.c
OBJ=$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(addprefix $(OBJDIR)/,$(OBJ))
	ar -rcs $@ $?

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $? -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
