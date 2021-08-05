#include "src/Visualiser.hpp"

int main()
{
	Visualiser visual;
	while (visual.isRunning()) {
		visual.update();
		visual.render();
	}
}