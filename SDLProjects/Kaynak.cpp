#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
using namespace std;

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "HATA", "SDL Baþlatýlamadý!", NULL);
		return EXIT_FAILURE;
	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {
		cout << "SDL_image baþlatma hatasý: " << IMG_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	int screenWidth = 1920;
	int screenHeight = 1080;

	SDL_Window* window = SDL_CreateWindow(
		"SDL DENEME ROBOTIC BOOTCAMP",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_FULLSCREEN
	);

	if (window == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "HATA", "Pencere Oluþturulamadý!", NULL);;
		return EXIT_FAILURE;
	}

	SDL_Renderer* cizimalani = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (cizimalani == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "HATA", "Ýþleyici Alan Oluþturulamadý!", NULL);
		if (window != nullptr) {
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Surface* zeminresmi = IMG_Load("karincayiyen.jpg");
	SDL_Surface* butonresmi = IMG_Load("buton.png");
	if (zeminresmi == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "HATA", "Resim Yüklenemedi!", NULL);
		if (cizimalani != nullptr) {
			SDL_DestroyRenderer(cizimalani);
		}
		if (window != nullptr) {
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Texture* imgtexture = SDL_CreateTextureFromSurface(cizimalani, zeminresmi);
	SDL_Texture* buttexture = SDL_CreateTextureFromSurface(cizimalani, butonresmi);
	if (imgtexture == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "HATA", "Kaplama Dokusu Oluþturulamadý!", NULL);
		if (zeminresmi != nullptr) {
			SDL_FreeSurface(zeminresmi);
		}
		if (cizimalani != nullptr) {
			SDL_DestroyRenderer(cizimalani);
		}
		if (window != nullptr) {
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
		return EXIT_FAILURE;
	}

	if (TTF_Init() < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "HATA", "TTF Baþlatýlamadý!", NULL);
	}

	TTF_Font* Font = TTF_OpenFont("Dragon.ttf", 32);
	if (Font == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "HATA", "Font Yüklenemedi!", NULL);
	}

	SDL_Color textColor = { 0, 0, 0 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(Font, "BELLIGOOOOAL", textColor);
	if (!textSurface) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "HATA", "Yazý Yüzeyi Oluþturulamadý!", NULL);
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(cizimalani, textSurface);
	if (!textTexture) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "HATA", "Yazý Texture'u Oluþturulamadý!", NULL);
	}

	SDL_Rect butalan = { 1800, 10, 100, 100 };
	SDL_Rect rect;
	rect.x = 50;
	rect.y = 100;
	rect.w = 200;
	rect.h = 200;

	SDL_Rect textRect;
	textRect.x = screenWidth / 2;
	textRect.y = screenHeight / 2;
	textRect.w = textSurface->w;
	textRect.h = textSurface->h;
	
	bool quit = false;
	bool yazidurumu = false;
	SDL_Event event;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					int x = event.button.x;
					int y = event.button.y;
					if (x > 1815 && x < 1885 && y > 30 && y < 95) {
						yazidurumu = true;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					yazidurumu = false;
				}
				break;
			default:
				break;
			}
		}
		SDL_RenderClear(cizimalani);
		SDL_RenderCopy(cizimalani, imgtexture, NULL, NULL);
		SDL_SetRenderDrawColor(cizimalani, 0, 0, 0, 0);
		SDL_RenderDrawLine(cizimalani, 50, 50, 350, 50);
		SDL_RenderDrawRect(cizimalani, &rect);

		int parcasayisi = 500;
		int centerX = 150, centerY = 380;
		int radius = 50;
		for (int i = 0; i < parcasayisi; i++) {
			double aci = 2.0 * M_PI * i / parcasayisi;
			int x1 = static_cast<int>(centerX + radius * cos(aci));
			int y1 = static_cast<int>(centerY + radius * sin(aci));
			int x2 = static_cast<int>(centerX + radius * cos(2.0 * M_PI * (i + 1) / parcasayisi));
			int y2 = static_cast<int>(centerY + radius * sin(2.0 * M_PI * (i + 1) / parcasayisi));

			SDL_RenderDrawLine(cizimalani, x1, y1, x2, y2);
		}
		if (yazidurumu == true) {
			SDL_RenderCopy(cizimalani, textTexture, nullptr, &textRect);
		}
		SDL_RenderCopy(cizimalani, buttexture, NULL, &butalan);
		SDL_RenderPresent(cizimalani);
	}

	SDL_DestroyTexture(imgtexture);
	SDL_DestroyRenderer(cizimalani);
	SDL_DestroyWindow(window);
	SDL_Quit();


    return 0;
}

/*
if (yazidurumu) {


	SDL_FreeSurface(textSurface);
	

	

}*/