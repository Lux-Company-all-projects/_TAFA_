NAME = Tafa

CC = gcc
CFLAGS = -Wall -Wextra #-Werror
LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

SRCS = raylib_game.c screen_ending.c screen_gameplay.c \
	screen_logo.c screen_options.c screen_title.c \
	player.c raylib_utils.c

OBJS = $(SRCS:%.c=%.o)

GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)Compilation de $(NAME)...$(NC)"
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "$(GREEN)Compilation terminée avec succès !$(NC)"

$(%.o): $(%.c)
	@echo "$(BLUE)Compilation de $<...$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Nettoyage des fichiers objets...$(NC)"
	rm -rf $(OBJS)
	@echo "$(GREEN)Nettoyage terminée$(NC)"
	
fclean: clean
	@echo "$(RED)Suppresion de l'exécutable...$(NC)"
	rm -f $(NAME)
	@echo "$(GREEN)Nettoyage complet terminée$(NC)"

re: fclean all

run: all
	./$(NAME)

.PHONY: all clean fclean re run
