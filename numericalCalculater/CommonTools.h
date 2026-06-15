#pragma once
class CommonTools
{
public:
	CommonTools();
	~CommonTools();
};





/**
* 本程序实现了视频文件解码为YUV数据。
*
*/
#include <stdio.h>
#include <string>
#include <iostream>
using  namespace  std;

#ifdef    __cplusplus
extern "C"
{
#endif 
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#ifdef   __cplusplus
};
#endif 



int open_codec_context(int *stream_idx,
	AVFormatContext *fmt_ctx, AVCodecContext **codec_context, enum AVMediaType type);


void initPakcet(AVPacket *packet);
void inityuvframe(AVFrame **yuvFrame, AVCodecContext* codec_context);

void makeyuvfile(FILE *fp_yuv, AVFrame *yuvFrame);
//void decodeVideo(AVFormatContext *format_context, AVCodecContext* codec_context);
void decodeVideo(AVFormatContext *format_context, AVCodecContext* codec_context, string  nameFileOut);

void main_ttt(string  nameFile, string nameFileOut);

