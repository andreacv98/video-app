//
// Created by Andrea on 22/02/2022.
//

#include <iostream>
#include "AudioDemuxer.h"

/**
 * The method opens the device and defines the needed data structure.
 *
 * @return the device context
 * @throw runtime_error
 */
AVFormatContext *AudioDemuxer::open() {
    //if one of them != nullptr then input already initialized
    if(inFormatContext != nullptr || inCodecContext!= nullptr || streamIndex != -1)
        return inFormatContext;

    value = avformat_open_input(&inFormatContext, url, inFormat, &options);
    if (value != 0) {
        throw std::runtime_error("Cannot open selected device");
        exit(1);
    }

    value = avformat_find_stream_info(inFormatContext, nullptr);
    if (value < 0) {
        throw std::runtime_error("Cannot find the stream information");
    }

    //find the first video stream with a given code
    streamIndex = -1;
    for (int i = 0; i < inFormatContext->nb_streams; i++){
        if (inFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            streamIndex = i;
            break;
        }
    }

    if (streamIndex == -1) {
        throw std::runtime_error("Cannot find the audio stream index. (-1)");
    }

    AVCodecParameters *params = inFormatContext->streams[streamIndex]->codecpar;
    inCodec = avcodec_find_decoder(params->codec_id);
    if (inCodec == nullptr) {
        throw std::runtime_error("Cannot find the audio decoder");
    }
    std::cout << "Input audio codec:" << inCodec->name;

    inCodecContext = avcodec_alloc_context3(inCodec);

    if(avcodec_parameters_to_context(inCodecContext, params)<0)
        throw std::runtime_error("Cannot create codec context for audio input");


    value = avcodec_open2(inCodecContext, inCodec, nullptr);
    if (value < 0) {
        throw std::runtime_error("Cannot open the input audio codec");
    }

    return inFormatContext;
}

AudioDemuxer::AudioDemuxer(char *url, char *src) : Demuxer(url, src) {

}