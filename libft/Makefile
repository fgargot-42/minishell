# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fgargot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/20 19:31:22 by fgargot           #+#    #+#              #
#    Updated: 2026/01/29 16:39:27 by fgargot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a
CC=cc
CFLAGS=-Wall -Wextra -Werror
INCDIR=.
SRC_MAIN=ft_isalpha.c \
		 ft_isdigit.c \
		 ft_isalnum.c \
		 ft_isascii.c \
		 ft_isprint.c \
		 ft_strlen.c \
		 ft_memset.c \
		 ft_bzero.c \
		 ft_memcpy.c \
		 ft_memmove.c \
		 ft_strlcpy.c \
		 ft_strlcat.c \
		 ft_toupper.c \
		 ft_tolower.c \
		 ft_strchr.c \
		 ft_strrchr.c \
		 ft_strncmp.c \
		 ft_memchr.c \
		 ft_memcmp.c \
		 ft_strnstr.c \
		 ft_atoi.c \
		 ft_calloc.c \
		 ft_strdup.c \
		 ft_substr.c \
		 ft_strjoin.c \
		 ft_strtrim.c \
		 ft_split.c \
		 ft_itoa.c \
		 ft_strmapi.c \
		 ft_striteri.c \
		 ft_putchar_fd.c \
		 ft_putstr_fd.c \
		 ft_putendl_fd.c \
		 ft_putnbr_fd.c
SRC_BONUS=ft_lstnew_bonus.c \
		  ft_lstadd_front_bonus.c \
		  ft_lstsize_bonus.c \
		  ft_lstlast_bonus.c \
		  ft_lstadd_back_bonus.c \
		  ft_lstdelone_bonus.c \
		  ft_lstclear_bonus.c \
		  ft_lstiter_bonus.c \
		  ft_lstmap_bonus.c \
		  ft_lstsort_bonus.c
OBJ_MAIN=$(SRC_MAIN:.c=.o)
OBJ_BONUS=$(SRC_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ_MAIN)
	ar -rcs $@ $?

bonus: $(NAME) $(OBJ_BONUS)
	ar -rcs $?

%.o: %.c
	$(CC) $(CFLAGS) -I $(INCDIR) -c $? -o $@

clean:
	rm -f $(OBJ_MAIN) $(OBJ_BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
