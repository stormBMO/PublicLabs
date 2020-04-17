#include <SFML/Graphics.hpp>
#include <vector>
#include "cvector.h"
#include "surface.h"

using namespace sf;

int draw(const map<pair<string, string>, int>& mat1, const vector<CVector>& vertices1 ) {
		RenderWindow window(VideoMode(900, 900), "ConvexHull");

		VertexArray cXYZ(Lines, 6);
		cXYZ[0].position = Vector2f(450, 500);
		cXYZ[1].position = Vector2f(450, 100);
		cXYZ[2].position = Vector2f(400, 450);
		cXYZ[3].position = Vector2f(800, 450);
		cXYZ[4].position = Vector2f(480, 420);
		cXYZ[5].position = Vector2f(200, 700);

		VertexArray cXYZarrows(Lines, 12);

		cXYZarrows[0].position = Vector2f(450, 100);
		cXYZarrows[1].position = Vector2f(460, 115);
		cXYZarrows[2].position = Vector2f(450, 100);
		cXYZarrows[3].position = Vector2f(440, 115);
		cXYZarrows[4].position = Vector2f(800, 450);
		cXYZarrows[5].position = Vector2f(785, 440);
		cXYZarrows[6].position = Vector2f(800, 450);
		cXYZarrows[7].position = Vector2f(785, 460);
		cXYZarrows[8].position = Vector2f(200, 700);
		cXYZarrows[9].position = Vector2f(220, 700);
		cXYZarrows[10].position = Vector2f(200, 700);
		cXYZarrows[11].position = Vector2f(204, 680);

		VertexArray row(Lines, 2);
		row[0].color = Color::Yellow;
		row[1].color = Color::Yellow;

		while (window.isOpen()) {
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}

			window.clear();
			for (int i = 0; i < vertices1.size(); ++i)
				for (int j = 0;i != j,  j < vertices1.size(); ++j) {
					row[0].position.x = 450 + vertices1[i].x * 100 - 40 * vertices1[i].y;
					row[0].position.y = 450 - vertices1[i].z * 100 + 40 * vertices1[i].y;
					if ((mat1.count(make_pair(vertices1[i].GetType(), vertices1[j].GetType())) ? 1 : 0)) {
						row[1].position.x = 450 + vertices1[j].x * 100 - 40 * vertices1[j].y;
						row[1].position.y = 450 - vertices1[j].z * 100 + 40 * vertices1[j].y;
						window.draw(row);
					}
				}
			window.draw(cXYZ);
			window.draw(cXYZarrows);
			window.display();
		}

		return 0;
}