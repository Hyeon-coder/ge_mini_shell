# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/12 23:58:00 by JuHyeon           #+#    #+#              #
#    Updated: 2025/08/24 11:46:04 by juhyeonl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

SRC = \
	char/ft_isalnum.c char/ft_isalpha.c char/ft_isascii.c char/ft_isdigit.c char/ft_isprint.c \
	char/ft_tolower.c char/ft_toupper.c \
	conversion/ft_atoi.c conversion/ft_itoa.c \
	linked_list/ft_lstadd_back.c linked_list/ft_lstclear.c linked_list/ft_lstiter.c \
	linked_list/ft_lstmap.c linked_list/ft_lstsize.c linked_list/ft_lstadd_front.c \
	linked_list/ft_lstdelone.c linked_list/ft_lstlast.c linked_list/ft_lstnew.c \
	memory/ft_bzero.c memory/ft_calloc.c memory/ft_memchr.c memory/ft_memcmp.c \
	memory/ft_memcpy.c memory/ft_memmove.c memory/ft_memset.c memory/ft_realloc.c \
	print/ft_error.c print/ft_putchar_fd.c print/ft_putendl_fd.c print/ft_putnbr_fd.c print/ft_putstr_fd.c \
	string/ft_split.c string/ft_strdup.c string/ft_strjoin.c string/ft_strlcpy.c \
	string/ft_strmapi.c string/ft_strnstr.c string/ft_strtrim.c string/ft_strchr.c \
	string/ft_striteri.c string/ft_strlcat.c string/ft_strlen.c string/ft_strncmp.c \
	string/ft_strrchr.c string/ft_substr.c string/ft_strcmp.c string/ft_strndup.c \
	string/ft_strcpy.c string/ft_strcat.c string/ft_strstr.c ft_strjoin_helpers.c \
	ft_isspace.c

OBJ = $(SRC:.c=.o)

PRINTF_DIR = printf
PRINTF_LIB = $(PRINTF_DIR)/printf.a

GNL_DIR = get_next_line
GNL_LIB = $(GNL_DIR)/get_next_line.a

all: $(NAME) submodules

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

submodules:
	$(MAKE) -C $(PRINTF_DIR)
	$(MAKE) -C $(GNL_DIR)

clean:
	$(RM) $(OBJ)
	$(MAKE) -C $(PRINTF_DIR) clean
	$(MAKE) -C $(GNL_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(PRINTF_DIR) fclean
	$(MAKE) -C $(GNL_DIR) fclean

re: fclean all

.PHONY: all clean fclean re submodules
.SECONDARY: $(OBJ)