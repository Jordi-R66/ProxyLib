CC = gcc
CFLAGS = -std=gnu17 -Wall -Wextra -O3 -flto
LDFLAGS = -flto

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = libs
CRYPTO_DIR = $(LIB_DIR)/Cryptography
MOCL_DIR = $(LIB_DIR)/myOwnCLib

CRYPTO_ABS_PATH = $(abspath $(CRYPTO_DIR))
MOCL_ABS_PATH = $(abspath $(MOCL_DIR))

INCLUDES = -I$(SRC_DIR) -I$(CRYPTO_DIR)/src/common -I$(MOCL_DIR)

LDLIBS = -L$(CRYPTO_DIR)/build -lcryptography -L$(MOCL_DIR)/build -lmyownclib -lpthread

COMMON_SRCS = $(wildcard $(SRC_DIR)/common/*.c)
SERVER_SRCS = $(wildcard $(SRC_DIR)/server/*.c)
CLIENT_SRCS = $(wildcard $(SRC_DIR)/client/*.c)

COMMON_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(COMMON_SRCS:.c=.o))
SERVER_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SERVER_SRCS:.c=.o))
CLIENT_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(CLIENT_SRCS:.c=.o))

SERVER_EXEC = server.out
CLIENT_EXEC = client.out

.PHONY: all clean crypto_lib mocl_lib

all: mocl_lib crypto_lib $(SERVER_EXEC) $(CLIENT_EXEC)

mocl_lib:
	$(MAKE) -C $(MOCL_ABS_PATH) lib MODE=prod

crypto_lib: mocl_lib
	$(MAKE) -C $(CRYPTO_ABS_PATH) lib MOCL_DIR=$(MOCL_ABS_PATH) MODE=prod

$(SERVER_EXEC): $(SERVER_OBJS) $(COMMON_OBJS) crypto_lib mocl_lib
	$(CC) $(CFLAGS) $(LDFLAGS) $(SERVER_OBJS) $(COMMON_OBJS) -o $@ $(LDLIBS)

$(CLIENT_EXEC): $(CLIENT_OBJS) $(COMMON_OBJS) crypto_lib mocl_lib
	$(CC) $(CFLAGS) $(LDFLAGS) $(CLIENT_OBJS) $(COMMON_OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(SERVER_EXEC) $(CLIENT_EXEC)
	$(MAKE) -C $(CRYPTO_ABS_PATH) clean
	$(MAKE) -C $(MOCL_ABS_PATH) clean