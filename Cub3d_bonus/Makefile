# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/12/06 13:29:12 by jvigny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = le_dernier_voyage

CC = gcc
CFLAGS = -Wall -Wextra -Werror -MMD
LIBS = -L$(MINILIBX_DIR) -lmlx -lX11 -lXext -lm  -L$(RAUDIO_SRC) -lraudio -ldl -pthread
INCLUDES = -I$(MINILIBX_HEADERS) -I$(HEADERS_DIR) -I$(RAUDIO_HEADERS)

REDIRECT_ERROR = >/dev/null 2>&1

RESET_COLOR	:= \033[0m
RED			:= \033[0;31m
GREEN		:= \033[0;32m

HEADERS_DIR = ./includes/

# ------------------MLX-------------------
MINILIBX_URL = https://github.com/42Paris/minilibx-linux.git
MINILIBX_DIR = $(HEADERS_DIR)minilibx-linux/
MINILIBX_HEADERS = $(MINILIBX_DIR)
MINILIBX = $(MINILIBX_DIR)libmlx.a
# ----------------------------------------

# ------------------RAUDIO-------------------
RAUDIO_URL = https://github.com/raysan5/raudio.git
RAUDIO_DIR = $(HEADERS_DIR)raudio/
RAUDIO_SRC = $(RAUDIO_DIR)src/
RAUDIO_HEADERS = $(RAUDIO_SRC)
RAUDIO = $(RAUDIO_SRC)/libraudio.a
# defines for the raudio compilation :
# - RAUDIO_STANDALONE/SUPPORT_MODULE_RAUDIO: compile raudio without all raylib; 
# - SUPPORT_FILEFORMAT_WAV/MP3: support wav format for raudio
# - 'TRACELOG(level, ...)': override log print function to remove unwanted logs
RAUDIO_DEFINE = -D RAUDIO_STANDALONE -D SUPPORT_MODULE_RAUDIO -D SUPPORT_FILEFORMAT_MP3 -D SUPPORT_FILEFORMAT_WAV -D 'TRACELOG(level, ...)'
#----------------------------------------------

GNL = get_next_line/
SRC_GNL = \
			get_next_line.c \
			get_next_line_utils.c

UTILS = utils/
SRC_UTILS = \
			ft_strjoin.c \
			ft_strncmp.c \
			ft_atoi.c \
			utils.c \
			ft_calloc.c \
			ft_realloc.c \
			ft_strdup.c \
			ft_strlen.c \
			ft_memcpy.c \
			ft_bzero.c \
			free.c \
			free_map.c \
			print_error.c \
			bettermlx_bilinear_resize.c \
			bettermlx.c \
			is_wall.c \
			check_type.c

PARSING = parsing/
SRC_PARSING = \
			get_filename.c \
			is_existing.c \
			read_dir.c \
			read_anim.c \
			check_map.c \
			sort_anim.c \
			ft_fill_wall.c \
			get_texture_wall.c \
			get_texture_wall_door.c \
			get_texture_none_wall.c \
			fill_texture.c \
			cmp_texture.c \
			init.c \
			init_mouse.c \
			load_image.c \
			load_image_tab.c \
			multiple_texture.c \
			parsing.c \
			parse_map.c \
			parse_texture.c \
			parse_music.c \
			load_subtitle.c \
			fill_object_door.c

RAYCASTING = raycasting/
SRC_RAYCASTING = \
			raycasting.c \
			collisions.c \
			collision_se.c \
			collision_sw.c \
			collision_ne.c \
			collision_nw.c \
			wall_hit_se.c \
			wall_hit_sw.c \
			wall_hit_ne.c \
			wall_hit_nw.c \
			wall_hit.c \
			door.c \
			door_raycasting.c \
			door_raycasting1.c \
			door_raycasting2.c \
			door_raycasting3.c \
			open_door.c \
			floor.c \
			object.c \
			object_compute.c \
			object_interactive.c \
			drop_object.c \
			ray_object1.c \
			ray_object.c

MINIMAP = minimap/
SRC_MINIMAP = \
			minimap.c \
			minimap_rect.c \
			minimap_rotate.c \
			minimap_init.c

DRAW = draw/
SRC_DRAW = \
			draw_wall_pixels.c \
			draw_wall.c \
			draw_hand_item.c \
			animation.c \
			draw_object.c \
			draw_floor.c \
			image_operations.c \
			get_sprite.c \
			get_image.c

HOOKS = hooks/
SRC_HOOKS = key_hook.c \
			move_player.c \
			mouse_hook.c \
			ft_close.c

MENU = menu/
SRC_MENU = \
			horizontal_blur.c \
			vertical_blur.c \
			menu_key_hooks.c \
			menu_mouse_hooks.c \
			menu_loop_hook.c \
			option_menu.c \
			init_menu.c \
			init_menu_images.c \
			draw_button.c \
			draw_slider.c \
			menu_utils.c \
			pause_menu.c

AUDIO = audio/
SRC_AUDIO = \
			audio.c \
			init_audio.c \
			update_audio.c \
			audio_utils.c \
			draw_subtitle.c \
			subtitle.c

LOADINGSCREEN = loading_screen/
SRC_LOADINGSCREEN = \
			init_loading_screen.c \
			loading_screen.c \
			draw_image.c

ENDGAME = end_game/
SRC_ENDGAME = \
			end_game.c \
			end_open_door.c \
			end_open_door1.c \
			wall_hit_end.c \
			wall_hit_end_ne.c \
			wall_hit_end_nw.c \
			wall_hit_end_se.c \
			wall_hit_end_sw.c \
			end_door.c \
			end_utils.c \
			end_game_raycaster.c

SRC_LIST =	$(addprefix $(GNL), $(SRC_GNL)) \
			$(addprefix $(UTILS), $(SRC_UTILS)) \
			$(addprefix $(PARSING), $(SRC_PARSING)) \
			$(addprefix $(RAYCASTING), $(SRC_RAYCASTING)) \
			$(addprefix $(MINIMAP), $(SRC_MINIMAP)) \
			$(addprefix $(DRAW), $(SRC_DRAW)) \
			$(addprefix $(HOOKS), $(SRC_HOOKS)) \
			$(addprefix $(MENU), $(SRC_MENU)) \
			$(addprefix $(AUDIO), $(SRC_AUDIO)) \
			$(addprefix $(ENDGAME), $(SRC_ENDGAME)) \
			$(addprefix $(LOADINGSCREEN), $(SRC_LOADINGSCREEN)) \
			main.c

SRC_DIR = ./src/
SRC = $(addprefix $(SRC_DIR),$(SRC_LIST))

OBJ_DIR = ./obj/
OBJ_LIST = $(patsubst %.c,%.o, $(SRC_LIST))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))
DEP_LIST = $(patsubst %.o,%.d, $(OBJ))

all:	$(NAME)

bonus:	all

run: $(NAME)
	./$(NAME) maps/map.cub

vrun: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=valgrind.supp ./$(NAME) maps/map.cub

-include $(DEP_LIST)

$(NAME):	$(MINILIBX) $(RAUDIO) $(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c Makefile
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

$(OBJ_DIR): Makefile
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(GNL)
	@mkdir -p $(OBJ_DIR)$(RAYCASTING)
	@mkdir -p $(OBJ_DIR)$(DRAW)
	@mkdir -p $(OBJ_DIR)$(HOOKS)
	@mkdir -p $(OBJ_DIR)$(UTILS)
	@mkdir -p $(OBJ_DIR)$(MINIMAP)
	@mkdir -p $(OBJ_DIR)$(PARSING)
	@mkdir -p $(OBJ_DIR)$(MENU)
	@mkdir -p $(OBJ_DIR)$(AUDIO)
	@mkdir -p $(OBJ_DIR)$(ENDGAME)
	@mkdir -p $(OBJ_DIR)$(LOADINGSCREEN)

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

$(RAUDIO):
	@echo "${GREEN}~-~-~-~-~ COMPILING RAUDIO ~-~-~-~-~${RESET_COLOR}"
	@echo "   ${GREEN}- Fetching sources...${RESET_COLOR}"
	@if [ ! -d "$(RAUDIO_DIR)" ]; then \
		git clone $(RAUDIO_URL) $(RAUDIO_DIR); \
	else \
		cd $(RAUDIO_DIR) && git pull; \
	fi
	@echo "   ${GREEN}- Compiling sources...${RESET_COLOR}"
	gcc -g -c $(RAUDIO_SRC)raudio.c $(RAUDIO_DEFINE) -o $(RAUDIO_SRC)raudio.o
	ar rcs $(RAUDIO) $(RAUDIO_SRC)raudio.o
	rm -f $(RAUDIO_SRC)raudio.o
	@echo "${GREEN}~ DONE ~\n${RESET_COLOR}"

clean_minilibx:
	rm -rf $(MINILIBX_DIR)

clean_audio:
	rm -rf $(RAUDIO_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean clean_minilibx clean_audio
	rm -rf $(NAME)

re: fclean all

.PHONY : all bonus clean fclean re run vrun clean_minilibx clean_audio
