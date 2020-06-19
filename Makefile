snake.exe : main.cpp snake.cpp stageClass.cpp
	g++ -std=c++14 -o snake.exe main.cpp snake.cpp stageClass.cpp -lncurses

clean :
	rm snake.exe