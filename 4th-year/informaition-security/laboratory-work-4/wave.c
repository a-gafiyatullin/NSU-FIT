#include "wave.h"

#define DEBUG

#ifdef DEBUG

void debug_wave_header(const struct wave_header *header)
{
	printf("ID: ");
	for (int i = 0; i < 4; i++) {
		printf("%c", header->id[i]);
	}

	printf("\nFile size: %u", header->size);

	printf("\nFile type header: ");
	for (int i = 0; i < 4; i++) {
		printf("%c", header->file_type[i]);
	}

	printf("\nFormat chunk marker: ");
	for (int i = 0; i < 4; i++) {
		printf("%c", header->format_chunk_marker[i]);
	}

	printf("\nLength of format data: %u", header->format_data_length);

	printf("\nType of format: %hu", header->audio_format);

	printf("\nNumber of channels: %hu", header->num_channels);

	printf("\nSample rate: %u", header->sample_rate);

	printf("\nByte rate: %u", header->byteRate);

	printf("\nBlock align: %hu", header->block_align);

	printf("\nBits per sample: %hu", header->bits_per_sample);
}

void debug_wave_chunk_header(const struct wave_chunk_header *header)
{
	printf("\nData chunk marker: ");
	for (int i = 0; i < 4; i++) {
		printf("%c", header->chunk_marker[i]);
	}

	printf("\nSize of the data section: %u", header->chunk_length);
}

#endif

int safe_read(int fd, const uint8_t *buffer, const size_t num)
{
	ssize_t size = 0;
	ssize_t curr_size = 0;

	while (1) {
		curr_size = read(fd, (void *)(buffer) + curr_size, num);
		if (curr_size < 0) {
			return -1;
		}
		size += curr_size;
		if (size == num) {
			break;
		}
	}

	return 0;
}

int safe_write(int fd, const uint8_t *buffer, const size_t num)
{
	ssize_t size = 0;
	ssize_t curr_size = 0;

	while (1) {
		curr_size = write(fd, (void *)buffer + curr_size, num);
		if (curr_size < 0) {
			return -1;
		}
		size += curr_size;
		if (size == num) {
			break;
		}
	}

	return 0;
}

int read_wave_header(int file, struct wave_header *header)
{
	static size_t wave_header_size = sizeof(struct wave_header);

	if (safe_read(file, (const uint8_t *)header, wave_header_size) < 0) {
		return -1;
	}

	if (*((uint32_t *)(header->id)) != RIFF) {
		errno = EAGAIN;
		return -1;
	}

	if (*((uint32_t *)(header->file_type)) != WAVE) {
		errno = EAGAIN;
		return -1;
	}

	if (header->audio_format != 1) {
		errno = EAGAIN;
		return -1;
	}

	return 0;
}

int read_wave_chunk_header(int file, struct wave_chunk_header *header)
{
	static size_t wave_chunk_header_size = sizeof(struct wave_chunk_header);

	if (safe_read(file, (const uint8_t *)header, wave_chunk_header_size) <
	    0) {
		return -1;
	}

	return 0;
}

int is_data_wave_chunk_header(const struct wave_chunk_header *header)
{
	return (*((uint32_t *)header->chunk_marker) == DATA);
}

void assign_bytes_to_dst_data(uint8_t *dst_data, const uint8_t *bytes,
			      const size_t data_len, const size_t num_bits)
{
	const size_t dst_bytes_per_src_byte = CHAR_BIT / num_bits;

	for (size_t i = 0; i < data_len; i++) {
		uint8_t mask = (1 << num_bits) - 1;
		uint8_t data_mask = mask;
		mask = ~mask;

		for (int j = 0; j < dst_bytes_per_src_byte; j++) {
			dst_data[(i * dst_bytes_per_src_byte) + j] =
				(dst_data[(i * dst_bytes_per_src_byte) + j] &
				 mask) |
				((bytes[i] & data_mask)) >> j * num_bits;
			data_mask <<= num_bits;
		}
	}
}

void gather_bits_to_bytes(const uint8_t *dst_data, uint8_t *bytes,
			  const size_t data_len, const size_t num_bits)
{
	const size_t dst_bytes_per_src_byte = CHAR_BIT / num_bits;

	for (size_t i = 0; i < data_len; i++) {
		uint8_t mask = 1;

		for (int j = 0; j < dst_bytes_per_src_byte; j++) {
			bytes[i] = (bytes[i] & (~mask)) |
				   (dst_data[i * dst_bytes_per_src_byte + j] &
				    1) << j;
			mask <<= 1;
		}
	}
}

