PROJECT = cpp_nanotekspice

OBJ_DIR = obj/

# Library

NAME_LIB = libnanotekspice.a

SRC_LIBDIR = lib/
OBJ_LIBDIR = $(OBJ_DIR)lib/

SRC_LIB =	parser/Parser.cpp						\
					cli/CLI.cpp									\
					cli/mode/NcursesMode.cpp		\
					cli/mode/BasicMode.cpp			\
					error/BaseException.cpp			\
					components/AComponent.cpp		\
					components/Pin.cpp					\
					components/Input.cpp				\
					components/Comp4001.cpp			\
					components/Output.cpp

OBJ_LIB = $(addprefix $(OBJ_LIBDIR), $(SRC_LIB:.cpp=.o))

# General behavior of nanotekspice

NAME_GEN = nanotekspice

SRC_BINDIR = bin/
OBJ_BINDIR = $(OBJ_DIR)bin/

SRC_BIN = main.cpp

OBJ_BIN = $(addprefix $(OBJ_BINDIR), $(SRC_BIN:.cpp=.o))

# Utility

INCDIRS := $(addprefix -I,$(shell find $(SRC_LIBDIR) -type d -print))
CC = g++
LDFLAGS = -L. -lnanotekspice -lncurses
FLAGS = -W -Werror -Wextra -Wall -g
FLAGS += -std=c++11
CFLAGS = $(FLAGS) $(INCDIRS)
ifeq ($(shell uname), Darwin)
	AR = ar rc
else
	AR = ar rcf
endif
RANLIB = ar s

RM		= rm -rf

# Rules

all:
	@make --no-print-directory $(NAME_LIB) $(NAME_GEN)

$(OBJ_LIBDIR)%.o: $(SRC_LIBDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_BINDIR)%.o: $(SRC_BINDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

$(NAME_LIB): $(OBJ_LIB)
	$(AR) $(NAME_LIB) $(OBJ_LIB)
	$(RANLIB) $(NAME_LIB)

$(NAME_GEN): $(NAME_LIB) $(OBJ_BIN)
	$(CC) -o $(NAME_GEN) $(OBJ_BIN) $(LDFLAGS)

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_GEN) $(NAME_LIB)

re: fclean all

.PHONY: all clean fclean re
