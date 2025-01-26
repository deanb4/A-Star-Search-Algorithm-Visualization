SRC_FILE = a_star.cpp
OUT_FILE = a_star
INCLUDE = -I"C:\Users\deanb\OneDrive\SMFL_practice\SFML\src\include"
LIB = -L"C:\Users\deanb\OneDrive\SMFL_practice\SFML\src\lib" 
all: compile link

compile:
	g++ -c $(SRC_FILE) $(INCLUDE) -DSFML_STATIC 

link:
	g++ $(SRC_FILE:.cpp=.o) -o $(OUT_FILE) $(LIB) -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows  

clean:
	rm -f $(OUT_FILE) *.o