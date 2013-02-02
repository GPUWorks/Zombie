#ifndef ZOMBIEWINDOW_H
#define ZOMBIEWINDOW_H

#include "mw/gamewindow.h"
#include "mw/sprite.h"
#include "zombiegame.h"

#include <SDL_opengl.h>
#include <iostream>

namespace zombie {

class ZombieWindow : public mw::GameWindow {
public:
    ZombieWindow() : mw::GameWindow(400,400,"Zombie","images/icon.bmp") {
        setResizable(false);
        setTimeStep(20);

		setUnicodeInputEnable(true);
		reshapeWindowsOpenGL();
    }

	~ZombieWindow() {
	}

	void startLocal() {
		zombieGame_.connect(1,STATUS_LOCAL);		
	}

	void startServer() {
		zombieGame_.connect(1,STATUS_SERVER);		
	}

	void startClient() {
		zombieGame_.connect(1,STATUS_CLIENT);		
	}
private:
    // Override mw::GameWindow
    void graphicUpdate(Uint32 msDeltaTime) {
		zombieGame_.graphicUpdate(msDeltaTime);
    }

    // Override mw::GameWindow
    void physicUpdate(Uint32 msDeltaTime) {		
		zombieGame_.physicUpdate(msDeltaTime);
    }

    // Override mw::GameWindow
    void eventUpdate(const SDL_Event& windowEvent) {
		zombieGame_.eventUpdate(windowEvent);

		switch (windowEvent.type) {
		case SDL_QUIT:
			setQuiting(true);
			break;
        case SDL_KEYDOWN:
            switch (windowEvent.key.keysym.sym) {
            case SDLK_t:
                mw::Window::setWindowsSize(100,100);
                //SDL_SetVideoMode(300,300, 32, SDL_OPENGL);
                //SDL_WM_ToggleFullScreen();
                break;
			case SDLK_F2:
				break;
			case SDLK_RETURN:
				zombieGame_.startGame();
				break;
			case SDLK_p:
				// Fall through.
			case SDLK_PAUSE:
				break;
			case SDLK_ESCAPE:
				setQuiting(true);
				break;
            default:
                break;
			}
			break;
        default:
            break;
		}
	}
	
    // Override mw::GameWindow
    void resize(int width, int height) {
        reshapeWindowsOpenGL();
    }

    void reshapeWindowsOpenGL() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        int w = getWidth();
        int h = getHeight();

        glViewport(0,0,w,h);
        glOrtho(0,50,0,50,-1,1);
        std::cout << "\nreshapeWindowsOpenGL" << std::endl;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

	ZombieGame zombieGame_;	
};

} // namespace zombie

#endif // ZOMBIEWINDOW_H
