extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/avutil.h>
	#include <libavutil/imgutils.h>
	#include <libswscale/swscale.h>
}

#include "../include/ffmpeg.hpp"

//! \note avformat_network_init(); - Optional and not recommended anymore
FFmpeg::FFmpeg(const string filename) : filename(filename) {}

//! \note avformat_network_deinit(); - Optional and not recommended anymore
FFmpeg::~FFmpeg(void) {}

const Frames* packet(AVFormatContext* formatContext, int videoStreamIndex, AVCodecContext* codec_ctx, AVFrame* frame) {
	Frames* result = new Frames();
	// av_init_packet(&packet); - deprecated
	AVPacket* packet = av_packet_alloc();
	assert_v(packet != nullptr, "packet", "Could not allocate packet for video stream in the input file");
	while (av_read_frame(formatContext, packet) >= 0) {
		if (packet->stream_index == videoStreamIndex) {
			int ret = avcodec_send_packet(codec_ctx, packet);
			assert_v(ret >= 0, "packet", "Error sending a packet for decoding in the input file");
			while (ret >= 0) {
				ret = avcodec_receive_frame(codec_ctx, frame);
				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
					break;
				} else if (ret < 0) {
					panic_v("packet", "Error during decoding in the input file");
				} else {
					// Send the frame to NVDEC
					result->push(frame);
				}
			}
		}
		av_packet_unref(packet);
	}
	av_packet_free(&packet);
	return result;
}

const Frames* FFmpeg::decode(void) const {
	// Open the input file
	AVFormatContext* formatContext = nullptr;
	assert_v(avformat_open_input(&formatContext, filename.c_str(), nullptr, nullptr) == 0, "FFmpeg", "Could not open input file");
	// Find the stream information
	assert_v(avformat_find_stream_info(formatContext, nullptr) >= 0, "FFmpeg", "Could not find stream information");
	// Find the video stream in the input file
	int videoStreamIndex = -1;
	for (unsigned int i = 0; i < formatContext->nb_streams; ++i)
		if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			assert_v(videoStreamIndex <= INT_MAX, "FFmpeg", "Too many video streams");
			videoStreamIndex = i;
			break;
		}
	assert_v(videoStreamIndex >= 0, "FFmpeg", "Could not find video stream");
	// Find the decoder for the video stream
	AVCodecParameters* codec_params = formatContext->streams[videoStreamIndex]->codecpar;
	const AVCodec* codec = avcodec_find_decoder(codec_params->codec_id);
	assert_v(codec != nullptr, "FFmpeg", "Could not find codec for video stream");
	// Create a new codec context for the video stream
	AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
	assert_v(codec_ctx != nullptr, "FFmpeg", "Could not allocate codec context for video stream");
	assert_v(avcodec_parameters_to_context(codec_ctx, codec_params) >= 0, "FFmpeg", "Could not copy codec parameters to codec context for video stream");
	assert_v(avcodec_open2(codec_ctx, codec, nullptr) == 0, "FFmpeg", "Could not open codec for video stream");
	// Create a new frame to hold the decoded video data
	AVFrame* frame = av_frame_alloc();
	assert_v(frame != nullptr, "FFmpeg", "Could not allocate frame for video stream");
	// Read frames from the input file, decode them using the codec context, and send them to NvDEC
	const Frames* result = packet(formatContext, videoStreamIndex, codec_ctx, frame);
	// Release the resources used by the codec context, frame, and input file
	avcodec_free_context(&codec_ctx);
	av_frame_free(&frame);
	avformat_close_input(&formatContext);
	return result;
}
void FFmpeg::encode(const Frames* Frames) const {
}
