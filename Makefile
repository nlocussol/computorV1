MAKEFLAGS += -j

CXX		=	c++
NAME 	= 	computor
SRC 	=	main.cpp Computor.cpp plot.cpp utils.cpp
HEADER	=	inc/Computor.hpp inc/utils.hpp inc/plot.hpp inc/matplotlibcpp.h
			
OBJS	=	$(addprefix obj/, $(SRC:.cpp=.o))
CXXFLAGS=	-Wall -Wextra --std=c++17 -g -DWITHOUT_NUMPY
LDFLAGS = 	-lpython3.10

all		: ${NAME}

plot: CXXFLAGS += -DPLOT

plot: ${NAME}

${NAME}	: ${OBJS} ${HEADER} 
	${CXX} -o ${NAME} ${OBJS} ${LDFLAGS}

obj/%.o: src/%.cpp $(HEADER)
	@mkdir -p $(@D)
	${CXX} ${CXXFLAGS} -c $< -o $@

fclean: clean
	rm -f $(NAME)

clean:
	rm -rf obj/

re: fclean
	@make all

plotre: fclean
	@make plot

.PHONY: all plot fclean clean re
