#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

#define WIDTH 320
#define HEIGHT 240

std::vector<float> interpolateSingleFloats(float from, float to, int numOfValues) {
	std::vector<float> arr(numOfValues);
	float increment = (to-from)/(numOfValues-1);
	for (int i=0; i < numOfValues; i++){
		arr[i] = from + increment*i;
	}
	return arr;
}

void draw(DrawingWindow &window) {
	window.clearPixels();
	std::vector<float> grayscale = interpolateSingleFloats(255, 0, window.width);
	int i = 0;

	// Triangle co-ordinates
	glm::vec2 v0(0,0); // red
	glm::vec2 v1(WIDTH/2, HEIGHT); // green
	glm::vec2 v2(WIDTH, 0); // blue
	// Colours
	glm::vec3 red(255,0,0);
	glm::vec3 green(0,255,0);
	glm::vec3 blue(0,0,255);

	int l = 0; int r = WIDTH;
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = l; x < r; x++) {
			glm::vec2 cur(x,y);
			float v1dist = glm::distance(v1, cur) / glm::distance(v0,v1);
			float v2dist = glm::distance(v2, cur) / glm::distance(v0,v2);
			float v0dist = 1 - v1dist - v2dist;
			float red = (1-v0dist) * 255;
			float green = (1-v1dist) * 255;
			float blue = (1-v2dist) * 255;
//			float red = pixels[i][0];
//			float green = pixels[i][1];
//			float blue = pixels[i][2];
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
			i += 1;
		}
		l ++;
		r --;
	}
}

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues){
	std::vector<float> arr1 = interpolateSingleFloats(from[0], to[0], numberOfValues);
	std::vector<float> arr2 = interpolateSingleFloats(from[1], to[1], numberOfValues);
	std::vector<float> arr3 = interpolateSingleFloats(from[2], to[2], numberOfValues);
	std::vector<glm::vec3> res;
	for (int i = 0; i < numberOfValues; i++){
		res.push_back(glm::vec3(arr1[i], arr2[i], arr3[i]));
	};
	return res;
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char *argv[]) {
	glm::vec3 topLeft(255,0,0);
	glm::vec3 topRight(0,0,255);
	glm::vec3 bottomLeft(255,255,0);
	glm::vec3 bottomRight(0,255,0);
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;

	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
