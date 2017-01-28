PROJECT		= cpp_nanotekspice

SRC_GEN		= main.cpp \
						test/test.cpp

OBJ_GEN		= $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

NAME_GEN	= nanotekspice
SRC_GENDIR	= src/
OBJ_GENDIR	= obj/

NAME_LIB = libnanotekspice.a

FLAGS		= -W -Werror -Wextra -Wall
CFLAGS		= $(FLAGS)

RM		= rm -rf

CC		= g++

all:
	@make --no-print-directory $(NAME_GEN)

$(OBJ_GENDIR)%.o: $(SRC_GENDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

$(NAME_GEN): $(OBJ_GEN) $(MYNAME)
	$(CC) -o $(NAME_GEN) $(OBJ_GEN)

clean:
	$(RM) $(OBJ_GENDIR)

fclean:	clean
	$(RM) $(NAME_GEN)

re: fclean all


.PHONY: all clean fclean re
