
#.~"~._.~"~._.~"~._.~"~.__.~"~._.~"~. VARS .~"~._.~"~.__.~"~._.~"~._.~"~._.~"~.#

SHELL 			=	/bin/bash
CPPFLAGS		= 	-Wall -Wextra -Werror -std=c++98 -g 
CPPFLAGS		+=	-fsanitize=address
CXX 			= 	c++
# ---------------------------------------#

NAME			=	ircserv


SRC				= 	main \
					Client.Class \
					Channel.Class \
					Server.Class \
					Server.Parsing \
					modes \
					commands
					
DEPS 			= Channel.Class.hpp \
						Client.Class.hpp \
						defines.hpp \
						Server.Class.hpp

SRC_FILES		=	$(addsuffix .c, $(SRC))
OBJ_FILES		=	$(addsuffix .o, $(SRC))

#.~"~._.~"~._.~"~._.~"~.__.~"~._.~"~. RULES .~"~._.~"~.__.~"~._.~"~._.~"~._.~"~.#

all: $(NAME)

%.o: %.c
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJ_FILES) $(DEPS)
	$(CXX) $(CPPFLAGS) $(OBJ_FILES) -o $(NAME)

clean:
	@if find ${OBJ_FILES} > /dev/null 2>&1; then \
		rm -f $(OBJ_FILES); \
	fi

fclean: clean
	@rm -f ${NAME};

re: fclean all

.PHONY: all clean fclean re

#.~"~._.~"~._.~"~._.~"~.__.~"~._.~"~._.~"~._.~"~._.~"~.__.~"~._.~"~._.~"~._.~"~.#