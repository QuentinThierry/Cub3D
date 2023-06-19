# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/06/19 18:53:33 by jvigny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

CC = gcc
CFLAGS = -g -Wall -Wextra #-Werror
LIBS = -lm -L$(MINILIBX_DIR) -lmlx -lX11 -lXext -L$(LIBAO_LIB) -lao
INCLUDES = -I$(MINILIBX_HEADERS) -I$(LIBAO_HEADERS) -I$(HEADERS_DIR)

REDIRECT_ERROR = >/dev/null 2>&1

MINILIBX_DIR = $(HEADERS_DIR)minilibx-linux/
MINILIBX_HEADERS = $(MINILIBX_DIR)
MINILIBX = $(MINILIBX_DIR)libmlx.a

LIBAO_DIR = $(HEADERS_DIR)libao/
LIBAO_HEADERS = $(LIBAO_DIR)include/ao/
LIBAO_LIB = $(LIBAO_DIR)lib/
LIBAO_TAR = $(HEADERS_DIR)libao.tar
LIBAO_ABS_PATH = $(shell pwd)/$(LIBAO_DIR)
LIBAO = $(LIBAO_LIB)/libao.so
LIBAO_SRC = libao-1.2.0/

HEADERS_LIST = cub3d.h get_next_line.h
HEADERS_DIR = ./includes/
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))

SOUND = sound/
SRC_SOUND = \
			sound.c

GNL = get_next_line/
SRC_GNL = get_next_line.c\
			get_next_line_utils.c

SRC_LIST =	$(addprefix $(SOUND), $(SRC_SOUND)) \
			$(addprefix $(GNL), $(SRC_GNL)) \
			key_hook.c \
			raycasting.c \
			draw.c \
			parsing.c \
			ft_calloc.c \
			init.c \
			main.c

SRC_DIR = ./src/
SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))

OBJ_DIR = ./obj/
OBJ_LIST = $(patsubst %.c, %.o, $(SRC_LIST))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))

# export LD_LIBRARY_PATH=$(LIBAO_ABS_PATH)lib

all:	$(NAME)

bonus:	all

run: $(NAME)
	./$(NAME)

# vrun: $(NAME)
# 	valgrind --leak-check=full --track-fds=yes --trace-children=yes --show-leak-kinds=all --track-origins=yes ./$(NAME)

vrun: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)


$(NAME):	$(LIBAO) $(MINILIBX) $(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) -Wl,-rpath,$(LIBAO_ABS_PATH)lib $(OBJ) $(LIBS) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $(INCLUDES) $(LIBS) $< -o $@
	
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)$(SOUND)
	mkdir -p $(OBJ_DIR)$(GNL)

$(LIBAO):
	curl -L http://downloads.xiph.org/releases/ao/libao-1.2.0.tar.gz --output $(LIBAO_TAR) $(REDIRECT_ERROR)
	tar -xf $(LIBAO_TAR) $(REDIRECT_ERROR)
	rm -rf $(LIBAO_TAR) $(REDIRECT_ERROR)
	mkdir -p $(LIBAO_ABS_PATH)
	cd $(LIBAO_SRC) && ./configure --prefix=$(LIBAO_ABS_PATH) --exec-prefix=$(LIBAO_ABS_PATH) $(REDIRECT_ERROR)
	make -C $(LIBAO_SRC) $(REDIRECT_ERROR)
	make -C $(LIBAO_SRC) install $(REDIRECT_ERROR)
	rm -rf $(LIBAO_SRC)

$(MINILIBX):
	make -C $(MINILIBX_DIR) all

clean:
	make -C $(MINILIBX_DIR) clean
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)
	rm -rf $(LIBAO_DIR)

re: fclean all

.PHONY : all clean fclean re
