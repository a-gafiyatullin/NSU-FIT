#ifndef LABORATORY_WORK_4_WAVE_H
#define LABORATORY_WORK_4_WAVE_H

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

// x86_64 ARCHITECTURE ONLY!!!
#ifdef __x86_64__

#define RIFF 0x46464952
#define WAVE 0x45564157
#define DATA 0x61746164
#define KB 1024

#endif

struct wave_header {
	uint8_t id[4]; // RIFF
	uint32_t size; // file size - 8 in bytes
	uint8_t file_type[4]; // WAVE

	uint8_t format_chunk_marker[4]; // fmt
	uint32_t format_data_length;
	uint16_t audio_format; // 1 if PCM
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byteRate; // (Sample Rate * Bits Per Sample * Channels) / 8
	uint16_t block_align; // num_channels * Bits Per Sample / 8
	uint16_t bits_per_sample;
};

struct wave_chunk_header {
	uint8_t chunk_marker[4];
	uint32_t chunk_length; // size of the data section
};

int hide_src_in_dst(char *src_file_name, char *dst_file_name);
int get_src_from_dst(char *dst_file_name);

#endif //LABORATORY_WORK_4_WAVE_H
