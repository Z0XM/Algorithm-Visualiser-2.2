#include "Visualiser.hpp"
#include <random>
#include <chrono>
#include <algorithm>

std::string secondsAsTime(float t) {
	int ms = int(t * 100) % 100;
	int s = t;
	int m = s / 60;
	s = s % 60;

	return std::to_string(m) + ":" + std::to_string(s) + ":" + std::to_string(ms);
}

void Visualiser::initWindow()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	window.create(sf::VideoMode(1450, 500), "Visualiser", sf::Style::Default, settings);
	window.setFramerateLimit(120);
}

void Visualiser::initGUI()
{
	font.loadFromFile("data/consola.ttf");

	mainFrame.setWindow(window);
	mainPage.setFillColor(sf::Color(255, 255, 255, 26));
	mainPage.setSize({400, 1000});
	mainPage.setPosition(1050, 0);
	mainPage.setActiveRegion({ 0,0, 400, 600 });
	mainPage.setScroll(gui::Scroll::RIGHT);
	mainFrame.addEntity(mainPage);

	sf::Vector2f size(180, 50);
	sf::Vector2i pos(0, 0);

	shuffle.setSize({ 180, 50 });
	shuffle.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	shuffle.setFillColor(sf::Color::Transparent);
	shuffle.setOutlineColor(sf::Color::Red);
	shuffle.setOutlineThickness(3);
	shuffle.setHighlightFillColor(sf::Color(255, 0, 0, 126));
	shuffle.setHighlightOutlineColor(sf::Color(255, 165, 0));
	shuffle.setHighlightOutlineThickness(5);
	shuffle.setCharacterSize(28);
	shuffle.setTextFillColor(sf::Color::White);
	shuffle.setFont(font);
	shuffle.setString("Shuffle");
	mainPage.addEntity(shuffle);
	shuffle.setAction([this] {generateRandom();});
	
	pos = { 1, 0 };
	load.copy(shuffle);
	load.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	load.setString("Load");
	mainPage.addEntity(load);
	load.setAction([this] {loadAgain(); });

	pos = { 0, 2 };
	widthSlider.setSize(size);
	widthSlider.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	widthSlider.setFillColor(sf::Color::Transparent);
	widthSlider.setOutlineColor(sf::Color::Blue);
	widthSlider.setOutlineThickness(3);
	widthSlider.setTextFillColor(sf::Color::White);
	widthSlider.setFont(font);
	widthSlider.setCharacterSize(28);
	widthSlider.setString("Width");
	widthSlider.setAction([this]() {
		widthSlider.moveSliderBar(mainPage.getMousePosition());
		barWidth = std::max(2.f, widthSlider.getMappedOffset() * 50);
		});
	mainPage.addEntity(widthSlider);

	pos = { 1, 2 };
	speedSlider.copy(widthSlider);
	speedSlider.setString("Speed");
	speedSlider.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	speedSlider.setAction([this]() {
		speedSlider.moveSliderBar(mainPage.getMousePosition());
		waitTime = 50 - std::max(0.f, speedSlider.getMappedOffset() * 50);
		});
	speedSlider.setHigh();
	mainPage.addEntity(speedSlider);

	timeBox.setSize({ 80, 40 });
	timeBox.setPosition(10 + 0.75 * 195, 75);
	timeBox.setFont(font);
	timeBox.setFillColor(sf::Color::Transparent);
	timeBox.setOutlineColor(sf::Color::Transparent);
	timeBox.setCharacterSize(28);
	timeBox.setTextFillColor(sf::Color::White);
	timeBox.setString(secondsAsTime(0));
	mainPage.addEntity(timeBox);

	pos = { 0, 3 };
	bubbleButton.copy(shuffle);
	bubbleButton.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	bubbleButton.setOutlineColor(sf::Color(160, 32, 240));
	bubbleButton.setHighlightFillColor(sf::Color(160, 32, 240, 126));
	bubbleButton.setHighlightOutlineColor(sf::Color(124, 163, 251));
	bubbleButton.setString("Bubble");
	mainPage.addEntity(bubbleButton);
	bubbleButton.setAction([this] {setAlgorithm(BUBBLE); });

	pos = { 1,3 };
	selectButton.copy(bubbleButton);
	selectButton.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	selectButton.setString("Select");
	mainPage.addEntity(selectButton);
	selectButton.setAction([this] {setAlgorithm(SELECTION); });

	pos = { 0,4 };
	insertButton.copy(bubbleButton);
	insertButton.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	insertButton.setString("Insert");
	mainPage.addEntity(insertButton);
	insertButton.setAction([this] {setAlgorithm(INSERTION); });

	pos = { 1,4 };
	quickLLButton.copy(bubbleButton);
	quickLLButton.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	quickLLButton.setString("Quick LL");
	mainPage.addEntity(quickLLButton);
	quickLLButton.setAction([this] {setAlgorithm(QUICK_LL); });

	pos = { 0,5 };
	quickLRButton.copy(bubbleButton);
	quickLRButton.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	quickLRButton.setString("Quick LR");
	mainPage.addEntity(quickLRButton);
	quickLRButton.setAction([this] {setAlgorithm(QUICK_LR); });

	pos = { 1,5 };
	mergeButton.copy(bubbleButton);
	mergeButton.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	mergeButton.setString("Merge");
	mainPage.addEntity(mergeButton);
	mergeButton.setAction([this] {setAlgorithm(MERGE); });

	pos = { 0,6 };
	merge2Button.copy(bubbleButton);
	merge2Button.setPosition(10 + pos.x * 195, 10 + pos.y * 65);
	merge2Button.setString("Merge 2");
	mainPage.addEntity(merge2Button);
	merge2Button.setAction([this] {setAlgorithm(MERGE_2); });
}

