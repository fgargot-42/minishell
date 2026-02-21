NAME=minishell
CC=cc
CFLAGS=-Wall -Werror -Wextra -g
CLINK=-lreadline
SRCDIR=src
OBJDIR=obj
INCDIR=includes
LIBFTDIR=libft
LIBFT=$(LIBFTDIR)/libft.a
SRC=main.c \
	execution/executor.c \
	execution/builtin.c \
	execution/exec_tree.c \
	execution/exec_pipe.c \
	execution/exec_path.c \
	execution/resolve_redir.c \
	execution/resolve_redir_pipe.c \
	env/env.c \
	env/env_utils.c \
	prompt/prompt.c \
	lexer/lexer.c \
	lexer/lexer_utils.c \
	lexer/lexer_syntax.c \
	lexer/lexer_token.c \
	parser/tree_parser.c \
	parser/parser_utils.c \
	parser/nodes.c \
	parser/redir.c \
	expander/expander.c \
	expander/expander_split.c \
	expander/expander_utils.c \
	expander/expander_replace.c \
	builtins/echo.c \
	builtins/cd.c \
	builtins/pwd.c \
	builtins/export.c \
	builtins/export_add.c \
	builtins/export_print.c \
	builtins/unset.c \
	builtins/env.c \
	builtins/exit.c \
	utils/file_utils.c \
	utils/string_utils.c \
	utils/string_utils2.c \
	wildcards/wildcards.c
OBJ=$(SRC:.c=.o)

$(NAME): $(addprefix $(OBJDIR)/,$(OBJ)) $(LIBFT)
	$(CC) $(CFLAGS) $(CLINK) -I$(INCDIR) $^ -o $@

all: $(NAME)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $? -o $@

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	rm -rf $(OBJDIR)
	make clean -C $(LIBFTDIR)

fclean: clean
	rm -f $(NAME) $(BONUS)
	make fclean -C $(LIBFTDIR)

re: fclean all

.PHONY: all clean fclean re
