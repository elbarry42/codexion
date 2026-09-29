NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c parse.c numeric.c time.c heap.c heap_ops.c heap_remove.c sync.c stop.c dongle.c acquire.c scheduler.c scheduler_ops.c wait.c wait_resources.c \
	coder.c coder_state.c request.c monitor.c deadline.c lifecycle.c threads.c init_resources.c cleanup.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