Visualiser::Visualiser()
{
	this->initWindow();
	this->initGUI();

	running = true;

	barWidth = 2.f;
	waitTime = 0.f;

	mainColor = sf::Color::Red;
	priColor = sf::Color::White; secColor = sf::Color::Green; terColor = sf::Color::Blue;

	generateRandom();
}

Visualiser::~Visualiser()
{

}

void Visualiser::generateRandom()
{
	nums.clear();
	int bar_count = (getWinSizeInt().y * 2) / barWidth;
	nums.reserve(bar_count);
	for (int i = 0; i < bar_count; i++)
		nums.push_back(i + 1);

	std::shuffle(nums.begin(), nums.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

	backup = nums;
	
	colorMap.clear();
	colorMap.reserve(nums.size());
	for (int i = 0; i < nums.size(); i++)
		colorMap.push_back(mainColor);
	barsToResetColor.clear();

	counter = 0;

	algo = nullptr;
}

void Visualiser::loadAgain()
{
	nums = backup;

	colorMap.clear();
	colorMap.reserve(nums.size());
	for (int i = 0; i < nums.size(); i++)
		colorMap.push_back(mainColor);
	barsToResetColor.clear();

	counter = 0;

	algo = nullptr;
}

bool Visualiser::isRunning()
{
	return this->running;
}

void Visualiser::update()
{
	mainFrame.update();
	this->pollEvents();

	if(algo != nullptr)timeBox.setString(secondsAsTime(algoClock.getElapsedTime().asSeconds()));

	if (counter++ >= waitTime) {
		if (algo != nullptr) {
			for (int i = 0; i < barsToResetColor.size(); i++) {
				*barsToResetColor[i] = mainColor;
			}
			barsToResetColor.clear();
			algo();
		}

		counter = 0;
	}
}

void Visualiser::pollEvents()
{
	sf::Event event;
	while (this->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
			this->running = false;
		}

		mainFrame.pollEvents(event);
	}
}

