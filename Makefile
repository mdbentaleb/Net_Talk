CC = gcc
CFLAGS = -Wall -Wextra -Werror

OBJS_SERVER = server.o
OBJS_CLIENT = client.o

SERVER = server
CLIENT = client

all: $(SERVER) $(CLIENT)

$(SERVER): $(OBJS_SERVER)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT): $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_SERVER) $(OBJS_CLIENT)

fclean: clean
	rm -f $(SERVER) $(CLIENT)

re: fclean all

.PHONY: all clean fclean re
