# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/07/06 01:07:15 by jvigny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

CC = gcc
CFLAGS += -g -Wall -Wextra# -03#-Werror
LIBS = -lm -L$(MINILIBX_DIR) -lmlx -lX11 -lXext -L$(LIBAO_LIB) -lao -lpthread
INCLUDES = -I$(MINILIBX_HEADERS) -I$(LIBAO_HEADERS) -I$(HEADERS_DIR)

REDIRECT_ERROR = >/dev/null 2>&1

MINILIBX_URL = https://github.com/42Paris/minilibx-linux.git
MINILIBX_DIR = $(HEADERS_DIR)minilibx-linux/
MINILIBX_HEADERS = $(MINILIBX_DIR)
MINILIBX = $(MINILIBX_DIR)libmlx.a

LIBAO_URL = http://downloads.xiph.org/releases/ao/libao-1.2.0.tar.gz
LIBAO_DIR = $(HEADERS_DIR)libao/
LIBAO_HEADERS = $(LIBAO_DIR)include/ao/
LIBAO_LIB = $(LIBAO_DIR)lib/
LIBAO_TAR = $(HEADERS_DIR)libao.tar
LIBAO_ABS_PATH = $(shell pwd)/$(LIBAO_DIR)
LIBAO = $(LIBAO_LIB)/libao.so
LIBAO_SRC = libao-1.2.0/

RESET_COLOR	:= \033[0m
RED			:= \033[0;31m
GREEN		:= \033[0;32m


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
			utils.c \
			raycasting.c \
			draw.c \
			parsing.c \
			ft_calloc.c \
			init.c \
			move.c \
			view_2d.c \
			ft_memcpy.c \
			ft_close.c \
			wall_hit.c \
			main.c

SRC_DIR = ./src/
SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))

OBJ_DIR = ./obj/
OBJ_LIST = $(patsubst %.c, %.o, $(SRC_LIST))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))

all:	$(NAME)

bonus:	all

run: $(NAME)
	./$(NAME)

# vrun: $(NAME)
# 	valgrind --leak-check=full --track-fds=yes --trace-children=yes --show-leak-kinds=all --track-origins=yes ./$(NAME)

vrun: $(NAME)
	valgrind ./$(NAME)

prun: CFLAGS += -pg
prun: $(NAME)
	./$(NAME)
	gprof cub3d gmon.out > analysis.txt

$(NAME):	$(LIBAO) $(MINILIBX) $(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) -Wl,-rpath,$(LIBAO_ABS_PATH)lib $(OBJ) $(LIBS) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@
	
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(SOUND)
	@mkdir -p $(OBJ_DIR)$(GNL)

$(LIBAO):
	@echo "${GREEN}~-~-~-~-~ COMPILING LIBAO ~-~-~-~-${RESET_COLOR}"
	@echo "   ${GREEN}- Fetching sources...${RESET_COLOR}"
	@curl -L $(LIBAO_URL) --output $(LIBAO_TAR) $(REDIRECT_ERROR)
	@echo "   ${GREEN}- Decompressing...${RESET_COLOR}"
	@tar -xf $(LIBAO_TAR) $(REDIRECT_ERROR)
	@rm -rf $(LIBAO_TAR) $(REDIRECT_ERROR)
	@mkdir -p $(LIBAO_ABS_PATH)
	@echo "   ${GREEN}- Compiling sources...${RESET_COLOR}"
	@cd $(LIBAO_SRC) && ./configure --prefix=$(LIBAO_ABS_PATH) --exec-prefix=$(LIBAO_ABS_PATH) $(REDIRECT_ERROR)
	@make -C $(LIBAO_SRC) $(REDIRECT_ERROR)
	@make -C $(LIBAO_SRC) install $(REDIRECT_ERROR)
	@echo "   ${GREEN}- Cleaning directory...${RESET_COLOR}"
	@rm -rf $(LIBAO_SRC)
	@echo "${GREEN}~ DONE ~\n${RESET_COLOR}"

$(MINILIBX):
	@echo "${GREEN}~-~-~-~-~ COMPILING MINILIBX ~-~-~-~-~${RESET_COLOR}"
	@echo "   ${GREEN}- Fetching sources...${RESET_COLOR}"
	@if [ ! -d "$(MINILIBX_DIR)" ]; then \
		git clone $(MINILIBX_URL) $(MINILIBX_DIR); \
	else \
		cd $(MINILIBX_DIR) && git pull; \
	fi
	@echo "   ${GREEN}- Compiling sources...${RESET_COLOR}"
	@make -s -C $(MINILIBX_DIR) all $(REDIRECT_ERROR)
	@echo "${GREEN}~ DONE ~\n${RESET_COLOR}"

clean_libao:
	rm -rf $(LIBAO_TAR)
	rm -rf ./$(LIBAO_SRC)

clean_minilibx:
	rm -rf $(MINILIBX_DIR)

clean: clean_libao
	rm -rf $(OBJ_DIR)

fclean:	clean clean_minilibx
	rm -rf $(NAME)
	rm -rf $(LIBAO_DIR)

re: fclean all

.PHONY : all clean fclean re
