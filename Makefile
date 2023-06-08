NAME = cub3d

all: $(NAME)

$(NAME):
	gcc src/sound/ao_example.c includes/libao.so.4.1.0 -lm -Iincludes -o $(NAME)

run: all
	./$(NAME)

vrun: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

fclean:
	rm cub3d

re:	fclean all