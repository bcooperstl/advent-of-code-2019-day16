#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024
#define BASE_PATTERN_LEN 4

void build_pattern_for_element(int element_number, int * output, int output_length, int * base_pattern, int base_pattern_length)
{
    int base_index=0;
//    printf("Element number %d generates pattern: ", element_number);
    for (int i=0; i<output_length; i++)
    {
        if ((i+1)%element_number==0)
            base_index++;
        if (base_index==base_pattern_length)
            base_index=0;
        output[i]=base_pattern[base_index];
//        printf("%d ", output[i]);
    }
//    printf("\n");
}

int output_for_element(int * current, int * pattern, int length)
{
    int sum=0;
    for (int i=0; i<length; i++)
    {
        sum+=(current[i]*pattern[i]);
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
    int current[MAX_SIZE];
    int next[MAX_SIZE];
    int base_pattern[BASE_PATTERN_LEN]={0,1,0,-1};
    int pattern_for_element[MAX_SIZE];
    
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
        current[i]=buffer[i]-'0';
    }
    
    for (int i=0; i<num_phases; i++)
    {
//        printf("Phase %d:\n", num_phases+1);
        for (int j=0; j<input_len; j++)
        {
            build_pattern_for_element(j+1, pattern_for_element, input_len, base_pattern, BASE_PATTERN_LEN);
            next[j]=output_for_element(current, pattern_for_element, input_len);
//            printf("  element %d goes from %d to %d\n", j+1, current[j], next[j]);
        }
        copy_next_to_current(current, next, input_len);
    }
    
    printf("*****Final output: ");
    for (int i=0; i<8; i++)
        printf("%d", next[i]);
    printf("\n");
    
    return 0;
}
