#pragma once

#include <vector>

extern "C" {
	#include <libavutil/frame.h>
}

#include "global.hpp"

typedef struct Frames {
	std::vector<AVFrame*> frames;
	void push(AVFrame* frame) {
		const auto fr = av_frame_clone(frame);
		assert_v(fr != nullptr, "Frames", "Could not clone frame");
		frames.push_back(fr);
	}
	Frames(void) {
		frames.reserve(4096 * 4);
	}
	~Frames(void) {
		for (auto& frame : frames)
			av_frame_free(&frame);
	}
} Frames;

/*!
 * \brief FFmpeg interface
 */
class FFmpeg {
public:
	FFmpeg(const string filename);
	~FFmpeg(void);

public:
	const Frames* decode(void) const;
	void encode(const Frames* frames) const;

private:
	string filename;
};
