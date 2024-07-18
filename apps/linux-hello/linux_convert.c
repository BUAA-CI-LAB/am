#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *in;
	FILE *out;

	unsigned char mem[32];

    FILE *in_data;
    out = fopen("build/linux.vlog","w");

    in  = fopen("build/vmlinux.bin","rb");
    fprintf(out,"@300000\n");
    while(!feof(in)) {
        if (fread(mem,1,1,in) != 1) {
            fprintf(out,"%02x\n", mem[0]);
            break;
        }
        fprintf(out,"%02x\n", mem[0]);
    }

    FILE *in_init_5f;
    in_init_5f = fopen("init_5f.txt", "r");
    fprintf(out, "@5f00000\n");
    while(!feof(in_init_5f)) {
        if (fread(mem,3,1,in_init_5f) != 1) {
            break;
        }
        fprintf(out,"%c%c%c", mem[0], mem[1], mem[2]);
    }


    in_data = fopen("build/start.bin", "rb");
    fprintf(out, "@1c000000\n");
    while(!feof(in_data)) {
        if (fread(mem,1,1,in_data) != 1) {
            fprintf(out,"%02x\n", mem[0]);
            break;
        }
        fprintf(out,"%02x\n", mem[0]);
    } 
    
    fclose(in_init_5f); 
    fclose(in_data);
    fclose(in);
    fclose(out);
    return 0;
}
