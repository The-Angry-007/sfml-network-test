#include "Main.hpp"
#include "MainMenu.hpp"
// #include "Civitron/ItemInfo.hpp"
#include "Input/Input.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"
#include <fstream>
#include <iostream>
#include "Server.hpp"
// #include "Civitron/EntityInfo.hpp"
// a unique pointer to the window object; this is unique to prevent accidentally creating multiple windows
std::unique_ptr<sf::RenderWindow> window;
// width and height of the window
int width = 1280;
int height = 720;
std::unique_ptr<Kosmic::State> state;
std::unique_ptr<Server> server;

// the main procedure that runs the program
int main()
{
	srand(time(NULL));
	window = std::make_unique<sf::RenderWindow>(sf::VideoMode({(unsigned int)width, (unsigned int)height}), "Cosmic Construction II");
	window->setFramerateLimit(9999);
	window->setVerticalSyncEnabled(false);
	// set the icon image that is displayed in the corner of the window
	sf::Image icon;
	if (!icon.loadFromFile("content/resources/images/icon.png"))
	{
		return 1;
	}

	window->setIcon({256, 256}, icon.getPixelsPtr());
	if (!ImGui::SFML::Init(*window))
		return -1;
	Input input;
	state = std::unique_ptr<Kosmic::State>(new MainMenu());
	state->renderTarget = window.get();
	sf::Clock deltaClock;

	// io.Fonts->AddFontDefault(&config);
	auto &io = ImGui::GetIO();
	io.Fonts->Clear();
	ImFont *font = io.Fonts->AddFontFromFileTTF("content/resources/fonts/default font.ttf", 20.f);
	if (!ImGui::SFML::UpdateFontTexture())
	{
		window->close();
	};
	// macro.ParseFile("content/resources/macro2.txt");

	// tgui::Button::Ptr button = tgui::Button::create();
	// gui.add(button);
	while (window->isOpen())
	{
		InputState inputState = input.ProcessEvents(*window);
		auto time = deltaClock.restart();
		double dt = time.asSeconds();
		ImGui::SFML::Update(*window, time);
		inputState.DrawToWindow();
		if (server.get() != nullptr)
		{
			server->Update();
		}
		if (state.get() != nullptr)
		{
			state->Update(inputState, dt);
		}
		window->clear(sf::Color(0, 0, 0));
		// window->clear(sf::Color(8, 38, 19));
		if (state.get() != nullptr)
		{
			state->Render();
		}
		ImGui::SFML::Render(*window);

		window->display();
	}
	return 0;
}
