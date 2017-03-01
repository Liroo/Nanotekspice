PROJECT = cpp_nanotekspice

OBJ_DIR = obj/

# Library

NAME_LIB = libnanotekspice.a

SRC_LIBDIR = lib/
OBJ_LIBDIR = $(OBJ_DIR)lib/

SRC_LIB =	parser/Parser.cpp									\
					cli/CLI.cpp												\
					cli/mode/NcursesMode.cpp					\
					cli/mode/BasicMode.cpp						\
					error/BaseException.cpp						\
					components/AComponent.cpp					\
					components/Pin.cpp								\
					components/CInputs/Input.cpp			\
					components/CInputs/Clock.cpp			\
					components/CInputs/True.cpp				\
					components/CInputs/False.cpp			\
					components/COutput/Output.cpp			\
					components/C4001/Comp4001.cpp			\
					components/C4008/Comp4008.cpp			\
					components/C4011/Comp4011.cpp			\
					components/C4013/Comp4013.cpp			\
					components/C4017/Comp4017.cpp			\
					components/C4030/Comp4030.cpp			\
					components/C4040/Comp4040.cpp			\
					components/C4069/Comp4069.cpp			\
					components/C4071/Comp4071.cpp			\
					components/C4081/Comp4081.cpp			\
					components/C4094/Comp4094.cpp

OBJ_LIB = $(addprefix $(OBJ_LIBDIR), $(SRC_LIB:.cpp=.o))

# General behavior of nanotekspice

NAME_BIN = nanotekspice

SRC_GENDIR = src/
OBJ_GENDIR = $(OBJ_DIR)src/

SRC_GEN = main.cpp

OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

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
	@make --no-print-directory $(NAME_LIB) $(NAME_BIN)

$(OBJ_LIBDIR)%.o: $(SRC_LIBDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_GENDIR)%.o: $(SRC_GENDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

$(NAME_LIB): $(OBJ_LIB)
	$(AR) $(NAME_LIB) $(OBJ_LIB)
	$(RANLIB) $(NAME_LIB)

$(NAME_BIN): $(NAME_LIB) $(OBJ_GEN)
	$(CC) -o $(NAME_BIN) $(OBJ_GEN) $(LDFLAGS)

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_BIN) $(NAME_LIB)

re: fclean all

.PHONY: all clean fclean re
