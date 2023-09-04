/* 
 * Task: Decode video from file, flip it horizontally, encode it and save to file
 * mkv —(ffmpeg)—>{NvDEC—>Flip,Flop—>NvENC—>ffmpeg}—>out.mkv
 * Note: {} - GPU memory
 */

#include "../include/global.hpp"
#include "../include/ffmpeg.hpp"

auto main(const int32_t argc, const char* const* const argv) -> int32_t {
	assert_v(argc >= 3, "main", "Usage: " + string(argv[0]) + " <input.mkv> <output.mkv>");
	if (!expect_v(fs::exists(argv[1]), "main", "File '" + string(argv[1]) + "' does not exist") ||
		!expect_v(fs::is_regular_file(argv[1]), "main", "File '" + string(argv[1]) + "' is not a regular file"))
		panic_v("main", "Invalid file. Specify a valid video file to decode");
	assert_v(!fs::exists(argv[2]), "main", "Path '" + string(argv[2]) + "' is already occupied");

	//! \note Prepare video files for interactions with FFmpeg
	const auto videoInput = FFmpeg(argv[1]);
	const auto videoOutput = FFmpeg(argv[2]);
	//! \note Decode video from file using ffmpeg library
	const Frames* framesInput = videoInput.decode();
	//! \note for debug purposes
	clog << "Number of frames: " << framesInput->frames.size() << '\t' << framesInput->frames.capacity() << endl;

	//! \note Flip video horizontally using NvDEC
	// ...
	// delete framesInput; - temporary forbidden;

	//! \note Encode video using NvENC
	// ...
	const auto framesOutput = framesInput; // temporary solution

	//! \note Save video to file using ffmpeg library
	videoOutput.encode(framesOutput);
	delete framesOutput;

	return 0;
}
