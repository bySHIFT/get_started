CXX = g++
SOURCE_CXX = main.cpp src/entity/cicd.cpp src/theapp/my_request.cpp
TARGET = the3
LDFLAGS_COMMON = -std=c++17

all:
	$(CXX) $(SOURCE_CXX)  $(LDFLAGS_COMMON) -o $(TARGET)

clean:
	rm -rvf *.o $(TARGET)