int hide_src_in_dst(char *src_file_name, char *dst_file_name)
{
	static size_t wave_chunk_header_size = sizeof(struct wave_chunk_header);

	int src_file;
	int dst_file = -1;
	int out_file = -1;

	struct wave_header *wave_hdr = NULL;
	struct wave_chunk_header *wave_chunk_hdr = NULL;
	uint8_t *dst_buffer = NULL;
	uint8_t *src_buffer = NULL;

	src_file = open(src_file_name, O_RDONLY);
	if (src_file < 0) {
		goto error;
	}

	// get file length in bytes
	size_t src_file_size = lseek(src_file, 0, SEEK_END);
	lseek(src_file, 0, SEEK_SET);

	dst_file = open(dst_file_name, O_RDONLY);
	if (dst_file < 0) {
		goto error;
	}

	out_file =
		open(strcat(dst_file_name, ".out"), O_CREAT | O_WRONLY, 0644);
	if (out_file < 0) {
		goto error;
	}

	// allocate memory for WAVE headers
	wave_hdr = malloc(sizeof(struct wave_header));
	if (!wave_hdr) {
		goto error;
	}
	wave_chunk_hdr = malloc(sizeof(struct wave_chunk_header));
	if (!wave_chunk_hdr) {
		goto error;
	}

	// get WAVE header and check it, then write to output
	if (read_wave_header(dst_file, wave_hdr) < 0) {
		goto error;
	}
	if (safe_write(out_file, (const uint8_t *)wave_hdr,
		       sizeof(struct wave_header)) < 0) {
		goto error;
	}
#ifdef DEBUG
	debug_wave_header(wave_hdr);
#endif
	size_t dst_buffer_size_curr = sizeof(struct wave_header);

	// search for data section
	while (dst_buffer_size_curr < wave_hdr->size) {
		if (read_wave_chunk_header(dst_file, wave_chunk_hdr) < 0) {
			goto error;
		}
		if (safe_write(out_file, (const uint8_t *)wave_chunk_hdr,
			       wave_chunk_header_size) < 0) {
			goto error;
		}

#ifdef DEBUG
		debug_wave_chunk_header(wave_chunk_hdr);
#endif
		dst_buffer_size_curr += wave_chunk_hdr->chunk_length;
		if (!is_data_wave_chunk_header(wave_chunk_hdr)) {
			dst_buffer = malloc(wave_chunk_hdr->chunk_length);
			if (!dst_buffer) {
				goto error;
			}
			if (safe_read(dst_file, dst_buffer,
				      wave_chunk_hdr->chunk_length) < 0) {
				goto error;
			}
			if (safe_write(out_file, dst_buffer,
				       wave_chunk_hdr->chunk_length) < 0) {
				goto error;
			}
			free(dst_buffer);
			dst_buffer = NULL;
		} else {
			break;
		}
	}

	// calculate parameters
	size_t dst_buffer_size = wave_hdr->block_align * KB;
	dst_buffer = malloc(dst_buffer_size);
	if (!dst_buffer) {
		goto error;
	}

	// choose two degree
	size_t src_bits_per_dest_byte =
		ceil((double)(CHAR_BIT * src_file_size) /
		     wave_chunk_hdr->chunk_length);
	if (src_bits_per_dest_byte > 1) {
		if (src_bits_per_dest_byte == 3) {
			src_bits_per_dest_byte = 4;
		} else if (src_bits_per_dest_byte > 4 &&
			   src_bits_per_dest_byte < 8) {
			src_bits_per_dest_byte = 8;
		}
	}

	// hide source file length in the destination file
	if (safe_read(dst_file, dst_buffer, sizeof(size_t) * CHAR_BIT) < 0) {
		goto error;
	}
	assign_bytes_to_dst_data(dst_buffer, (const uint8_t *)&src_file_size,
				 sizeof(size_t), src_bits_per_dest_byte);
	if (safe_write(out_file, dst_buffer, sizeof(size_t) * CHAR_BIT) < 0) {
		goto error;
	}

	// hide src content in the dst
	src_buffer = malloc(4 * KB);
	ssize_t src_buffer_size = read(src_file, src_buffer, 4 * KB);
	if (src_buffer_size < 0) {
		goto error;
	}
	size_t src_buffer_size_curr = 0;

	dst_buffer_size_curr = sizeof(size_t) * CHAR_BIT;
	while (dst_buffer_size_curr < wave_chunk_hdr->chunk_length) {
		ssize_t size = read(dst_file, dst_buffer, dst_buffer_size);
		if (size < 0) {
			goto error;
		}
		dst_buffer_size_curr += size;

		for (size_t i = 0; i < size;
		     i += CHAR_BIT / src_bits_per_dest_byte) {
			if (src_buffer_size <= 0) {
				continue;
			}
			assign_bytes_to_dst_data(dst_buffer + i,
						 src_buffer +
							 src_buffer_size_curr,
						 1, src_bits_per_dest_byte);
			src_buffer_size_curr++;
			if (src_buffer_size_curr == src_buffer_size) {
				src_buffer_size =
					read(src_file, src_buffer, 4 * KB);
			}
		}
		if (safe_write(out_file, dst_buffer, size) < 0) {
			goto error;
		}
	}

	// copy tail of the src
	while (1) {
		ssize_t size = read(dst_file, dst_buffer, dst_buffer_size);
		if (size <= 0) {
			break;
		}
		if (safe_write(out_file, dst_buffer, size) < 0) {
			goto error;
		}
	}

error:
	free(wave_hdr);
	free(wave_chunk_hdr);
	free(dst_buffer);
	free(src_buffer);
	close(src_file);
	close(out_file);
	close(dst_file);

	return errno;
}

