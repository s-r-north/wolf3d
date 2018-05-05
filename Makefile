NAME := wolf3d

FLAGS := -Wall -Werror -Wextra

LIBS := libs/minilibx_macos_10.11/libmlx.a\
       libs/libft/libft.a

SDIR := srcs

ODIR := obj

FILES := draw_image		\
			minimap		\
			ray_cast	\
			menu		\
			event_hooks	\
			main		\
			wolf		\
			movement	\
			read

SRCS := $(addprefix $(SDIR)/, $(FILES))

OFILES := $(addprefix $(ODIR)/, $(FILES))
OBJS := $(addsuffix .o, $(OFILES))

INC := includes/

FRAMEWORKS := -framework OpenGL -framework AppKit

$(ODIR)/%.o: $(SDIR)/%.c
	@$(CC) $(CFLAGS) -I $(INC) -c $^ -o $@
	@/bin/echo -n "*"

.PHONY := all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	@echo "compiling libft..."
	@make -C libs/libft/
	@echo "compiling minilibx..."
	@make -C libs/minilibx_macos_10.11/
	@gcc $(FLAGS) $(OBJS) $(LIBS) -I$(INC) $(FRAMEWORKS) -o $(NAME)
	@echo "Done!"

$(OBJS): | $(ODIR)

$(ODIR):
	@mkdir -p $(ODIR)

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