void Visualiser::setActiveBars(sf::Color* w, sf::Color* g, sf::Color* b)
{
	if (w != nullptr) {
		*w = priColor;
		barsToResetColor.push_back(w);
	}
	if (g != nullptr) {
		*g = secColor;
		barsToResetColor.push_back(g);
	}

	if (b != nullptr) {
		*b = terColor;
		barsToResetColor.push_back(b);
	}
}

void Visualiser::drawBars()
{
	sf::RectangleShape bar;
	for (int i = 0; i < nums.size(); i++) {
		bar.setFillColor(colorMap[i]);
		bar.setSize({ barWidth - 1, ((float)nums[i] * barWidth) / 2 });
		bar.setPosition(barWidth * i, getWinSizeInt().y - bar.getSize().y);
		this->window.draw(bar);
	}
}

void Visualiser::render()
{
	this->window.clear();

	this->drawBars();
	mainFrame.draw();

	this->window.display();
}

sf::Vector2f Visualiser::getMousePosition()
{
	return window.mapPixelToCoords(sf::Mouse::getPosition(this->window));
}

sf::Vector2f Visualiser::getWinSize()
{
	return sf::Vector2f(this->window.getSize());
}

sf::Vector2u Visualiser::getWinSizeInt()
{
	return this->window.getSize();
}

void Visualiser::setAlgorithm(Algo algo_code)
{
	if (algo_code != NONE) {
		algoClock.restart();
		widthSlider.setInactive();
	}

	switch (algo_code) {
	case BUBBLE:
		bubble.outer = 0;
		bubble.inner = 0;
		algo = [this]() {bubble_sort(); };
		break;
	case SELECTION:
		selection.outer = 0;
		selection.inner = 1;
		selection.min = 0;
		algo = [this]() {selection_sort(); };
		break;
	case INSERTION:
		insertion.outer = 1;
		insertion.inner = 0;
		insertion.bar = nums[1];
		algo = [this]() {insertion_sort(); };
		break;
	case QUICK_LL:
		quick_ll.outer = -1;
		quick_ll.inner = 0;
		quick_ll.pivot = &nums[nums.size() - 1];
		while (!quick_ll.stack.empty())quick_ll.stack.pop();
		quick_ll.stack.push(std::make_pair(0, nums.size() - 1));
		algo = [this]() {quick_sort_LL(); };
		break;
	case QUICK_LR:
		quick_lr.left = 1;
		quick_lr.right = nums.size() - 1;
		quick_lr.pivot = &nums[0];
		while (!quick_lr.stack.empty())quick_lr.stack.pop();
		quick_lr.stack.push(std::make_pair(0, nums.size() - 1));
		algo = [this]() {quick_sort_LR(); };
		break;
	case MERGE:
		merge.first = 0;
		merge.second = 0;
		merge.one.clear();
		merge.two.clear();
		while (!merge.stack.empty())merge.stack.pop();
		{
			std::vector<std::pair<int, int>> s;
			s.push_back(std::make_pair(0, nums.size()));
			int top = 0;
			while (top < s.size()) {
				if (s[top].first != (s[top].first + s[top].second) / 2) {
					s.insert(s.begin() + top + 1, std::make_pair(s[top].first, (s[top].first + s[top].second) / 2));
					s.insert(s.begin() + top + 1, std::make_pair((s[top].first + s[top].second) / 2, s[top].second));
					merge.stack.push(std::make_pair(
						std::make_pair(s[top].first, (s[top].first + s[top].second) / 2),
						std::make_pair((s[top].first + s[top].second) / 2, s[top].second)
					));
				}
				top++;
			}
			auto info = merge.stack.top();
			merge.one.insert(merge.one.begin(), nums.begin() + info.first.first, nums.begin() + info.first.second);
			merge.two.insert(merge.two.begin(), nums.begin() + info.second.first, nums.begin() + info.second.second);
			merge.merged = info.first.first;
		}
		algo = [this]() {merge_sort(); };
		break;
	case MERGE_2:
		merge2.first = 0;
		merge2.second = 0;
		merge2.one.clear();
		merge2.two.clear();
		while (!merge2.stack.empty())merge2.stack.pop();
		{
			std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> v;
			int top = -1;
			v.push_back(std::make_pair(std::make_pair(0, nums.size() / 2 + 1), std::make_pair(nums.size() / 2 + 1, nums.size())));
			while (top + 1 < v.size()) {
				auto k = v[++top];
				if (k.first.first != (k.first.second + k.first.first) / 2)
					v.push_back(std::make_pair(std::make_pair(k.first.first, (k.first.second + k.first.first) / 2), std::make_pair((k.first.second + k.first.first) / 2, k.first.second)));
				if (k.second.first != (k.second.second + k.second.first) / 2)
					v.push_back(std::make_pair(std::make_pair(k.second.first, (k.second.second + k.second.first) / 2), std::make_pair((k.second.second + k.second.first) / 2, k.second.second)));
			}
			for (auto k : v)merge2.stack.push(k);
			auto info = merge2.stack.top();
			merge2.one.insert(merge2.one.begin(), nums.begin() + info.first.first, nums.begin() + info.first.second);
			merge2.two.insert(merge2.two.begin(), nums.begin() + info.second.first, nums.begin() + info.second.second);
			merge2.merged = info.first.first;
		}

		algo = [this]() {merge_sort2(); };
		break;
	default:
		algo = nullptr;
		widthSlider.setActive();
	}
}

