// TODO: REPLACE BX WITH INNER ROUTINES
#include "Font.hpp"
#include "Logger.hpp"
#include "MathFuncs.hpp"
#include "NoobUtils.hpp"
#include "NoobDefines.hpp"
#include <bx/fpumath.h>

noob::font::~font()
{
	// text_buffer_manager->destroyTextBuffer(transient_text);
	// delete font_manager;
	// delete text_buffer_manager;
}

void noob::font::init(const std::string& font_path)
{
	logger::log("Font init begin");
	font_manager = new FontManager(1024);
	text_buffer_manager = new TextBufferManager(font_manager);
	transient_text = text_buffer_manager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Transient);

	// Instantiate a usable font.
	TrueTypeHandle fontfile_handle = load_ttf(font_manager, font_path.c_str());
	ttf_fonthandle = font_manager->createFontByPixelSize(fontfile_handle, 0, 32);
	// Preload glyphs and blit them to atlas.
	font_manager->preloadGlyph(ttf_fonthandle, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890-=~!@#$%^&*()_+{}[]|\\\"';:<>?,./ ");

	// You can unload the truetype files at this stage, but in that case, the set of glyph's will be limited to the set of preloaded glyph.
	//font_manager->destroyTtf(fontfile_handle);
	logger::log("Font init end");
}


void noob::font::change_colour(uint32_t colour)
{
	text_buffer_manager->setTextColor(transient_text, colour);
}


void noob::font::draw_text(const std::string& str, float x_pos, float y_pos)
{
	text_buffer_manager->clearTextBuffer(transient_text);
	text_buffer_manager->setPenPosition(transient_text, x_pos, y_pos);
	text_buffer_manager->appendText(transient_text, ttf_fonthandle, str.c_str());

	float at[3]  = { 0, 0,  0.0f };
	float eye[3] = { 0, 0, -1.0f };
	float view[16];
	bx::mtxLookAt(view, eye, at);

	const float centering = 0.5f;
	float ortho[16];
	bx::mtxOrtho(ortho, centering, window_width + centering, window_height + centering, centering, -1.0f, 1.0f);

	bgfx::setViewTransform(UI_TEXT_LAYER, view, ortho);
	bgfx::setViewRect(UI_TEXT_LAYER, 0, 0, window_width, window_height);
	text_buffer_manager->submitTextBuffer(transient_text, UI_TEXT_LAYER);
}


void noob::font::set_window_size(uint32_t width, uint32_t height)
{
	window_width = width;
	window_height = height;
}


TrueTypeHandle noob::font::load_ttf(FontManager* _fm, const char* _filePath)
{
	FILE* file = fopen(_filePath, "rb");
	if (NULL != file)
	{
		uint32_t size = (uint32_t)noob::utils::fsize(file);
		uint8_t* mem = (uint8_t*)malloc(size+1);
		size_t ignore = fread(mem, 1, size, file);
		BX_UNUSED(ignore);
		fclose(file);
		mem[size-1] = '\0';
		TrueTypeHandle handle = _fm->createTtf(mem, size);
		free(mem);
		return handle;
	}
	TrueTypeHandle invalid = BGFX_INVALID_HANDLE;
	return invalid;
}
