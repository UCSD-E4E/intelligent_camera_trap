# REMOTE SYSTEM MAKEFILE
all: remote arm

remote: remote_system.cpp
	g++ -ggdb `pkg-config --cflags opencv` -o `basename remote_system.cpp .cpp` remote_system.cpp motor_controller.cpp `pkg-config --libs opencv` -std=c++0x -lpthread -lboost_system

arm:
	arm-none-linux-gnueabi-g++ -ggdb `pkg-config --cflags opencv` -o `basename remote_system_arm.cpp .cpp` remote_system.cpp motor_controller.cpp `pkg-config --libs opencv` -std=c++0x -lpthread -lboost_system -I/home/riley/Downloads/boost_1_54_0

clean:
	rm -rf *.o remote_system

