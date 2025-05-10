#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <curl/curl.h>
#include "arial.h"
#include <windows.h>
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
	output->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static std::vector<std::string> LoadWords(const std::string& base_url)
{
	CURL* curl = curl_easy_init();
	std::string response;

	if (curl) {
		std::string url = base_url + "?t=" + std::to_string(time(0));

		struct curl_slist* headers = nullptr;
		
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) 
		{
			MessageBox(
				NULL,                               
				L"Check your internet conection",
				L"Connect-Erorr",     
				MB_OK | MB_ICONWARNING             
			);
		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}

	std::vector<std::string> words;
	size_t start = 0, end = response.find('\n');
	while (end != std::string::npos) {
		words.push_back(response.substr(start, end - start));
		start = end + 1;
		end = response.find('\n', start);
	}
	if (start < response.size()) words.push_back(response.substr(start));

	return words;
}

static void clearVector(std::vector<std::string>& words)
{
	std::erase_if(words, [](const auto& s) { return s.length() <= 4; });
}

static void createVector(std::vector<std::string>& words)
{
	std::string base_url = "https://raw.githubusercontent.com/SigmaGit1eR/ZenType-Words/refs/heads/main/ZenType_Words.txt";
	words = LoadWords(base_url);
	clearVector(words);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(words.begin(), words.end(), gen);
}
unsigned int typedWords = 0;
struct word
{
public:
	word();
	void next();
	char needToEnter;
	std::string getWord();
private:
	std::vector<std::string> words;
	unsigned int charNumber = 0;
	unsigned int stringSize;
	
};
word::word()
{
	createVector(words);
	stringSize = words[0].size() - 1;
	needToEnter = words[0][0];
}
void word::next()
{
	charNumber++;
	if (charNumber == stringSize)
	{
		charNumber = 0;
		words.erase(words.begin());
		stringSize = words[0].size()-1;
		typedWords++;
	}
	needToEnter = words[0][charNumber];
}

std::string word::getWord()
{
	return words[0].substr(charNumber);
}

int main()
{
	word WORD;

	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;

	const std::wstring WINDOW_NAME = L"ZenType";
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), WINDOW_NAME);

	bool isFullscreen = false;
	sf::Vector2u lastWindowSize(windowWidth, windowHeight);
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

	const sf::Color TEXT_COLOR(sf::Color::White);
	const sf::Color BACKROUND_COLOR(sf::Color::Black);

	sf::Font font;
	font.loadFromMemory(arial_ttf, arial_ttf_len);

	bool isRunning = false;
	sf::Clock clock;
	sf::Time elapsedTime;
	float totalTime=0.0f;

	unsigned int totalCharacters = 0;
	unsigned int correctCharacters = 0;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::Resized && !isFullscreen) {
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}

			if (event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space)
			{
				if (isRunning)
				{
					elapsedTime = clock.getElapsedTime();
					totalTime += elapsedTime.asSeconds();
				}
				else
				{
					clock.restart();
				}
				isRunning = !isRunning;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11) 
			{
				if (isFullscreen)
				{
					window.create(sf::VideoMode(lastWindowSize.x, lastWindowSize.y), WINDOW_NAME, sf::Style::Default);
				}
				else
				{
					lastWindowSize = window.getSize();
					window.create(desktopMode, WINDOW_NAME, sf::Style::Fullscreen);
				}
				isFullscreen = !isFullscreen;
			}
			if (isRunning)
			{
				if (event.type == sf::Event::TextEntered)
				{
					if (static_cast<char>(event.text.unicode) == WORD.needToEnter || static_cast<char>(event.text.unicode) == std::toupper(WORD.needToEnter))
					{
						WORD.next();
						correctCharacters++;
					}
					if (event.text.unicode != ' ')
					{
						totalCharacters++;
					}
				}
			}
		}
		
		if (isRunning)
		{
			elapsedTime = clock.getElapsedTime();
		}
		
		float currentTime = totalTime + float((isRunning ? elapsedTime.asSeconds() : 0));
		float speed = float(correctCharacters / (currentTime==0?1:currentTime));
		float percent = float(correctCharacters) / float((totalCharacters==0?1:totalCharacters))*100.0f;

		window.clear(BACKROUND_COLOR);

		
		sf::Vector2u WinSize = window.getSize();

		sf::Text text((isRunning?WORD.getWord() : "Press space to start"), font, WinSize.y / 7.5f);
		sf::FloatRect textBounds = text.getLocalBounds();
		text.setOrigin(textBounds.left + textBounds.width / 2.0f,
			textBounds.top + textBounds.height / 2.0f);
		text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
		window.draw(text);
		
		std::string info = "Accuracy: "+std::to_string(percent)+"\n"+
			"Speed (Char/Sec): "+std::to_string(speed)+"\n"+
			"Typed words: "+std::to_string(typedWords);
		sf::Text info_text(info, font, WinSize.y / 30);
		info_text.setPosition(0, 0);
		window.draw(info_text);

		window.display();
	}
}