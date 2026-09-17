#include "MainMenu.hpp"
#include "Main.hpp"
#include "imgui/imgui.h"
#include "State.hpp"
	MainMenu::MainMenu()
	{
		currentState = TITLE_SCREEN;
	}

	void MainMenu::DerivedUpdate()
	{
		if (currentState == TITLE_SCREEN)
		{
			DisplayTitleScreen();
		}
		if (inputState.Pressed(sf::Keyboard::Key::Escape) && (currentState != TITLE_SCREEN))
		{
			currentState = TITLE_SCREEN;
		}
	}
	void MainMenu::DisplayTitleScreen()
	{
		ImGuiIO &io = ImGui::GetIO();
		ImVec2 displaySize = io.DisplaySize;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(displaySize);

		ImGui::Begin("MainMenu", nullptr, windowFlags);

		// Font scaling
		float originalFontScale = ImGui::GetFont()->Scale;
		ImGui::SetWindowFontScale(3.0f); // Double font size

		const char *title = "Cosmic Construction II";
		ImVec2 textSize = ImGui::CalcTextSize(title);
		// ImGui::SetCursorPos(ImVec2((displaySize.x - textSize.x) * 0.5f, displaySize.y * 0.2f));
		ImGui::TextUnformatted(title);

		ImGui::SetWindowFontScale(1.0f); // Reset font scale

		// Buttons
		ImVec2 buttonSize(200, 50);
		ImVec2 windowCenter = ImVec2(displaySize.x * 0.5f, displaySize.y * 0.5f);

		// ImGui::SetCursorPos(ImVec2(windowCenter.x - buttonSize.x * 0.5f, windowCenter.y - buttonSize.y - 10));
		if (ImGui::Button("New Game", buttonSize))
		{
			strcpy(saveName, "");
			strcpy(seed, "");
			currentState = NEW_GAME;
		}

		// ImGui::SetCursorPos(ImVec2(windowCenter.x - buttonSize.x * 0.5f, windowCenter.y + 10));
		if (ImGui::Button("Load Game", buttonSize))
		{
		}

		// ImGui::SetCursorPos(ImVec2(windowCenter.x - buttonSize.x * 0.5f, windowCenter.y + 20 + buttonSize.y));

		ImGui::End();
	}

	MainMenu::~MainMenu()
	{
	}