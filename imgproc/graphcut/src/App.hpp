#ifndef APP_HPP
#define APP_HPP


#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Image.hpp"
#include "GraphCut.hpp"


enum class Mode {
	NORMAL,
	SELECT_OBJECT,
	SELECT_BACKGROUND,
	ERASE,
	QUIT
};


class App {
public:
	explicit App(const std::string &filepath,
	             Algorithm algorithm);
	explicit App(const std::string &filepath,
	             Algorithm algorithm, float lambda);
	~App();

	void run();

private:
	static constexpr float BRUSH_FACTOR = 0.02f;
	static constexpr uint8_t DISPLAY_SELECTION = 0x01;
	static constexpr uint8_t DISPLAY_SEGMENTATION = 0x02;

	Algorithm m_algorithm;
	float m_lambda;

	SDL_Window *m_window;

	SDL_Surface *m_picture;
	SDL_Surface *m_selection;
	SDL_Surface *m_segmentation;

	Mode m_mode;
	uint8_t m_display;
	size_t m_mouse_x, m_mouse_y;

	uint32_t width() const {
		return m_picture->w;
	}

	uint32_t height() const {
		return m_picture->h;
	}

	uint32_t size() const {
		return std::min(width(), height());
	}

	void draw();
	void update();
	void handle_events();
	void handle_key_down(SDL_Keycode key);
	void handle_mouse_motion(const SDL_MouseMotionEvent &event);
	void handle_mouse_up(const SDL_MouseButtonEvent &event);
	void handle_mouse_down(const SDL_MouseButtonEvent &event);

	Image<float> picture_to_image() const;
	Image<Selection> selection_to_image() const;

	void paint_selection();
	void segment();
};


#endif
