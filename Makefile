NAME := wolf3d

FLAGS := -Wall -Werror -Wextra

LIBS := libs/minilibx_macos_10.11/libmlx.a\
       libs/libft/libft.a

SRC_FILES := draw_image.c		\
			minimap.c			\
			ray_cast.c			\
			menu.c				\
			event_hooks.c		\
			main.c				\
			wolf.c				\
			movement.c			\
			read.c

SRCS := $(addprefix srcs/, $(SRC_FILES))

OBJS := $(SRCS:.c=.o)

INC := includes/

FRAMEWORKS := -framework OpenGL -framework AppKit

.PHONY := all clean fclean re

all: $(NAME)

$(NAME):
	@echo "compiling libft..."
	@make -C libs/libft/
	@echo "compiling minilibx..."
	@make -C libs/minilibx_macos_10.11/
	@gcc $(FLAGS) $(SRCS) -I$(INC) $(LIBS) $(FRAMEWORKS) -o $(NAME)
	@echo "Done!"

clean:
	@echo "deleting files..."
	@/bin/rm -f rm $(OFILES)
	@make -C libs/libft/ clean
	@make -C libs/minilibx_macos_10.11/ clean

fclean: clean
	@/bin/rm -f rm $(NAME)
	@make -C libs/libft/ fclean
	@make -C libs/minilibx_macos_10.11/ clean

new:
	@/bin/rm -f rm $(NAME)
	@gcc $(FLAGS) $(SRCS) -I$(INC) $(LIBS) $(FRAMEWORKS) -o $(NAME)
	@echo "Done!"

debug:
	@/bin/rm -f rm $@
	@gcc -g $(FLAGS) $(SRCS) -I$(INC) $(LIBS) $(FRAMEWORKS) -o $@

re: fclean all
