MAKEFLAGS += -j

CXX		=	c++
NAME 	= 	computor
SRC 	=	main.cpp Computor.cpp utils.cpp
HEADER	=	inc/Computor.hpp
			
OBJS	=	$(addprefix obj/, $(SRC:.cpp=.o))
CXXFLAGS=	-Wall -Wextra --std=c++17 -g

all		:	${NAME} 

${NAME}	:	${OBJS} ${HEADER}
			${CXX} -o ${NAME} ${OBJS}
obj/%.o: src/%.cpp $(HEADER)
	@mkdir -p $(@D)
	${CXX} ${CXXFLAGS} -c $< -o $@

fclean: clean
	rm -f $(NAME)

clean:
	rm -rf obj/

re: fclean
	@make all

.PHONY: all fclean clean re
