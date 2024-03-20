MTX_DIR := ./entradas_alg5
OUTPUT_DIR := ./saidas_alg5
# Lista de arquivos .mtx
MTX_FILES := $(wildcard $(MTX_DIR)/*.mtx)

all: compile

compile:
	g++ -std=c++11 reverseDelete.cpp -o reverseDelete.bin

clean:
	rm -f reverseDelete.bin

# Comando para executar o programa para cada arquivo .mtx
run: compile
	for file in $(MTX_FILES); do \
        output_file=$(OUTPUT_DIR)/$$(basename $$file); \
        ./reverseDelete.bin -f $$file -o $$output_file; \
		echo "$$file calculado, salvo em $$output_file"; \
    done
