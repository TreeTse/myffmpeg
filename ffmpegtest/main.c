#include <stdio.h>
#include <libavcodec/avcodec.h>

int main() {
	printf("test ffmpeg\n");
	printf("%s\n", avcodec_configuration());
	return 0;
}