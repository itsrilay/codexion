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
	$(SRC_DIR)get_set.c \
	$(SRC_DIR)queue.c

# ==== OBJECTS ====
OBJ_DIR = obj/
OBJS = $(addprefix $(OBJ_DIR), $(SRC:%.c=%.o))

# ==== INCLUDES ====
INC = .
INC_FLAGS = -I$(INC)


# ==== DEFAULT ====
all: $(NAME)

# ==== NAME ====
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INC_FLAGS) $(OBJS) -o $(NAME)

# ==== OBJECT DIRECTORY ====
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# ==== COMPILE OBJECTS ====
obj/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# ==== CLEAN ====
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re