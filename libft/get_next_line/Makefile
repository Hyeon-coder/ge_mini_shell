# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/28 16:00:00 by JuHyeon           #+#    #+#              #
#    Updated: 2024/02/28 16:00:00 by JuHyeon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = get_next_line.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

SRCS = \
		get_next_line.c \
		get_next_line_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@echo "$(NAME) created"
	@ranlib $(NAME)
	@echo "$(NAME) indexed"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@echo "OBJS deleted"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(NAME) deleted"

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: $(OBJS) 