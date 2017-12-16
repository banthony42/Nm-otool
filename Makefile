#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: banthony <banthony@students.42.fr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/23 16:22:07 by banthony          #+#    #+#              #
#    Updated: 2017/12/16 16:45:31 by banthony         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME_NM = ft_nm
NAME_OTOOL = ft_otool

NAME_SANIT1 = ft_nms
NAME_SANIT2 = ft_otools

PATH_SRC = ./src/
PATH_HEAD = ./include/
PATH_LIBFT = ~/libft/

HEADER_FILE = ft_nm_otool.h	\

SRC_FILE_COMMON += ar.c
SRC_FILE_COMMON += fat_32.c
SRC_FILE_COMMON += ft_nm_master.c
SRC_FILE_COMMON += ft_otool_master.c
SRC_FILE_COMMON += list_manage.c
SRC_FILE_COMMON += macho_32cigam.c
SRC_FILE_COMMON += macho_32magic.c
SRC_FILE_COMMON += macho_64cigam.c
SRC_FILE_COMMON += macho_64magic.c
SRC_FILE_COMMON += misc.c
SRC_FILE_COMMON += misc2.c
SRC_FILE_COMMON += misc3.c
SRC_FILE_COMMON += misc4.c
SRC_FILE_COMMON += parsing.c
SRC_FILE_COMMON += section_text_handler.c
SRC_FILE_COMMON += symtype_handler32.c
SRC_FILE_COMMON += symtype_handler64.c

SRC_FILE_NM += ft_nm_main.c
SRC_FILE_OTOOL += ft_otool_main.c

SRC_COMMON = $(SRC_FILE_COMMON:%c=$(PATH_SRC)%c)
SRC_NM = $(SRC_FILE_NM:%c=$(PATH_SRC)%c)
SRC_OTOOL = $(SRC_FILE_OTOOL:%c=$(PATH_SRC)%c)

OBJ_FILE_COMMON = $(SRC_FILE_COMMON:.c=.o)
OBJ_FILE_NM = $(SRC_FILE_NM:.c=.o)
OBJ_FILE_OTOOL = $(SRC_FILE_OTOOL:.c=.o)

OBJ_COMMON = $(OBJ_FILE_COMMON:%.o=$(PATH_SRC)%.o)
OBJ_NM = $(OBJ_FILE_NM:%.o=$(PATH_SRC)%.o)
OBJ_OTOOL = $(OBJ_FILE_OTOOL:%.o=$(PATH_SRC)%.o)

INCLUDE = $(HEADER_FILE:%h=$(PATH_HEAD)%h)
LIB = -L $(PATH_LIBFT) -lft
LIB_SANIT = -L $(PATH_LIBFT) -lft_sanit

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
HEAD_DIR = -I $(PATH_HEAD) -I $(PATH_LIBFT) -I ./system_headers_sguillia/
HEAD_DIR += -I ./system_headers_sguillia/mach-for-linux
FLAGS = -Wall -Wextra -Werror -g3
endif

ifeq ($(UNAME), Darwin)
HEAD_DIR = -I $(PATH_HEAD) -I $(PATH_LIBFT)
FLAGS = -Wall -Wextra -Werror -g3 -Weverything
endif

%.o:%.c
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC_COMMON) -fsanitize=address
	mv $(OBJ_FILE_COMMON) $(PATH_SRC)

all: $(NAME_NM) $(NAME_OTOOL)

sanit: $(NAME_SANIT1) $(NAME_SANIT2)

$(NAME_NM): $(OBJ_COMMON) $(SRC_NM) $(INCLUDE)
	@make -C $(PATH_LIBFT)
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC_NM)
	mv $(OBJ_FILE_NM) $(PATH_SRC)
	gcc -o $(NAME_NM) $(OBJ_COMMON) $(OBJ_NM) $(HEAD_DIR) $(LIB) $(FLAGS)

$(NAME_OTOOL): $(OBJ_COMMON) $(SRC_OTOOL) $(INCLUDE)
	@make -C $(PATH_LIBFT)
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC_OTOOL)
	mv $(OBJ_FILE_OTOOL) $(PATH_SRC)
	gcc -o $(NAME_OTOOL) $(OBJ_COMMON) $(OBJ_OTOOL) $(HEAD_DIR) $(LIB) $(FLAGS)

$(NAME_SANIT1): $(OBJ_COMMON) $(SRC_NM) $(INCLUDE)
	@make -C $(PATH_LIBFT) sanit
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC_NM) -fsanitize=address
	mv $(OBJ_FILE_NM) $(PATH_SRC)
	gcc -o $(NAME_SANIT1) $(OBJ_COMMON) $(OBJ_NM) $(HEAD_DIR) $(LIB_SANIT) $(FLAGS) -fsanitize=address

$(NAME_SANIT2): $(OBJ_COMMON) $(SRC_OTOOL) $(INCLUDE)
	@make -C $(PATH_LIBFT) sanit
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC_OTOOL) -fsanitize=address
	mv $(OBJ_FILE_OTOOL) $(PATH_SRC)
	gcc -o $(NAME_SANIT2) $(OBJ_COMMON) $(OBJ_OTOOL) $(HEAD_DIR) $(LIB_SANIT) $(FLAGS) -fsanitize=address

clean:
	@make clean -C $(PATH_LIBFT)
	rm -f $(OBJ_NM) $(OBJ_OTOOL) $(OBJ_COMMON)

fclean: clean
	rm -f $(NAME_NM) $(NAME_SANIT1) $(NAME_OTOOL) $(NAME_SANIT2)
	@make fclean -C $(PATH_LIBFT)

re: fclean all