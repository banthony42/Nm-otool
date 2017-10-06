#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: banthony <banthony@students.42.fr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/23 16:22:07 by banthony          #+#    #+#              #
#    Updated: 2017/10/06 20:10:49 by banthony         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = ft_nm

PATH_SRC = ./src/

PATH_HEAD = ./include/

HEADER_FILE = ft_nm.h	\

SRC_FILE = 	main.c		\

SRC = $(SRC_FILE:%c=$(PATH_SRC)%c)

INCLUDE = $(HEADER_FILE:%h=$(PATH_HEAD)%h)

OBJ = $(SRC_FILE:.c=.o)

OBJ2 = $(OBJ:%.o=$(PATH_SRC)%.o)

HEAD_DIR = -I ./include -I ./libft

PATH_LIBFT = ~/libft/

LIB = -L ~/libft -lft

FLAGS = -Wall -Wextra -Werror -g3 -Weverything -fsanitize=address

all: $(NAME)

$(NAME): $(SRC) $(INCLUDE)
	make -C $(PATH_LIBFT) sanit
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC)
	mv $(OBJ) $(PATH_SRC)
	gcc -o $(NAME) $(OBJ2) $(HEAD_DIR) $(LIB) $(FLAGS)

clean:
	make clean -C $(PATH_LIBFT)
	rm -f $(OBJ2)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(PATH_LIBFT)

re: fclean all