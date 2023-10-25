ifeq (,$(wildcard $(./matplotlib-cpp/)))
    FOLDER_EXISTS := 0
else
    FOLDER_EXISTS := 1
endif

MAKEFLAGS += -j

CXX		=	c++
NAME 	= 	computor
SRC 	=	main.cpp Computor.cpp plot.cpp utils.cpp
HEADER	=	inc/Computor.hpp inc/utils.hpp matplotlib-cpp/matplotlibcpp.h inc/plot.hpp
			
OBJS	=	$(addprefix obj/, $(SRC:.cpp=.o))
CXXFLAGS=	-Wall -Wextra --std=c++17 -g -DWITHOUT_NUMPY -I/usr/include/python3.10
LDFLAGS= 	-lpython3.10

all		:	${NAME} 

matplotlib_cpp_folder_present := $(wildcard matplotlib-cpp)

lib     :
	-@if [ $(FOLDER_EXISTS) -eq 1 ]; then \
		echo "The folder exists"; \
	else \
		git clone https://github.com/lava/matplotlib-cpp.git; \
	fi

${NAME}	:	lib ${OBJS} ${HEADER} 
	${CXX} -o ${NAME} ${OBJS} ${LDFLAGS}

obj/%.o: src/%.cpp $(HEADER)
	@mkdir -p $(@D)
	${CXX} ${CXXFLAGS} -c $< -o $@

fclean: clean
	rm -rf matplotlib-cpp/
	rm -f $(NAME)

clean:
	rm -rf obj/

re: fclean
	@make all

.PHONY: all fclean clean re
