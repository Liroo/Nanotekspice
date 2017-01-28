PROJECT = cpp_nanotekspice

OBJ_DIR = obj/

# Library

SRC_LIBDIR = lib/
OBJ_LIBDIR = $(OBJ_DIR)lib/

SRC_LIB =	test/test.cpp
OBJ_LIB = $(addprefix $(OBJ_LIBDIR), $(SRC_LIB:.cpp=.o))

NAME_LIB = libnanotekspice.a

# General behavior of nanotekspice

SRC_GENDIR = src/
OBJ_GENDIR = $(OBJ_DIR)src/

SRC_GEN = main.cpp
OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

NAME_GEN = nanotekspice

# Utility

CC = g++
FLAGS = -W -Werror -Wextra -Wall
CFLAGS = $(FLAGS)
AR = ar

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
	$(AR) rcf $(NAME_LIB) $(OBJ_LIB)
	$(AR) s $(NAME_LIB)

$(NAME_GEN): $(NAME_LIB) $(OBJ_GEN)
	$(CC) -o $(NAME_GEN) $(OBJ_GEN) -L. -lnanotekspice

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_GEN) $(NAME_LIB)

re: fclean all


.PHONY: all clean fclean re
