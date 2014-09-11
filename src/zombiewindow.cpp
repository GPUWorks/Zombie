#include "zombiewindow.h"
#include "zombiegame.h"
#include "gamedata.h"
#include "gamedataentry.h"

#include <gui/borderlayout.h>
#include <gui/horizontallayout.h>
#include <gui/button.h>
#include <gui/frame.h>
#include <gui/panel.h>
#include <gui/label.h>
#include <gui/verticallayout.h>

namespace zombie {

	namespace {

		void handleKeyboard(gui::Component& c, const SDL_Event& keyEvent) {
			ZombieGame& zombieGame = static_cast<ZombieGame&>(c);

			switch (keyEvent.type) {
				case SDL_KEYDOWN:
					switch (keyEvent.key.keysym.sym) {
						case SDLK_ESCAPE:
							break;
						case SDLK_PAGEUP:
							zombieGame.zoom(1.1f);
							break;
						case SDLK_PAGEDOWN:
							zombieGame.zoom(1 / 1.1f);
							break;
						case SDLK_RETURN:
							zombieGame.startGame();
							break;
						case SDLK_p:
							// Fall through.
						case SDLK_PAUSE:
							break;
					}
			}
		}

		std::shared_ptr<gui::Button> createButton(std::string str, const mw::Font& font) {
			auto button = std::make_shared<gui::Button>(str, font);
			button->setAutoSizeToFitText(true);			
			button->setHoverColor(0.7f, 0, 0, 0.5);
			button->setFocusColor(0.8f, 0, 0, 0.5);
			button->setPushColor(0.9f, 0, 0, 0.5);
			return button;
		}

	} // Anonymous namespace.

	ZombieWindow::ZombieWindow(GameData& gameData) : gui::Frame(gameData.getWindowXPosition(), gameData.getWindowYPosition(), gameData.getWindowWidth(), gameData.getWindowHeight(), true, "Zombie", "images/1.png"), gameData_(gameData) {
		SDL_SetWindowMinimumSize(mw::Window::getSdlWindow(), 400, 400);
		if (gameData.isWindowMaximized()) {
			SDL_MaximizeWindow(mw::Window::getSdlWindow());
		}

		addWindowListener([&](gui::Frame& frame, const SDL_Event& sdlEvent) {
			switch (sdlEvent.type) {
				case SDL_WINDOWEVENT:
					switch (sdlEvent.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
							if (!(SDL_GetWindowFlags(mw::Window::getSdlWindow()) & SDL_WINDOW_MAXIMIZED)) {
								// The Window's is not maximized. Save size!
								gameData.setWindowSize(sdlEvent.window.data1, sdlEvent.window.data2);
							}
							break;
						case SDL_WINDOWEVENT_MOVED:
							if (!(SDL_GetWindowFlags(mw::Window::getSdlWindow()) & SDL_WINDOW_MAXIMIZED)) {
								// The Window's is not maximized. Save position!
								int x, y;
								SDL_GetWindowPosition(mw::Window::getSdlWindow(), &x, &y);
								gameData.setWindowPosition(x, y);
							}
							break;
						case SDL_WINDOWEVENT_MAXIMIZED:
							gameData.setWindowMaximized(true);
							break;
						case SDL_WINDOWEVENT_RESTORED:
							gameData.setWindowMaximized(false);
							break;
					}
					break;
			}
		});

		zombieGame_ = std::make_shared<ZombieGame>(gameData);
		// Always react on key events!
		zombieGame_->setGrabFocus(true);

		menuFrameIndex_ = getCurrentPanelIndex();
		playFrameIndex_ = addPanelBack();
		customGameFrameIndex_ = addPanelBack();
		highscoreFrameIndex_ = addPanelBack();

		initMenuFrame();
		initplayFrame();
		initCustomplayFrame();
		initHighscoreFrame();
		setCurrentPanel(menuFrameIndex_);

		setDefaultClosing(true);

		zombieGame_->addKeyListener(std::bind(&handleKeyboard, std::placeholders::_1, std::placeholders::_2));
	}

