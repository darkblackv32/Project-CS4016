CXX      = g++
CXXFLAGS = -std=c++17 -I./
LDFLAGS  = -Llib/
LDLIBS   = -lbox2d -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

TARGET   = game

SRCS = \
    main.cpp \
    Bird.cpp \
    Collision.cpp \
    Game.cpp \
    LevelPreview.cpp \
    Levels.cpp \
    Menu.cpp \
    Particles.cpp \
    Pause.cpp \
    PhysicsWrapper.cpp \
    Slingshot.cpp \
    Title.cpp \
    Victory.cpp \
    helper.cpp

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

