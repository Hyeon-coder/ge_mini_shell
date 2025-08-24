# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/12 23:58:00 by JuHyeon           #+#    #+#              #
#    Updated: 2025/08/24 19:43:35 by juhyeonl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -I.
AR = ar rcs
RM = rm -f

# Libft Core Sources
SRC_FILES = \
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

# GNL Sources
GNL_DIR = get_next_line
GNL_FILES = get_next_line.c get_next_line_utils.c
GNL_SRCS = $(addprefix $(GNL_DIR)/, $(GNL_FILES))

# Printf Sources
PRINTF_DIR = printf
PRINTF_FILES = ft_printf.c ft_putchar_rt.c ft_putstr_rt.c ft_strchr.c ft_strlen.c \
               ft_itoa.c ft_itoa_base.c ft_form_check.c ft_putpointer.c
PRINTF_SRCS = $(addprefix $(PRINTF_DIR)/, $(PRINTF_FILES))

# All sources and objects
SRCS = $(SRC_FILES) $(GNL_SRCS) $(PRINTF_SRCS)
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS)