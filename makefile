CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -O3 -flto

export MOCL_DIR = libs/myOwnCLib
export CRYPTO_DIR = libs/cryptography

# 1. Déclarer les besoins spécifiques
MOCL_USE_DICTS = 1
MOCL_USE_VECTORS = 1

# 2. Inclusion des modules
include $(CRYPTO_DIR)/crypto.mk
include $(MOCL_DIR)/myOwnCLib.mk

# 3. Fusions
INCLUDES = -Isrc $(MOCL_INCLUDES) $(CRYPTO_INCLUDES)
SRCS = src/main.c $(MOCL_SRCS) $(CRYPTO_SRCS)

OBJS = $(SRCS:.c=.o)

# 4. Édition de liens finale (création de l'exécutable)
main: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

# 5. Règle de compilation de chaque fichier source (.c -> .o)
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Optionnel : Règle pour nettoyer
clean:
	rm -f main $(OBJS)