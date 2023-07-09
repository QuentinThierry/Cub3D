NAME = cub3d
NAME_BONUS = cub3d_bonus

all: $(NAME)

$(NAME):
	make -C Cub3d all
	cp ./Cub3d/$(NAME) .

bonus: $(NAME_BONUS)

$(NAME_BONUS):
	make -C Cub3d_bonus all
	cp ./Cub3d_bonus/$(NAME_BONUS) .

clean:
	make -C Cub3d clean
	make -C Cub3d_bonus clean

fclean:
	make -C Cub3d fclean
	rm -f $(NAME)
	make -C Cub3d_bonus fclean
	rm -f $(NAME_BONUS)

.PHONY:
	all bonus clean fclean
