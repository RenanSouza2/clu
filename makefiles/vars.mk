PRJ_NAME = clu

# Immediate (:=) so git runs once per make rather than on every expansion, and
# unconditional (not ?=) so an inherited PRJ_DIR from a parent project's
# sub-make cannot win and silently point at the wrong project.
PRJ_DIR := $(shell git rev-parse --show-toplevel)

DIR := $(patsubst $(PRJ_DIR)/%,%,$(CURDIR))

LIB_DIR = $(PRJ_DIR)/lib
BIN_DIR = $(PRJ_DIR)/bin
SRC_DIR = $(PRJ_DIR)/src

LIB_FILE = $(LIB_DIR)/lib.o
DBG_FILE = $(LIB_DIR)/debug.o

SRC_FILE = $(SRC_DIR)/code.o
CLU_FILE = $(BIN_DIR)/clu.o
