#include "MainMenu.hpp"
#include "Main.hpp"
#include "imgui/imgui.h"
#include "State.hpp"
#include "Client.hpp"
MainMenu::MainMenu()
{
}

void MainMenu::DerivedUpdate()
{
	DisplayTitleScreen();
}
void MainMenu::DisplayTitleScreen()
{
	ImGuiIO &io = ImGui::GetIO();
	ImVec2 displaySize = io.DisplaySize;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(displaySize);

	ImGui::Begin("MainMenu", nullptr, windowFlags);

	if (ImGui::Button("Host"))
	{
		server = std::make_unique<Server>();
		server->Start(5000);
	}
	if (ImGui::Button("Join"))
	{
		char buf[64] = "127.0.0.1";

		ImGui::InputText("Enter IP", buf, sizeof(buf));

		if (ImGui::Button("Connect"))
		{
			unsigned int a, b, c, d;

			if (std::sscanf(buf, "%u.%u.%u.%u", &a, &b, &c, &d) == 4 &&
				a <= 255 && b <= 255 && c <= 255 && d <= 255)
			{
				std::uint32_t ip =
					(a << 24) |
					(b << 16) |
					(c << 8) |
					d;

				Client* c = new Client(state->renderTarget,Player());
				c->socket.connect(sf::IpAddress(ip), 5000);
				state = std::unique_ptr<Kosmic::State>(c);
			}
		}
	}

	ImGui::End();
}

MainMenu::~MainMenu()
{
}