NAME		:= webserv
VPATH		:= sources

SRCS		:= main.cpp
BUILDDIR	:= build
OBJS		:= $(SRCS:%.c=$(BUILDDIR)/%.o)

CFLAGS		:= -Wall -Werror -Wextra -std=c++98
CC			:= g++

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -I includes $< -o $@

$(BUILDDIR)/%.o: %.c $(BUILDDIR)
	$(CC) $(CFLAGS) -I includes $^ -o $@

$(BUILDDIR):
	mkdir $(BUILDDIR)

.PHONY: clean all re fclean

clean:
	rm -rf $(BUILDDIR)

fclean: clean
	rm -f $(NAME)

all:
	make $(NAME)

re: fclean all