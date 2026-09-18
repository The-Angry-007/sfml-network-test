#pragma once
#include "State.hpp"
	class MainMenu : public Kosmic::State
	{
	public:

		MainMenu();
		void DisplayTitleScreen();
		void DerivedUpdate();
		~MainMenu();
	};