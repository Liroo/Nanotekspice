PROJECT = cpp_nanotekspice

OBJ_DIR = obj/

# Library

NAME_LIB = libnanotekspice.a

SRC_LIBDIR = lib/
OBJ_LIBDIR = $(OBJ_DIR)lib/

SRC_LIB =	parser/Parser.cpp						\
					cli/CLI.cpp									\
					cli/command/Display.cpp			\
					cli/command/Dump.cpp				\
					cli/command/Simulate.cpp		\
					error/BaseException.cpp			\
					components/AComponent.cpp		\
					components/Pin.cpp					\
					components/Input.cpp				\
					components/Comp4001.cpp			\
					components/Output.cpp

OBJ_LIB = $(addprefix $(OBJ_LIBDIR), $(SRC_LIB:.cpp=.o))

# General behavior of nanotekspice

NAME_GEN = nanotekspice

SRC_GENDIR = src/
OBJ_GENDIR = $(OBJ_DIR)src/

SRC_GEN = main.cpp

OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

# Utility

INCDIRS := $(addprefix -I,$(shell find $(SRC_LIBDIR) -type d -print))
CC = g++
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

$(OBJ_GENDIR)%.o: $(SRC_GENDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

$(NAME_LIB): $(OBJ_LIB)
	$(AR) $(NAME_LIB) $(OBJ_LIB)
	$(RANLIB) $(NAME_LIB)

$(NAME_GEN): $(NAME_LIB) $(OBJ_GEN)
	$(CC) -o $(NAME_GEN) $(OBJ_GEN) -L. -lnanotekspice

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_GEN) $(NAME_LIB)

re: fclean all

.PHONY: all clean fclean re
