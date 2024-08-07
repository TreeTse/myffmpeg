#include <stdio.h>
#include <signal.h>
#include "libavutil/log.h"
#include "libavformat/avformat.h"

static const char* input_filename;
static const char program_name[] = "mp4probe";

static void sigterm_handler(int sig)
{
	exit(123);
}

static void print_error(const char* filename, int err)
{
	char errbuf[128];
	const char* errbuf_ptr = errbuf;

	if (av_strerror(err, errbuf, sizeof(errbuf)) < 0)
		errbuf_ptr = strerror(AVUNERROR(err));
	av_log(NULL, AV_LOG_ERROR, "%s: %s\n", filename, errbuf_ptr);
}

int main(int argc, char** argv) {
	avformat_network_init();

	signal(SIGINT, sigterm_handler); /* Interrupt (ANSI).    */
	signal(SIGTERM, sigterm_handler); /* Termination (ANSI).  */

	if (argc != 3) {
		av_log(NULL, AV_LOG_ERROR, "error! usage: %s -i mp4_file_name\n", program_name);
		exit(1);
	}
	if (strcmp(argv[1], "-i")) {
		av_log(NULL, AV_LOG_ERROR, "error! usage: %s -i mp4_file_name\n", program_name);
		exit(1);
	}
	input_filename = argv[2];
	AVFormatContext* ic = NULL;
	int err = 0;

	ic = avformat_alloc_context();
	if (!ic) {
		av_log(NULL, AV_LOG_FATAL, "Could not allocate context.\n");
		goto fail;
	}
	av_log(NULL, AV_LOG_INFO, "start check.\n");
	err = avformat_open_input(&ic, input_filename, NULL, NULL);
	if (err < 0) {
		print_error(input_filename, err);
		goto fail;
	}
	err = avformat_find_stream_info(ic, NULL);
	if (err < 0) {
		if (err == AVERROR_EXIT)
			err = 1;
		goto fail;
	}
	av_log(NULL, AV_LOG_INFO, "finish. the mp4 file can be used.\n");
	return 0;

fail:
	if (ic)
		avformat_close_input(&ic);
	return err;
}