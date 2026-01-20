# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmousli <mmousli@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/20 18:41:31 by mmousli           #+#    #+#              #
#    Updated: 2026/01/20 18:41:37 by mmousli          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_SERVER	:= server
NAME_CLIENT	:= client

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror
INCLUDES	:= -Iinclude

BUILD_DIR	:= build


SERVER_SRCS	:= \
	src/server.c \
	src/utils/ft_putnbr.c

CLIENT_SRCS	:= \
	src/client.c \
	src/utils/ft_putnbr.c


SERVER_OBJS	:= $(SERVER_SRCS:src/%.c=$(BUILD_DIR)/%.o)
CLIENT_OBJS	:= $(CLIENT_SRCS:src/%.c=$(BUILD_DIR)/%.o)

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $(NAME_SERVER)

$(NAME_CLIENT): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $(NAME_CLIENT)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME_SERVER) $(NAME_CLIENT)

re: fclean all

.PHONY: all clean fclean re