	void ZombieWindow::initMenuFrame() {
		setCurrentPanel(menuFrameIndex_);
		auto panel = std::make_shared<gui::Panel>();
		add(panel, gui::BorderLayout::CENTER);
		panel->setLayout(std::make_shared<gui::VerticalLayout>());
		panel->setBackground(gameData_.getEntry("menu").getSprite("backGroundImage"));
		mw::Font font = gameData_.getDefaultFont(20);

		auto text = std::make_shared<gui::Label>("Zombie", gameData_.getDefaultFont(60));
		text->setTextColor(1, 1, 1);
		panel->addToGroup(text);

		auto play = createButton("Play", font);
		play->addActionListener([&](gui::Component&) {
			setCurrentPanel(playFrameIndex_);
			//gameData_.getMenuChoiceSound().play();
		});
		panel->addToGroup(play);

		auto customGame = createButton("Custom game", font);
		customGame->addActionListener([&](gui::Component&) {
			gameData_.getEntry("menu").getSound("soundChoice").play();
		});
		panel->addToGroup(customGame);

		auto highScore = createButton("Highscore", font);
		highScore->addActionListener([&](gui::Component&) {
			gameData_.getEntry("menu").getSound("soundChoice").play();
		});
		panel->addToGroup(highScore);

		auto quit = createButton("Quit", font);
		quit->addActionListener([&](gui::Component&) {
			gameData_.getEntry("menu").getSound("soundChoice").play();
			Window::quit();
		});
		panel->addToGroup(quit);
	}	

	void ZombieWindow::initplayFrame() {
		setCurrentPanel(playFrameIndex_);

		add(zombieGame_, gui::BorderLayout::CENTER);
		gamePanel_ = std::make_shared<gui::Panel>();
		gamePanel_->setPreferredSize(100, 50);
		add(gamePanel_, gui::BorderLayout::SOUTH);
		gamePanel_->setBackgroundColor(0, 0, 0);
		mw::Font font = gameData_.getDefaultFont(15);

		//panel->add(createButton("Button", font));
		label1_ = std::make_shared<gui::Label>("Kill count: 0", font);
		label2_ = std::make_shared<gui::Label>("Health: ?", font);
		label3_ = std::make_shared<gui::Label>("Ammo: ?", font);
		label4_ = std::make_shared<gui::Label>("Live units: ?", font);
		fps_ = std::make_shared<gui::Label>("Fps: ", font);
		label1_->setTextColor(1,0,0);
		label2_->setTextColor(1, 0, 0);
		label3_->setTextColor(1, 0, 0);
		label4_->setTextColor(1, 0, 0);
		fps_->setTextColor(1, 0, 0);
		gamePanel_->add(label1_);
		gamePanel_->add(label2_);
		gamePanel_->add(label3_);
		gamePanel_->add(label4_);
		gamePanel_->add(fps_);
		addUpdateListener([&](gui::Frame& frame, Uint32 deltaTime) {
			std::stringstream stream;
			stream << "Kill count: " << zombieGame_->getZombiesKilled();
			label1_->setText(stream.str());
			gamePanel_->validate();
		});
		addUpdateListener([&](gui::Frame& frame, Uint32 deltaTime) {
			std::stringstream stream;
			stream << "Health: " << zombieGame_->getHealth() << "/100";
			label2_->setText(stream.str());
			gamePanel_->validate();
		});
		addUpdateListener([&](gui::Frame& frame, Uint32 deltaTime) {
			std::stringstream stream;
			stream << "Ammo: " << zombieGame_->getBulletsInWeapon() << "/" << zombieGame_->getClipSize();
			label3_->setText(stream.str());
			gamePanel_->validate();
		});
		addUpdateListener([&](gui::Frame& frame, Uint32 deltaTime) {
			std::stringstream stream;
			stream << "Live units: " << zombieGame_->getNbrUnits();
			label4_->setText(stream.str());
			gamePanel_->validate();
		});
		addUpdateListener([&](gui::Frame& frame, Uint32 deltaTime) {
			std::stringstream stream;
			stream << "Fps: " << zombieGame_->getFps();
			fps_->setText(stream.str());
		});
	}

	void ZombieWindow::initCustomplayFrame() {
		setCurrentPanel(customGameFrameIndex_);
	}

	void ZombieWindow::initHighscoreFrame() {
		setCurrentPanel(highscoreFrameIndex_);
	}

} // Namespace zombie.
