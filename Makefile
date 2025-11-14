# --- Variáveis do Compilador ---
CXX = g++
CC = gcc
EXEC = boids_app

# --- Pastas ---
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
OBJ_DIR = obj

# --- Arquivos ---
CXX_SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
C_SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
CXX_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CXX_SRCS))
C_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SRCS))
OBJS = $(CXX_OBJS) $(C_OBJS)

# --- Flags de Compilação ---
CXXFLAGS = -std=c++17 -Wall -Wextra -g
CFLAGS = -Wall -g

# --- Flags do Preprocessador (Includes) ---
# Essencial para linkagem estática
CPPFLAGS = -DGLFW_STATIC \
           -I"$(INCLUDE_DIR)" \
           -I"$(LIB_DIR)/glad/include" \
           -I"$(LIB_DIR)/glfw/include" \
           -I"$(LIB_DIR)/glm"

# --- Flags do Linker (Bibliotecas) ---
# Apontando para a pasta 64-bit
LDFLAGS = -L"$(LIB_DIR)/glfw/lib-mingw-w64"

# Lista de bibliotecas limpa e correta
LIBS = -lglfw3 -lopengl32 -lglu32 -lgdi32 -luser32 -lkernel32 -lshell32 -lm

# --- Regras (Rules) ---

.PHONY: all
all: $(EXEC)

$(EXEC): $(OBJS)
	@echo "Linkando executável..."
	$(CXX) $(OBJS) -o $(EXEC) $(LDFLAGS) $(LIBS)
	@echo "Pronto! Execute com: ./$(EXEC).exe"

# Regra genérica C++
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando $<..."
	@-mkdir $(subst /,\,$(dir $@))
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Regra genérica C
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compilando $<..."
	@-mkdir $(subst /,\,$(dir $@))
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# --- CORREÇÃO AQUI ---
# Regra de limpeza compatível com Windows (rmdir/del)
.PHONY: clean
clean:
	@echo "Limpando..."
	@if exist $(EXEC).exe ( del $(EXEC).exe )
	@if exist $(EXEC) ( del $(EXEC) )
	@if exist $(OBJ_DIR) ( rmdir /s /q $(OBJ_DIR) )