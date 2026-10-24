CXX = g++
RAYLIB_PATH = C:/raylib/raylib

CXXFLAGS = -std=c++17 -Wall -Wshadow -Wunused-parameter -O0 -I$(RAYLIB_PATH)/src -Iexternal -DPLATFORM_DESKTOP
LDFLAGS = -L. -lraylib -lopengl32 -lgdi32 -lwinmm -lshcore

SRC_DIR = src
BUILD_DIR = build
TARGET = dungeon.exe

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(RAYLIB_PATH)/src/raylib.rc.data $(LDFLAGS) -static

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	-del /Q $(BUILD_DIR)\*.o $(BUILD_DIR)\*.d 2>NUL
	-del /Q $(TARGET) 2>NUL
