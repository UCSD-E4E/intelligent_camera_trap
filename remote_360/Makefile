# REMOTE SYSTEM MAKEFILE
all: remote

remote: remote_system.cpp
	g++ -ggdb `pkg-config --cflags opencv` -o `basename remote_system.cpp .cpp` remote_system.cpp motor_controller.cpp `pkg-config --libs opencv` -std=c++0x -lpthread -lboost_system

clean:
	rm -rf *.o remote_system

