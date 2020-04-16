#include <SFML/Graphics.hpp>
#include <vector>
#include "cvector.h"
#include "surface.h"

using namespace sf;

int draw(const vector<Surface>& faces, const vector<CVector>& veticals, const char& type) {
	RenderWindow window(VideoMode(900, 900), "ConvexHull");
	std::vector<Text> letters;

	Font f;
	f.loadFromFile("CyrilicOld.TTF");

	letters.resize(3);

	letters[0].setString("X");
	letters[1].setString("Y");
	letters[2].setString("Z");


	letters[0].setPosition(700, 400);
	letters[1].setPosition(200, 600);
	letters[2].setPosition(400, 100);

	for (auto s : letters) {
		s.setCharacterSize(16);
		s.setFillColor(Color::White);
		s.setFont(f);
	}


	VertexArray cXYZ(Lines, 6);
	cXYZ[0].position = Vector2f(450, 450);
	cXYZ[1].position = Vector2f(450, 100);
	cXYZ[2].position = Vector2f(450, 450);
	cXYZ[3].position = Vector2f(800, 450);
	cXYZ[4].position = Vector2f(450, 450);
	cXYZ[5].position = Vector2f(200, 700);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		//window.clear(Color::White);
		for (auto s : letters)
			window.draw(s);
		window.draw(cXYZ);
		window.display();
	}

	return 0;
}