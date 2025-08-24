# Makefile for Minishell

# Program name
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
# Readline library for prompt
LDFLAGS = -lreadline

# Directories
SRC_DIR = srcs
OBJ_DIR = objs
INC_DIR = includes
LIBFT_DIR = libft

# Source files
SRCS_FILES =	main.c \
				signals.c \
				error.c \
				env/env_utils.c \
				env/env_init.c \
				lexer/lexer.c \
				lexer/lexer_utils.c \
				parser/parser.c \
				parser/parser_utils.c \
				parser/ast_utils.c \
				expander/expander.c \
				expander/expander_utils.c \
				executor/executor.c \
				executor/execute_simple_cmd.c \
				executor/execute_pipeline.c \
				executor/redirect.c \
				executor/find_path.c \
				builtins/builtin_cd.c \
				builtins/builtin_echo.c \
				builtins/builtin_env.c \
				builtins/builtin_exit.c \
				builtins/builtin_export.c \
				builtins/builtin_pwd.c \
				builtins/builtin_unset.c \
				builtins/builtins.c \
				memory/memory.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRCS_FILES))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Libft
LIBFT = $(LIBFT_DIR)/libft.a

# Includes
INCS = -I$(INC_DIR) -I$(LIBFT_DIR)

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re