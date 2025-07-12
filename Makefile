run:
	g++ -I./ main.cpp Victory.cpp PhysicsWrapper.cpp Collision.cpp  Pause.cpp Menu.cpp Bird.cpp Slingshot.cpp Title.cpp Game.cpp Levels.cpp LevelPreview.cpp helper.cpp -o game -Llib/ -lbox2d -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
