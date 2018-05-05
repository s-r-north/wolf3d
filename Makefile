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
	@echo "•"

.PHONY := all clean fclean re confirm

all: $(NAME)

$(NAME): confirm

	@make -C libs/libft/
	@echo "compiling minilibx..."
	@make -C libs/minilibx_macos_10.11/
	@gcc $(FLAGS) $(OBJS) $(LIBS) -I$(INC) $(FRAMEWORKS) -o $(NAME)
	@echo "\033[32m[executable $(NAME) created ( ͡° ͜ʖ ͡°)]\033[0m"

confirm: $(OBJS)
	@echo "\033[32m[object files created ( ͡° ͜ʖ ͡°)]\033[0m"

$(OBJS): | $(ODIR)

$(ODIR):
	@mkdir -p $(ODIR)

clean:
	@/bin/rm -f rm $(OFILES)
	@echo "\033[31m[$(NAME) object files deleted (╯°□°）╯︵ ┻━┻ ]\033[0m"
	@make -C libs/libft/ clean
	@make -C libs/minilibx_macos_10.11/ clean

fclean: clean
	@/bin/rm -f rm $(NAME)
	@echo "\033[31m[executable $(NAME) deleted ᕙ(⇀‸↼‶)ᕗ ]\033[0m"
	@make -C libs/libft/ fclean
	@make -C libs/minilibx_macos_10.11/ clean

debug: $(OBJS)
	@/bin/rm -f rm $@
	@gcc -g $(FLAGS) $(SRCS) -I$(INC) $(LIBS) $(FRAMEWORKS) -o $@

re: fclean all
