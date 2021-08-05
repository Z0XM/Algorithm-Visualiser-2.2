#pragma once
#include <SFML/Graphics.hpp>
#include "GUI.hpp"

#include <vector>
#include <functional>
#include <stack>

class Visualiser {
private:
	sf::RenderWindow window;
	bool running;
	float counter;
	float waitTime;

	float barWidth;

	std::vector<int> nums;
	std::vector<int> backup;
	std::vector<sf::Color> colorMap;
	std::vector<sf::Color*> barsToResetColor;
	sf::Color mainColor, priColor, secColor, terColor;

	sf::Font font;
	sf::Clock algoClock;

	gui::Frame mainFrame;
	gui::Page mainPage;
	gui::Button shuffle, load;
	gui::Button bubbleButton, selectButton, insertButton, quickLLButton, quickLRButton, mergeButton, merge2Button;
	gui::Slider widthSlider, speedSlider;
	gui::Textbox timeBox;

	void initWindow();
	void initGUI();

	void generateRandom();
	void loadAgain();

	void drawBars();
	void setActiveBars(sf::Color* w, sf::Color* g, sf::Color* b = nullptr);

	struct {
		int outer = 0, inner = 0;
	} bubble;
	struct {
		int outer, inner, min;
	} selection;
	struct {
		int outer, inner, index;
		int bar;
	} insertion;
	struct {
		int outer, inner;
		int* pivot;
		std::stack<std::pair<int, int>> stack;
	} quick_ll;
	struct {
		int left, right;
		int* pivot;
		std::stack<std::pair<int, int>> stack;
	} quick_lr;
	struct {
		int first, second, merged;
		std::vector<int> one, two;
		std::stack<std::pair<std::pair<int, int>, std::pair<int, int>>> stack;
	} merge;
	struct {
		int first, second, merged;
		std::vector<int> one, two;
		std::stack<std::pair<std::pair<int, int>, std::pair<int, int>>> stack;
	} merge2;

	enum Algo { NONE, BUBBLE, SELECTION, INSERTION, QUICK_LL, QUICK_LR, MERGE, MERGE_2 };
	void setAlgorithm(Algo algo);
	std::function<void()> algo;
	void bubble_sort();
	void selection_sort();
	void insertion_sort();
	void quick_sort_LL();
	void quick_sort_LR();
	void merge_sort();
	void merge_sort2();

public:
	Visualiser();
	~Visualiser();

	bool isRunning();

	void update();
	void pollEvents();
	void render();

	sf::Vector2f getMousePosition();
	sf::Vector2f getWinSize();
	sf::Vector2u getWinSizeInt();
};
