#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024
#define BASE_PATTERN_LEN 4
#define COPIES 10000
//#define COPIES 10
#define MAX_MEGA_SIZE (MAX_SIZE*COPIES)

void copy_next_to_current(int * current, int * next, int length)
{
    for (int i=0; i<length; i++)
    {
        current[i]=next[i];
        next[i]=0;
    }
}

int main (int argc, char * argv[])
{
    FILE * infile=NULL;
    char buffer[MAX_SIZE+1];
    
    int num_phases;
    int input_len;
    int * current=malloc(sizeof(int) * MAX_MEGA_SIZE);
    int * next=malloc(sizeof(int) * MAX_MEGA_SIZE);
    int output_offset=0;
    
    memset(&buffer, '\0', sizeof(buffer));
    
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s filename phases\n", argv[0]);
        exit(1);
    }
    
    num_phases=atoi(argv[2]);
    
    fprintf(stderr, "Opening file %s\n", argv[1]);
    
    infile=fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        exit(1);
    }
    
    fgets(buffer, sizeof(buffer), infile);
    fclose(infile);
    
    char * eol = strchr(buffer, '\r');
    if (eol)
    {
        *eol='\0';
    }
    eol = strchr(buffer, '\n');
    if (eol)
    {
        *eol='\0';
    }
    
    input_len=strlen(buffer);
    printf("input_len is %d\n", input_len);
    for (int i=0; i<input_len; i++)
    {
        for (int j=0; j<COPIES; j++)
        {
            current[j*input_len+i]=buffer[i]-'0';
            next[j*input_len+i]=0;
        }
    }
    
    output_offset=(current[0]*1000000)+
                  (current[1]* 100000)+
                  (current[2]*  10000)+
                  (current[3]*   1000)+
                  (current[4]*    100)+
                  (current[5]*     10)+
                  (current[6]*      1);
    
    for (int i=0; i<num_phases; i++)
    {
        printf("Phase %d:\n", i);

        // only computing right halfs
        for (int j=input_len*COPIES; j>=output_offset; j--)
        {
            if (j%1000==0)
                printf("%d of %d\n", j, input_len*COPIES);
            next[j]+=current[j];
            next[j]=abs(next[j]%10);
            next[j-1]=next[j];
        }
        printf("\n");
        copy_next_to_current(current, next, input_len*COPIES);
    }
    
    printf("*****Final output (starting at offset %d: ", output_offset);
    for (int i=0; i<8; i++)
        printf("%d", current[output_offset+i]);
    printf("\n");
    
    free(current);
    free(next);
    
    return 0;
}
