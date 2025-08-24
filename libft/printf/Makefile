# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/15 11:20:32 by JuHyeon           #+#    #+#              #
#    Updated: 2025/02/27 17:13:16 by JuHyeon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = printf.a

SRCS = \
	ft_printf.c \
	ft_putchar_rt.c \
	ft_putstr_rt.c \
	ft_strchr.c \
	ft_strlen.c \
	ft_itoa.c \
	ft_itoa_base.c \
	ft_form_check.c \
	ft_putpointer.c

OBJ = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar -rcs

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: $(OBJ)