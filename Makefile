NAME = cub3d

LIBS = -lm -L./includes/minilibx-linux -lmlx -lX11 -lXext

all: $(NAME)

$(NAME):
	make -C includes/minilibx-linux
	gcc src/sound/sound.c src/main.c includes/libao.so.4.1.0 $(LIBS) -Iincludes -o $(NAME)

run: $(NAME)
	./$(NAME)

vrun: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

fclean:
	# make -C includes/minilibx-linux clean
	rm cub3d

re:	fclean all