CC = g++

src = $(wildcard *.cpp) \
	$(wildcard parser/*.cpp) \
	$(wildcard shader/*.cpp) \
	$(wildcard axes/*.cpp) \
	$(wildcard graph/*.cpp)
obj = $(src:.cpp=.o) build/src/glad.o

TARGET = main.exe
CXXFLAGS =-Ibuild/include
CFLAGS = $(CXXFLAGS)

all: linked

linked: $(obj)
	$(CXX) -o $(TARGET) $^ -lglfw3dll

static: $(obj)
	$(CXX) -o $(TARGET) $^ -static -lglfw3 -lgdi32

clean:
	del /Q /F /S $(TARGET) *.o