# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/08/27 20:33:02 by jvigny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d_bonus

CC = gcc
CFLAGS = -g -Wall -Wextra #-O3#-fsanitize=address#-Werror
LIBS = -Wl,-rpath,$(LIBAO_ABS_PATH)lib -lm -L$(MINILIBX_DIR) -lmlx -lX11 -lXext #-L$(LIBAO_LIB) -lao -lpthread
INCLUDES = -I$(MINILIBX_HEADERS) -I$(HEADERS_DIR) #-I$(LIBAO_HEADERS)

REDIRECT_ERROR = >/dev/null 2>&1

MINILIBX_URL = https://github.com/42Paris/minilibx-linux.git
MINILIBX_DIR = $(HEADERS_DIR)minilibx-linux/
MINILIBX_HEADERS = $(MINILIBX_DIR)
MINILIBX = $(MINILIBX_DIR)libmlx.a

RESET_COLOR	:= \033[0m
RED			:= \033[0;31m
GREEN		:= \033[0;32m

HEADERS_LIST = cub3d_bonus.h get_next_line.h
HEADERS_DIR = ./includes/
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))

LIBAO_URL = http://downloads.xiph.org/releases/ao/libao-1.2.0.tar.gz
LIBAO_DIR = $(HEADERS_DIR)libao/
LIBAO_HEADERS = $(LIBAO_DIR)include/ao/
LIBAO_LIB = $(LIBAO_DIR)lib/
LIBAO_TAR = $(HEADERS_DIR)libao.tar
LIBAO_ABS_PATH = $(shell pwd)/$(LIBAO_DIR)
LIBAO = $(LIBAO_LIB)/libao.so
LIBAO_SRC = libao-1.2.0/

SOUND = sound/
SRC_SOUND = \
			sound.c

GNL = get_next_line/
SRC_GNL = get_next_line.c \
			get_next_line_utils.c

UTILS = utils/
SRC_UTILS = ft_strjoin.c \
			ft_strncmp.c \
			ft_atoi.c \
			utils.c \
			ft_calloc.c \
			ft_realloc.c \
			ft_strdup.c \
			ft_memcpy.c \
			free.c \
			bettermlx.c

PARSING = parsing/
SRC_PARSING = check_map.c \
			ft_fill_wall.c \
			fill_texture.c \
			init.c \
			parsing.c \
			parse_texture.c

RAYCASTING = raycasting/
SRC_RAYCASTING = raycasting.c \
			collisions.c \
			wall_hit.c \
			door.c \
			floor.c \
			object.c \
			ray_object.c

MINIMAP = minimap/
SRC_MINIMAP = minimap.c

DRAW = draw/
SRC_DRAW = 	draw.c \
			image_operations.c \
			get_image.c

HOOKS = hooks/
SRC_HOOKS = key_hook.c \
			ft_close.c 

SRC_LIST =	$(addprefix $(GNL), $(SRC_GNL)) \
			$(addprefix $(UTILS), $(SRC_UTILS)) \
			$(addprefix $(PARSING), $(SRC_PARSING)) \
			$(addprefix $(RAYCASTING), $(SRC_RAYCASTING)) \
			$(addprefix $(MINIMAP), $(SRC_MINIMAP)) \
			$(addprefix $(DRAW), $(SRC_DRAW)) \
			$(addprefix $(HOOKS), $(SRC_HOOKS)) \
			print.c \
			main.c

#			$(addprefix $(SOUND), $(SRC_SOUND)) \

SRC_DIR = ./src/
SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))

OBJ_DIR = ./obj/
OBJ_LIST = $(patsubst %.c, %.o, $(SRC_LIST))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))

all:	$(NAME)

bonus:	all

run: $(NAME)
	./$(NAME) maps/map_test.cub

# vrun: $(NAME)
# 	valgrind --leak-check=full --track-fds=yes --trace-children=yes --show-leak-kinds=all --track-origins=yes ./$(NAME)

vrun: $(NAME)
	valgrind --leak-check=full ./$(NAME) maps/map_test.cub

prun: CFLAGS += -pg
prun: $(NAME)
	./$(NAME) maps/map_test.cub
	gprof $(NAME) gmon.out > analysis.txt

$(NAME):	$(MINILIBX) $(OBJ_DIR) $(OBJ) #$(LIBAO)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@
	
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(GNL)
	@mkdir -p $(OBJ_DIR)$(SOUND)
	@mkdir -p $(OBJ_DIR)$(RAYCASTING)
	@mkdir -p $(OBJ_DIR)$(DRAW)
	@mkdir -p $(OBJ_DIR)$(HOOKS)
	@mkdir -p $(OBJ_DIR)$(UTILS)
	@mkdir -p $(OBJ_DIR)$(MINIMAP)
	@mkdir -p $(OBJ_DIR)$(PARSING)

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

clean_minilibx:
	rm -rf $(MINILIBX_DIR)

clean_libao:
	rm -rf $(LIBAO_TAR)
	rm -rf ./$(LIBAO_SRC)
	rm -rf $(LIBAO_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean clean_minilibx clean_libao
	rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re run vrun prun
