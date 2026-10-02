NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -Iheader

SRCS = src/main.c src/parse.c src/numeric.c src/time.c src/heap.c \
	src/heap_ops.c src/heap_remove.c src/sync.c src/stop.c src/dongle.c \
	src/acquire.c src/scheduler.c src/scheduler_ops.c src/wait.c \
	src/wait_resources.c src/coder.c src/coder_state.c src/request.c \
	src/monitor.c src/deadline.c src/lifecycle.c src/threads.c \
	src/init_resources.c src/cleanup.c

OBJS = $(SRCS:src/%.c=.obj/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

.obj:
	mkdir -p .obj

.obj/%.o: src/%.c header/codexion.h | .obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf .obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re