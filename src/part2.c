#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024
#define BASE_PATTERN_LEN 4
#define COPIES 10000
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
            //build_pattern_for_element(j+1, pattern_for_element, input_len*COPIES, base_pattern, BASE_PATTERN_LEN);
            next[j]=output_for_element(current, input_len*COPIES, base_pattern, BASE_PATTERN_LEN, j+1);
//            printf("  element %d goes from %d to %d\n", j+1, current[j], next[j]);
        }
        printf("\n");
        copy_next_to_current(current, next, input_len*COPIES);
    }
    
    int offset=
    printf("*****Final output (starting at offset %d: ", output_offset);
    for (int i=0; i<8; i++)
        printf("%d", next[output_offset+i]);
    printf("\n");
    
    free(current);
    free(next);
    free(pattern_for_element);
    
    return 0;
}
