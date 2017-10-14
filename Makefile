#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: banthony <banthony@students.42.fr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/23 16:22:07 by banthony          #+#    #+#              #
#    Updated: 2017/10/14 18:47:22 by banthony         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = ft_nm

PATH_SRC = ./src/

PATH_HEAD = ./include/

HEADER_FILE = ft_nm.h	\

SRC_FILE = 	main.c	parsing.c	nm.c	misc.c	\

SRC = $(SRC_FILE:%c=$(PATH_SRC)%c)

INCLUDE = $(HEADER_FILE:%h=$(PATH_HEAD)%h)

OBJ = $(SRC_FILE:.c=.o)

OBJ2 = $(OBJ:%.o=$(PATH_SRC)%.o)

PATH_LIBFT = ~/libft/

HEAD_DIR = -I ./include -I $(PATH_LIBFT)

LIB = -L $(PATH_LIBFT) -lft

FLAGS = -Wall -Wextra -Werror -g3 -Weverything

all: $(NAME)

$(NAME): $(SRC) $(INCLUDE)
	make -C $(PATH_LIBFT)
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC)
	mv $(OBJ) $(PATH_SRC)
	gcc -o $(NAME) $(OBJ2) $(HEAD_DIR) $(LIB) $(FLAGS)

sanit: $(SRC) $(INCLUDE)
	make -C $(PATH_LIBFT) sanit
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC) -fsanitize=address
	mv $(OBJ) $(PATH_SRC)
	gcc -o $(NAME) $(OBJ2) $(HEAD_DIR) $(LIB) $(FLAGS) -fsanitize=address

clean:
	make clean -C $(PATH_LIBFT)
	rm -f $(OBJ2)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(PATH_LIBFT)
	rm Makefile~ ./*.c~ ./*.h~

re: fclean all