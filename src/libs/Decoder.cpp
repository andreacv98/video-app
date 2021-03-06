#include "Decoder.h"
#include <stdexcept>

int Decoder::sendPacket(const AVPacket *packet) {
    int ret;
    if((ret = avcodec_send_packet(inCodecContext, packet)) < 0){
        throw std::runtime_error("Decoder: failed to send frame to decoder");
    }
    return ret;
}

int Decoder::getDecodedOutput(AVFrame* rawFrame) {
    int ret = avcodec_receive_frame(this->inCodecContext, rawFrame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        return ret;
    else if (ret < 0) {
        throw std::runtime_error("Decoder: failed to receive frame from decoder");
    }
    return ret;
}

const AVCodecContext *Decoder::getCodecContext() const {
    return inCodecContext;
}

void Decoder::setCodecContext(AVCodecContext *context){
    this->inCodecContext = context;
}

