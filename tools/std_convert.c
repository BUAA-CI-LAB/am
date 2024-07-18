#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
    if (argc != 4) {
        printf("usage : ./std_convert <begin addr(hex)> <input bin> <output hex>\n");
        printf("example : ./std_convert 0x1c000000 main.bin main.mif\n");
        exit(1);
    }

	FILE *in;
	FILE *out;

	unsigned char mem[32];
    unsigned int begin_addr = 0;

    sscanf(argv[1], "%x", &begin_addr);

    out = fopen(argv[3],"w");

    in  = fopen(argv[2],"rb");
    fprintf(out,"@%x\n", begin_addr);
    while(!feof(in)) {
        if (fread(mem,1,1,in) != 1) {
            fprintf(out,"%02x\n", mem[0]);
            break;
        }
        fprintf(out,"%02x\n", mem[0]);
    }

    fclose(in);
    fclose(out);
    return 0;
}
