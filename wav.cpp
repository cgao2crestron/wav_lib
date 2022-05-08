#include "wav.h"
#include<stdio.h>

void write_header(struct HEADER myHeader, FILE* fpMixerOut)
{
	fseek(fpMixerOut, 0, SEEK_SET);

	fwrite((unsigned char *)&myHeader.riff, 1, 4, fpMixerOut);
	fwrite(&myHeader.overall_size, sizeof(int), 1, fpMixerOut);  // offset 4, data_size + fmt_size + 20 or data_size + 36
	
	//fmt chunk
	fwrite((unsigned char*)&myHeader.wave, 1, 4, fpMixerOut);
	fwrite((unsigned char*)&myHeader.fmt, 1, 4, fpMixerOut);
	fwrite(&myHeader.length_of_fmt, sizeof(int), 1, fpMixerOut);   // 16
	fwrite((unsigned short*)&myHeader.format_type, sizeof(short), 1, fpMixerOut);
	fwrite((unsigned short*)&myHeader.channels, sizeof(short), 1, fpMixerOut);

	fwrite(&myHeader.sample_rate, sizeof(int), 1, fpMixerOut);
	fwrite(&myHeader.byterate, sizeof(int), 1, fpMixerOut);

	fwrite((unsigned short*)&myHeader.block_align, sizeof(short), 1, fpMixerOut);
	fwrite((unsigned short*)&myHeader.bits_per_sample, sizeof(short), 1, fpMixerOut);

	// data chunk
	fwrite((unsigned char*)&myHeader.data, 1, 4, fpMixerOut);
	fwrite(&myHeader.data_size, sizeof(int), 1, fpMixerOut);  // offset 40
}

void write_data_short(short* pShort, int data_length, FILE* fpMixerOut)
{

	fwrite(pShort, sizeof(short), data_length, fpMixerOut);
}

void close_wav_file(struct HEADER myHeader, FILE* fpMixerOut)
{
	// update sample counts
	fseek(fpMixerOut, 4, SEEK_SET);
	fwrite(&myHeader.overall_size, sizeof(int), 1, fpMixerOut);  // offset 4, data_size + fmt_size + 20 or data_size + 36

	fseek(fpMixerOut, 40, SEEK_SET);
	fwrite(&myHeader.data_size, sizeof(int), 1, fpMixerOut);  // offset 40
};

void* parse_header(char* chunk_name, char* data_label,  FILE* fpMixerOut)
{
	if (chunk_name[0] == 'f' && chunk_name[1] == 'm' && chunk_name[2] == 't')
	{
		// add a function to find out the chunk location first
		switch (data_label[0]) {
			case 'a': // audio format
				fseek(fpMixerOut, 20, SEEK_SET);
				break;
			case 's': // sample rate
				fseek(fpMixerOut, 24, SEEK_SET);
					break;
			default:
				return NULL;
		}	
	}
}
 

