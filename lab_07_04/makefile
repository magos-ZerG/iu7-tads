SHELL := /bin/bash

SRC_DIR := ./src
INC_DIR := ./inc
OUT_DIR := ./out

CC := gcc
CFLAGS := -c -std=gnu99 -Wall -Wpedantic -Wextra -Wfloat-conversion -Wfloat-equal -Wvla -I$(INC_DIR)
LDFLAGS := -lm

SRCS := $(wildcard $(SRC_DIR)/*.c)
MAIN_OBJ := $(OUT_DIR)/main.o
OBJS := $(filter-out $(MAIN_OBJ), $(patsubst $(SRC_DIR)/%.c, $(OUT_DIR)/%.o, $(SRCS)))

app.exe: $(OBJS) $(MAIN_OBJ) | out_dir
	$(CC) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/%.o : $(SRC_DIR)/%.c | out_dir
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: out_dir debug clang_everything release clean clean_obj

out_dir:
	mkdir -p $(OUT_DIR)

debug : CFLAGS += -g3 -O0 
debug : app.exe

clang_everything : CC = clang
clang_everything : CFLAGS := -c -std=gnu99 -Weverything -Wno-declaration-after-statement -Wno-unsafe-buffer-usage -I$(INC_DIR)
clang_everything : app.exe

release : CFLAGS += -DNDEBUG -g0 -O2 
release : app.exe

clean:
	@rm -f ./*.exe
	@rm -rf $(OUT_DIR)

clean_obj:
	@rm -f $(OUT_DIR)/*.o
