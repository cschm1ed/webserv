NAME      := webserv
BUILDDIR  := build
VPATH	  := sources:sources/response
CFLAGS    := -Wall -Werror -Wextra -std=c++98 -g -I includes -D DEBUG=1
CC        := g++

SRCS	  := main.cpp Host.cpp Parser.cpp Router.cpp Server.cpp  MyException.cpp \
			 writeFileToFd.cpp RequestHandler.cpp
OBJS      := $(SRCS:%.cpp=$(BUILDDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS)  -o $@ $^

$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS)  -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

kill:
	bash kill.sh

run: kill re
	./webserv resources/configs/conf1.cnf


.PHONY: clean all re fclean kill
