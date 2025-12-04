CXX      = g++
CXXFLAGS = -std=c++17 -Isrc -Isrc/core -Isrc/entities -Isrc/ui -I. $(EXTRA_FLAGS)
LDFLAGS  = -Llib/
LDLIBS   = -lbox2d -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

TARGET   = game
SRC_DIR  = src

SRCS = \
    $(SRC_DIR)/main.cpp \
    $(SRC_DIR)/core/Game.cpp \
    $(SRC_DIR)/core/PhysicsWrapper.cpp \
    $(SRC_DIR)/core/Collision.cpp \
    $(SRC_DIR)/core/helper.cpp \
    $(SRC_DIR)/entities/Bird.cpp \
    $(SRC_DIR)/entities/Levels.cpp \
    $(SRC_DIR)/entities/LevelPreview.cpp \
    $(SRC_DIR)/entities/Particles.cpp \
    $(SRC_DIR)/entities/Slingshot.cpp \
    $(SRC_DIR)/ui/Menu.cpp \
    $(SRC_DIR)/ui/Pause.cpp \
    $(SRC_DIR)/ui/Title.cpp \
    $(SRC_DIR)/ui/Victory.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
