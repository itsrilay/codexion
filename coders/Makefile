NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

# ==== SOURCES ====
SRC_DIR = ./
SRC = \
	$(SRC_DIR)main.c \
	$(SRC_DIR)parse.c \
	$(SRC_DIR)utils.c \
	$(SRC_DIR)sim.c \
	$(SRC_DIR)monitor.c \
	$(SRC_DIR)get_set.c

# ==== OBJECTS ====
OBJS = $(SRC:%.c=%.o)

# ==== INCLUDES ====
INC = .
INC_FLAGS = -I$(INC)


# ==== DEFAULT ====
all: $(NAME)

# ==== NAME ====
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INC_FLAGS) $(OBJS) -o $(NAME)

# ==== COMPILE OBJECTS ====
%o: %.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# ==== CLEAN ====
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re