CC = gcc
CFLAGS = -std=gnu17 -Wall -Wextra -O3 -flto

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = libs

# Inclusion des makefiles des sous-modules
include $(LIB_DIR)/myOwnCLib/myOwnCLib.mk
include $(LIB_DIR)/Cryptography/crypto.mk

# Fichiers sources partagés (communs au client et au serveur)
COMMON_SRCS = $(wildcard $(SRC_DIR)/common/*.c)

# Fichiers sources spécifiques
SERVER_SRCS = $(wildcard $(SRC_DIR)/server/*.c)
CLIENT_SRCS = $(wildcard $(SRC_DIR)/client/*.c)

# Fichiers objets correspondants
COMMON_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(COMMON_SRCS:.c=.o))
SERVER_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SERVER_SRCS:.c=.o))
CLIENT_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(CLIENT_SRCS:.c=.o))

# Objets tiers issus de tes sous-modules (via les fichiers .mk)
LIB_OBJS = $(MYOWNCLIB_OBJS) $(CRYPTO_OBJS)

# Noms des exécutables cibles
SERVER_EXEC = server.out
CLIENT_EXEC = client.out

# Drapeau d'inclusion pour les structures de tes libs
INCLUDES = -I$(SRC_DIR) -I$(LIB_DIR)

# Règle principale : compile les deux binaires
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Règle de liaison pour le Serveur
$(SERVER_EXEC): $(SERVER_OBJS) $(COMMON_OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Règle de liaison pour le Client
$(CLIENT_EXEC): $(CLIENT_OBJS) $(COMMON_OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Règle générique pour la compilation des fichiers objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Nettoyage des exécutables et du répertoire build
clean:
	rm -rf $(BUILD_DIR) $(SERVER_EXEC) $(CLIENT_EXEC)

.PHONY: all clean