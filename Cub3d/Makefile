# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/10/03 16:03:58 by jvigny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

CC = gcc
CFLAGS = -g -Wall -Wextra# -03#-Werror
LIBS = -lm -L$(MINILIBX_DIR) -lmlx -lX11 -lXext
INCLUDES = -I$(MINILIBX_HEADERS) -I$(HEADERS_DIR)

REDIRECT_ERROR = >/dev/null 2>&1

# ------------------MLX-------------------
MINILIBX_URL = https://github.com/42Paris/minilibx-linux.git
MINILIBX_DIR = $(HEADERS_DIR)minilibx-linux/
MINILIBX_HEADERS = $(MINILIBX_DIR)
MINILIBX = $(MINILIBX_DIR)libmlx.a
# ----------------------------------------

RESET_COLOR	:= \033[0m
RED			:= \033[0;31m
GREEN		:= \033[0;32m

HEADERS_LIST = cub3d.h get_next_line.h
HEADERS_DIR = ./includes/
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))

GNL = get_next_line/
SRC_GNL = get_next_line.c \
			get_next_line_utils.c

SRC_LIST =	$(addprefix $(GNL), $(SRC_GNL)) \
			key_hook.c \
			utils.c \
			raycasting.c \
			draw.c \
			parsing.c \
			ft_calloc.c \
			init.c \
			move.c \
			ft_memcpy.c \
			ft_close.c \
			wall_hit.c \
			check_map.c \
			bettermlx.c \
			main.c

SRC_DIR = ./src/
SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))

OBJ_DIR = ./obj/
OBJ_LIST = $(patsubst %.c, %.o, $(SRC_LIST))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))

all:	$(NAME)

bonus:	all

run: $(NAME)
	./$(NAME) maps/map.cub

vrun: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) maps/map.cub

prun: CFLAGS += -pg
prun: $(NAME)
	./$(NAME)
	gprof cub3d gmon.out > analysis.txt

$(NAME):	$(MINILIBX) $(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@
	
$(OBJ_DIR): Makefile
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(GNL)

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

clean_minilibx:
	rm -rf $(MINILIBX_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean clean_minilibx
	rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re clean_minilibx bonus run vrun prun
