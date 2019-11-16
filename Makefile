CXX = g++
SOURCE_CXX = main.cpp       \
	src/utility/chrono.cpp    \
	src/utility/random.cpp    \
	src/entity/cicd.cpp       \
	src/theapp/my_request.cpp \

TARGET = the3
LDFLAGS_COMMON = -std=c++17
LIBS = -lpthread

all:
	$(CXX) $(SOURCE_CXX)  $(LDFLAGS_COMMON) -o $(TARGET) $(LIBS)

clean:
	rm -rvf *.o $(TARGET)