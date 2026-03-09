CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
APP = mesoShell

SRC_DIR = src
OBJ_DIR = obj

# Lista todos os .c em src/ e transforma em .o em obj/
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Alvo principal
all: $(APP)

# Linkagem final
$(APP): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compilação de .c 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(APP)
