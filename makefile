LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
CXX := g++

all: main

%.o: %.cpp
	$(CXX) -c $< -o $@

%.o: %.hpp
	$(CXX) -c $< -o $@

main: main.o
	@echo "** Building the game"
	$(CXX) -o game main.o src/*.cpp $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	rm -f main *.o

install:
	@echo '** Installing...'
	cp main /usr/bin/

uninstall:
	@echo '** Uninstalling...'
	$(RM) /usr/bin/main
