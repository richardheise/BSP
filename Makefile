# Nome do compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -Wextra -O2

# Nome do executável
TARGET = bsp

# Arquivos fonte
SRCS = main.cpp

# Arquivos de cabeçalho
HDRS = bsp.hpp

# Regra padrão
all: $(TARGET)

# Como compilar o executável
$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Limpa os arquivos gerados
clean:
	rm -f $(TARGET)

