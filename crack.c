#include <string.h>
#include <stdlib.h>
#include <stdio.h>
unsigned char out;
int sim_resol(unsigned char *iv, unsigned char *key, int byte_index)
{
	unsigned char s[256];
	unsigned char a, s_1, k;
	unsigned char tmp_key[8];
	int i, j;
	memcpy(tmp_key, iv, 3);
	memcpy(&tmp_key[3], key, 5);
	for (i=0; i<256; i++)
		s[i] = i;
	j=k=0;
	for(i=0; i<byte_index+3; i++)
	{
		j = (unsigned char)(j+s[i]+tmp_key[k]);
		a = s[i];
		s[i] = s[j];
		s[j] = a;
		if (++k >= 8)
			k = 0;
	}
	for (i=0; i<256; i++)
		if(s[i] == out)
			s_1 = i;
	if((s[0] != byte_index+ 3) || (s[1] != 0))
		return -1;
	else
		return (s_1-j-s[byte_index+3]);
}
int main(int argc,char* argv[]) 
{
	FILE *fd;
	unsigned char tmp_line[256];
	unsigned char plain = 'C';
	size_t read;
	unsigned char key[40];
	unsigned char counts[256];
	int tmp, max, max_index, i;
	int byte_index;
	for(byte_index=0; byte_index<5; byte_index++)
	{
		fd = fopen(argv[1],"r");
		if(fd == NULL)
		{
			printf ("can not open file\n");
			return 0;
		}
		memset(counts, 0, 256);
		read = fread (tmp_line, sizeof(unsigned char), 143, fd);
		while((read !=0 ) && (tmp_line[25] == 255))
		{
	 		if((tmp_line[24] == byte_index+3))
			{
				out = plain^tmp_line[27];
				tmp = sim_resol(&tmp_line[24], key, byte_index);
				if((tmp >= 0) && (tmp <=255))
					counts[tmp]++;
			}
			memset(tmp_line, 0, 256);
			read = fread(tmp_line, sizeof(unsigned char), 143, fd);
		}
		max=0;
		max_index=0;
		for(i=0; i<256; i++)
		{
			if(counts[i] > max)
			{
				max_index=i;
				max = counts[i];
			}
		}
		key[byte_index]=(unsigned char)max_index;
	}
	printf("%c%c%c%c%c", key[0], key[1], key[2], key[3], key[4]);
	fclose (fd);
	return 0;
}