int get_src_from_dst(char *dst_file_name)
{
	static size_t wave_chunk_header_size = sizeof(struct wave_chunk_header);

	int src_file = -1;
	int dst_file;

	struct wave_header *wave_hdr = NULL;
	struct wave_chunk_header *wave_chunk_hdr = NULL;
	uint8_t *dst_buffer = NULL;
	uint8_t *src_buffer = NULL;

	dst_file = open(dst_file_name, O_RDONLY);
	if (dst_file < 0) {
		goto error;
	}

	src_file =
		open(strcat(dst_file_name, ".out"), O_CREAT | O_WRONLY, 0644);
	if (src_file < 0) {
		goto error;
	}

	// allocate memory for WAVE headers
	wave_hdr = malloc(sizeof(struct wave_header));
	if (!wave_hdr) {
		goto error;
	}
	wave_chunk_hdr = malloc(sizeof(struct wave_chunk_header));
	if (!wave_chunk_hdr) {
		goto error;
	}

	// get WAVE header and check it
	if (read_wave_header(dst_file, wave_hdr) < 0) {
		goto error;
	}
#ifdef DEBUG
	debug_wave_header(wave_hdr);
#endif
	size_t dst_buffer_size_curr = sizeof(struct wave_header);

	// search for data section
	while (dst_buffer_size_curr < wave_hdr->size) {
		if (read_wave_chunk_header(dst_file, wave_chunk_hdr) < 0) {
			goto error;
		}
#ifdef DEBUG
		debug_wave_chunk_header(wave_chunk_hdr);
#endif
		dst_buffer_size_curr += wave_chunk_hdr->chunk_length;
		if (!is_data_wave_chunk_header(wave_chunk_hdr)) {
			if (lseek(dst_file, wave_chunk_hdr->chunk_length,
				  SEEK_CUR) < 0) {
				goto error;
			}
		} else {
			break;
		}
	}

	// calculate parameters
	size_t dst_buffer_size = wave_hdr->block_align * KB;
	dst_buffer = malloc(dst_buffer_size);
	if (!dst_buffer) {
		goto error;
	}

	// get file length
	if (safe_read(dst_file, dst_buffer, sizeof(size_t) * CHAR_BIT) < 0) {
		goto error;
	}
	size_t src_file_size = 0;
	gather_bits_to_bytes(dst_buffer, (uint8_t *)&src_file_size,
			     sizeof(size_t), 1);

	// choose two degree
	size_t src_bits_per_dest_byte =
		ceil((double)(CHAR_BIT * src_file_size) /
		     wave_chunk_hdr->chunk_length);
	if (src_bits_per_dest_byte > 1) {
		if (src_bits_per_dest_byte == 3) {
			src_bits_per_dest_byte = 4;
		} else if (src_bits_per_dest_byte > 4 &&
			   src_bits_per_dest_byte < 8) {
			src_bits_per_dest_byte = 8;
		}
	}

	// get source file from the destination file
	src_buffer = malloc(4 * KB);
	size_t current_src_length = 0;

	dst_buffer_size_curr = sizeof(size_t) * CHAR_BIT;
	while (dst_buffer_size_curr < wave_chunk_hdr->chunk_length) {
		ssize_t size = read(dst_file, dst_buffer, dst_buffer_size);
		if (size < 0) {
			goto error;
		}
		dst_buffer_size_curr += size;

		for (size_t i = 0; i < size;
		     i += CHAR_BIT / src_bits_per_dest_byte) {
			char symbol = 0;
			gather_bits_to_bytes(dst_buffer + i, &symbol, 1,
					     src_bits_per_dest_byte);
			current_src_length++;
			if (current_src_length == src_file_size) {
				break;
			}
			if (safe_write(src_file, &symbol, 1) < 0) {
				goto error;
			}
		}
		if (current_src_length == src_file_size) {
			break;
		}
	}

error:
	free(wave_hdr);
	free(wave_chunk_hdr);
	free(dst_buffer);
	free(src_buffer);
	close(src_file);
	close(dst_file);

	return errno;
}
