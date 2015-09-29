#include "App.hpp"
#include "Histogram.hpp"
#include "Log.hpp"


App::App(const std::string &filepath, Algorithm algorithm)
: App(filepath, algorithm, 0.0f)
{
}


App::App(const std::string &filepath, Algorithm algorithm, float lambda)
: m_algorithm(algorithm), m_lambda(lambda)
{
	Log << "Initializing SDL..." << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(SDL_GetError());
	}

	Log << "Loading input file..." << std::endl;
	if (!(m_picture = IMG_Load(filepath.c_str()))) {
		throw std::runtime_error(SDL_GetError());
	}

	Log << "Preparing selection surface..." << std::endl;
	if (!(m_selection = SDL_CreateRGBSurface(0, width(), height(), 32,
	                                         0x000000ff,
	                                         0x0000ff00,
	                                         0x00ff0000,
	                                         0xff000000)))
	{
		throw std::runtime_error(SDL_GetError());
	}

	Log << "Preparing segmentation surface..." << std::endl;
	if (!(m_segmentation = SDL_CreateRGBSurface(0, width(), height(), 32,
	                                            0x000000ff,
	                                            0x0000ff00,
	                                            0x00ff0000,
	                                            0xff000000)))
	{
		throw std::runtime_error(SDL_GetError());
	}

	Log << "Creating SDL window..." << std::endl;
	if (!(m_window = SDL_CreateWindow("Graph cut", 
	                                  SDL_WINDOWPOS_UNDEFINED,
	                                  SDL_WINDOWPOS_UNDEFINED,
	                                  width(), height(),
	                                  SDL_WINDOW_SHOWN)))
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_mode = Mode::NORMAL;
	m_display = DISPLAY_SELECTION | DISPLAY_SEGMENTATION;
}


App::~App()
{
	Log << "Destroying application context..." << std::endl;
	SDL_FreeSurface(m_picture);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}


void App::run()
{
	while (m_mode != Mode::QUIT) {
		update();
		draw();
	}
}


void App::draw()
{
	SDL_Surface *screen = SDL_GetWindowSurface(m_window);
	SDL_BlitSurface(m_picture, nullptr, screen, nullptr);

	if (m_display & DISPLAY_SELECTION) {
		SDL_BlitSurface(m_selection, nullptr, screen, nullptr);
	}

	if (m_display & DISPLAY_SEGMENTATION) {
		SDL_BlitSurface(m_segmentation, nullptr, screen, nullptr);
	}

	SDL_UpdateWindowSurface(m_window);
}


void App::update()
{
	handle_events();
	paint_selection();
}


void App::handle_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_mode = Mode::QUIT;
			break;
		case SDL_KEYDOWN:
			handle_key_down(event.key.keysym.sym);
			break;
		case SDL_MOUSEMOTION:
			handle_mouse_motion(event.motion);
			break;
		case SDL_MOUSEBUTTONUP:
			handle_mouse_up(event.button);
			break;
		case SDL_MOUSEBUTTONDOWN:
			handle_mouse_down(event.button);
			break;
		}
	}
}


void App::handle_key_down(SDL_Keycode key)
{
	switch (key) {
	case SDLK_s:
		m_display ^= DISPLAY_SELECTION;
		break;
	case SDLK_d:
		m_display ^= DISPLAY_SEGMENTATION;
		break;
	case SDLK_SPACE:
		segment();
		break;
	case SDLK_ESCAPE:
		m_mode = Mode::QUIT;
		break;
	}
}


void App::handle_mouse_down(const SDL_MouseButtonEvent &event)
{
	switch (event.button) {
	case SDL_BUTTON_LEFT:
		m_mode = Mode::SELECT_OBJECT;
		break;
	case SDL_BUTTON_RIGHT:
		m_mode = Mode::SELECT_BACKGROUND;
		break;
	case SDL_BUTTON_MIDDLE:
		m_mode = Mode::ERASE;
		break;
	}
}


void App::handle_mouse_up(const SDL_MouseButtonEvent &event)
{
	(void)event;
	m_mode = Mode::NORMAL;
}


void App::handle_mouse_motion(const SDL_MouseMotionEvent &event)
{
	m_mouse_x = event.x;
	m_mouse_y = event.y;
}


void App::paint_selection()
{
	if (m_mode == Mode::NORMAL) {
		return;
	}

	uint8_t r = 0, g = 0, b = 0, a = 255;
	switch (m_mode) {
	case Mode::SELECT_OBJECT:
		r = 255;
		break;
	case Mode::SELECT_BACKGROUND:
		b = 255;
		break;
	case Mode::ERASE:
		a = 0;
		break;
	default:
		break;
	}

	uint32_t brush = std::round(size() * BRUSH_FACTOR);
	SDL_Rect rect;
	rect.x = m_mouse_x - brush / 2;
	rect.y = m_mouse_y - brush / 2;
	rect.w = brush;
	rect.h = brush;

	uint32_t color = SDL_MapRGBA(m_selection->format, r, g, b, a);
	SDL_FillRect(m_selection, &rect, color);
}


void App::segment()
{
	Image<Selection> object(GraphCut::execute(picture_to_image(),
	                                          selection_to_image(),
	                                          m_algorithm, m_lambda));

	uint8_t bpp = m_segmentation->format->BytesPerPixel;
	uint8_t *pixels = static_cast<uint8_t *>(m_segmentation->pixels);

	for (size_t i = 0; i < width() * height(); i++) {
		pixels[bpp * i + 0] = 0;
		pixels[bpp * i + 1] = 0;
		pixels[bpp * i + 2] = 0;
		pixels[bpp * i + 3] = 128;

		switch (object.data()[i]) {
		case Selection::OBJECT:
			pixels[bpp * i + 0] = 255;
			break;
		case Selection::BACKGROUND:
			pixels[bpp * i + 2] = 255;
			break;
		case Selection::NONE:
			pixels[bpp * i + 3] = 0;
			break;
		}
	}
}


Image<float> App::picture_to_image() const
{
	uint8_t bpp = m_picture->format->BytesPerPixel;
	uint8_t *pixels = static_cast<uint8_t *>(m_picture->pixels);

	Image<float> image(width(), height());
	for (size_t i = 0; i < width() * height(); i++) {
		float r = pixels[bpp * i + 0] * 0.3f;
		float g = pixels[bpp * i + 1] * 0.6f;
		float b = pixels[bpp * i + 2] * 0.1f;
		image.data()[i] = (r + g + b) / 255.0f;
	}
	return std::move(image);
}


Image<Selection> App::selection_to_image() const
{
	uint8_t bpp = m_selection->format->BytesPerPixel;
	uint8_t *pixels = static_cast<uint8_t *>(m_selection->pixels);

	Image<Selection> image(width(), height());
	for (size_t i = 0; i < width() * height(); i++) {
		float r = pixels[bpp * i + 0];
		float b = pixels[bpp * i + 2];
		if (r == 255) {
			image.data()[i] = Selection::OBJECT;
		} else if (b == 255) {
			image.data()[i] = Selection::BACKGROUND;
		} else {
			image.data()[i] = Selection::NONE;
		}
	}
	return std::move(image);
}