void Visualiser::bubble_sort()
{
	int& inner = bubble.inner;
	int& outer = bubble.outer;

	if (outer < nums.size() - 1) {
		if (inner < nums.size() - outer - 1) {
			if (nums[inner + 1] < nums[inner])
				std::swap(nums[inner], nums[inner + 1]);

			setActiveBars(&colorMap[nums.size() - outer - 1], &colorMap[inner + 1]);

			inner++;
		}
		else {
			outer++;
			inner = 0;
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::selection_sort()
{
	int& outer = selection.outer;
	int& inner = selection.inner;
	int& min = selection.min;

	if (outer < nums.size() - 1) {
		if (inner < nums.size()) {
			if (nums[inner] < nums[min])
				min = inner;

			setActiveBars(&colorMap[outer], &colorMap[inner], &colorMap[min]);
			inner++;
		}
		else {
			std::swap(nums[min], nums[outer]);
			outer++;
			inner = outer + 1;
			min = outer;
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::insertion_sort()
{
	int& outer = insertion.outer;
	int& inner = insertion.inner;
	int& index = insertion.index;
	auto& bar = insertion.bar;

	if (outer < nums.size()) {
		if (inner >= 0 && nums[inner] > bar)
		{
			nums[inner + 1] = nums[inner];
			setActiveBars(&colorMap[outer], &colorMap[inner]);
			inner--;
		}
		else {
			nums[inner + 1] = bar;
			outer++;
			inner = outer - 1;
			if (outer < nums.size())bar = nums[outer];
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::quick_sort_LR()
{
	int& left = quick_lr.left;
	int& right = quick_lr.right;
	auto& stack = quick_lr.stack;

	if (!stack.empty()) {
		if (left < right) {
			bool ml = nums[left] < *quick_lr.pivot, mr = nums[right] > * quick_lr.pivot;
			if (ml)left++;
			if (mr)right--;
			if (!ml && !mr && left < right) {
				std::swap(nums[left], nums[right]);
			}
			setActiveBars(nullptr, &colorMap[left], &colorMap[right]);
		}
		else {
			auto info = stack.top();
			stack.pop();
			if (*quick_lr.pivot > nums[right])std::swap(nums[right], *quick_lr.pivot);

			if (right - 1 > info.first)
				stack.push(std::make_pair(info.first, right - 1));
			if (info.second > right)
				stack.push(std::make_pair(right, info.second));

			if (!stack.empty()) {
				left = stack.top().first + 1;
				right = stack.top().second;
				quick_lr.pivot = &nums[stack.top().first];
			}
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::quick_sort_LL()
{
	int& outer = quick_ll.outer;
	int& inner = quick_ll.inner;
	auto& stack = quick_ll.stack;

	if (!stack.empty()) {
		if (inner < stack.top().second) {
			if (nums[inner] < *quick_ll.pivot) {
				outer++;
				std::swap(nums[outer], nums[inner]);
			}
			if (outer == -1)setActiveBars(nullptr, &colorMap[inner], nullptr);
			else setActiveBars(nullptr, &colorMap[inner], &colorMap[outer]);
			inner++;
		}
		else {
			outer++;
			auto info = stack.top();
			stack.pop();
			std::swap(nums[outer], nums[info.second]);

			if (outer - 1 > info.first)
				stack.push(std::make_pair(info.first, outer - 1));
			if (info.second > outer + 1)
				stack.push(std::make_pair(outer + 1, info.second));

			if (!stack.empty()) {
				outer = stack.top().first - 1;
				inner = stack.top().first;
				quick_ll.pivot = &nums[stack.top().second];
			}
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::merge_sort()
{
	auto& first = merge.first;
	auto& second = merge.second;
	auto& merged = merge.merged;
	auto& stack = merge.stack;
	auto& one = merge.one;
	auto& two = merge.two;

	if (!stack.empty()) {
		if (merged < nums.size())setActiveBars(&colorMap[merged], &colorMap[std::min(stack.top().first.first + first, stack.top().first.second - 1)], &colorMap[std::min(stack.top().second.first + second, stack.top().second.second - 1)]);
		if (first < one.size() && second < two.size()) {
			if (one[first] < two[second]) {
				nums[merged] = one[first];
				first++;
			}
			else {
				nums[merged] = two[second];
				second++;
			}
			merged++;
		}
		else if (first < one.size()) {
			nums[merged] = one[first];
			first++; merged++;
		}
		else if (second < two.size()) {
			nums[merged] = two[second];
			second++; merged++;
		}
		else {
			stack.pop();
			if (!stack.empty()) {
				one.clear(); two.clear();
				auto info = stack.top();
				merge.one.insert(merge.one.begin(), nums.begin() + info.first.first, nums.begin() + info.first.second);
				merge.two.insert(merge.two.begin(), nums.begin() + info.second.first, nums.begin() + info.second.second);

				merged = info.first.first;
				first = second = 0;
			}
		}
	}
	else setAlgorithm(NONE);
}

void Visualiser::merge_sort2()
{
	auto& first = merge2.first;
	auto& second = merge2.second;
	auto& merged = merge2.merged;
	auto& stack = merge2.stack;
	auto& one = merge2.one;
	auto& two = merge2.two;

	if (!stack.empty()) {
		if (merged < nums.size())setActiveBars(&colorMap[merged], &colorMap[std::min(stack.top().first.first + first, stack.top().first.second - 1)], &colorMap[std::min(stack.top().second.first + second, stack.top().second.second - 1)]);
		if (first < one.size() && second < two.size()) {
			if (one[first] < two[second]) {
				nums[merged] = one[first];
				first++;
			}
			else {
				nums[merged] = two[second];
				second++;
			}
			merged++;
		}
		else if (first < one.size()) {
			nums[merged] = one[first];
			first++; merged++;
		}
		else if (second < two.size()) {
			nums[merged] = two[second];
			second++; merged++;
		}
		else {
			stack.pop();
			if (!stack.empty()) {
				one.clear(); two.clear();
				auto info = stack.top();
				merge2.one.insert(merge2.one.begin(), nums.begin() + info.first.first, nums.begin() + info.first.second);
				merge2.two.insert(merge2.two.begin(), nums.begin() + info.second.first, nums.begin() + info.second.second);

				merged = info.first.first;
				first = second = 0;
			}
		}
	}
	else setAlgorithm(NONE);
}