CC = g++

src = $(wildcard *.cpp) \
	$(wildcard parser/*.cpp) \
	$(wildcard shader/*.cpp) \
	$(wildcard text/*.cpp) \
	$(wildcard axes/*.cpp) \
	$(wildcard graph/*.cpp)
obj = $(src:.cpp=.o) build/src/glad.o

GLFW_INCLUDE = C:/libs/glfw-3.3.2.bin.WIN32/include
GLFW_LIB = C:/libs/glfw-3.3.2.bin.WIN32/lib-mingw-w64

TARGET = main.exe
CXXFLAGS =-Ibuild/include -I${GLFW_INCLUDE}
CFLAGS = $(CXXFLAGS)

all: linked

linked: $(obj)
	$(CXX) -o $(TARGET) $^ -L${GLFW_LIB} -lglfw3dll

static: $(obj)
	$(CXX) -o $(TARGET) $^ -static -lglfw3 -lgdi32

clean:
	del /Q /F /S $(TARGET) *.o