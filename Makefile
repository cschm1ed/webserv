NAME      := webserv
BUILDDIR  := build
VPATH	  := sources:sources/response
CFLAGS    := -Wall -Werror -Wextra -std=c++98 -g
CC        := g++
#SRCS      := main.cpp writeFileToFd.cpp handleRequest.cpp getRequest.cpp Server.cpp Logger.cpp \
#			Parser.cpp
SRCS	  := main.cpp Host.cpp Parser.cpp Router.cpp Server.cpp Logger.cpp MyException.cpp \
			handleRequest.cpp getRequest.cpp writeFileToFd.cpp
OBJS      := $(SRCS:%.cpp=$(BUILDDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -I includes -o $@ $^

$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -I includes -c $< -o $@

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
