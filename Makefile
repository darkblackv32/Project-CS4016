run:
	g++ main.cpp PhysicsWrapper.cpp Collision.cpp  Pause.cpp Menu.cpp Bird.cpp Slingshot.cpp Physics.cpp Title.cpp Game.cpp Levels.cpp helper.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d
