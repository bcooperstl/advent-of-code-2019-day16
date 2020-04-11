#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024
#define BASE_PATTERN_LEN 4
#define COPIES 1
#define MAX_MEGA_SIZE (MAX_SIZE*COPIES)

int output_for_element(int * current, int length, int * base_pattern, int base_pattern_length, int element_number)
{
    int sum=0;
    //printf("Element Number %d\n", element_number);
    for (int i=0; i<length; i++)
    {
        int offset=((i+1)/element_number)%base_pattern_length;
        //printf(" offset for i=%d is %d\n", i, offset);
        int pattern_value=base_pattern[offset];
        //printf(" pattern value for i=%d is %d\n", i, pattern_value);
        sum+=(current[i]*pattern_value);
    }
    return abs(sum%10);
}

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
    int base_pattern[BASE_PATTERN_LEN]={0,1,0,-1};
    int * pattern_for_element=malloc(sizeof(int) * MAX_MEGA_SIZE);
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
    for (int i=0; i<input_len; i++)
    {
        for (int j=0; j<COPIES; j++)
        {
            current[j*input_len+i]=buffer[i]-'0';
            next[j*input_len+i]=0;
        }
    }
    
    //output_offset=(current[0]*1000000)+
    //              (current[1]* 100000)+
    //              (current[2]*  10000)+
    //              (current[3]*   1000)+
    //              (current[4]*    100)+
    //              (current[5]*     10)+
    //              (current[6]*      1);
    
    for (int i=0; i<num_phases; i++)
    {
        printf("Phase %d:\n", i);
        for (int j=0; j<input_len*COPIES; j++)
        {
            if (j%100==0)
                printf("%d \n", j);

            printf("Element Number %d\n", j+1);
            for (int k=0; k<input_len*COPIES; k++)
            {
                int offset=((k+1)/(j+1))%BASE_PATTERN_LEN;
                printf(" offset for k=%d is %d\n", k, offset);
                int pattern_value=base_pattern[offset];
                printf(" pattern value for k=%d is %d\n", k, pattern_value);
                next[j]+=(current[k]*pattern_value);
            }
            for (int k=0; k<input_len*COPIES; k++)
            {
                next[k]=abs(next[k]);
                printf("value of position %d is %d\n", k, next[k]);
            }
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
    free(pattern_for_element);
    
    return 0;
}
