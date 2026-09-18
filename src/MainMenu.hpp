#pragma once
#include "State.hpp"
	class MainMenu : public Kosmic::State
	{
	public:
		bool inJoinMenu = false;
		MainMenu();
		void DisplayTitleScreen();
		void DerivedUpdate();
		~MainMenu();
	};