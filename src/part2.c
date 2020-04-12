#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024
#define BASE_PATTERN_LEN 4
//#define COPIES 10000
#define COPIES 10
#define MAX_MEGA_SIZE (MAX_SIZE*COPIES)

int gcd(int a, int b)
{
    int temp;
    while (b != 0)
    {
        temp = a % b;

        a = b;
        b = temp;
    }
    return a;
}

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

void calculate_sum(int ** range_sums, int single_length, int from, int to)
{
    int sum=0;
    int from_copy_number=from/single_length;
    int to_copy_number=to/single_length;
    int from_offset=from%single_length;
    int to_offset=to%single_length;
    int full_copies=to_copy_number-from_copy_number;

    if (full_copies==0)
    {
        return range_sums[from_offset][to_offset];
    }
    
    if (from_offset > 0)
        sum+=range_sums[from_offset][single_length-1]; // add the portion to get to the next whole run. only needs to be done if the request doesn't start on a whole range

    sum+=(range_sums[0][single_length-1]*full_copies); // add all the copies of the next whole run
    
    if (to_offset < (single_length-1)
        sum+=range_sums[0][to_offset]; // add the portion for after the whole runs. only needs to be done if the request doesn't end at a whole run
    return sum%10;
}

void calculate_cycle_sum(int ** range_sums, int single_length, int element_number, int * base_pattern, int base_pattern_length)
{
    int cycle_length=(element_number*base_pattern_length)/gcd(element_number,base_pattern_length);
    
}

int calculate_element_sum()
{
    // different cases:
    //    full cycles + remainder (see last 4 cases)
    //       occurs when element number * 4 < big_length
    //    zeroes, then positives, then zeros, then negatives, then one last zero;
    //       element number * 4 = big length
    //    zeroes, positives, zeros, some negatives
    //       element number 
    //    zeroes, all positives, zero or more zeroes
    //    zereos, some positives
    // case 1 - 
}

void precalculate_sums(int * current, int ** range_sums, int length)
{
    for (int i=0; i<length; i++)
    {
        for (int j=0; j<length; j++)
        {
            range_sums[i][j]=0;
        }
    }
    for (int i=0; i<length; i++)
    {
        // iterator over current
        for (int j=0; j<length; j++)
        {
            for (int k=0; k<length; k++)
            {
                if (i>=j && i<=k && j<=k)
                {
                    range_sums[j][k]+=current[i];
                }
            }
        }
    }

    for (int i=0; i<length; i++)
    {
        for (int j=0; j<length; j++)
        {
            range_sums[i][j]=range_sums[i][j]%10; // only care about the last digit.
            printf("The sum from terms %d to %d is %d\n", i+1, j+1, range_sums[i][j]);
        }
    }
}

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
    int ** range_sums=malloc(sizeof(int *)*MAX_SIZE);
    for (int i=0; i<MAX_SIZE; i++)
        range_sums[i]=malloc(sizeof(int)*MAX_SIZE);
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
    
    //TODO: comment this out
    output_offset=0;
    
    for (int i=0; i<num_phases; i++)
    {
        printf("Phase %d:\n", i);
        precalculate_sums(current, range_sums, input_len);

        for (int j=0; j<input_len*COPIES; j++)
        {
            if (j%1000==0)
                printf("%d \n", j);
            // this needs to go to the lcm of j+1 and input_len
            
            int lcm=((j+1)*BASE_PATTERN_LEN)/gcd(j+1,BASE_PATTERN_LEN);
            //printf("Element Number %d\n", j+1);
            //printf(" %d goes from %d to %d\n", j, 0, lcm);
            for (int k=j; k<lcm; k++)
            {
                //int offset=((k+1)/(j+1))%BASE_PATTERN_LEN;
                //printf(" offset for k=%d is %d\n", k, offset);
                //int pattern_value=base_pattern[offset];
                //printf(" pattern value for k=%d is %d\n", k, pattern_value);
                //next[j]+=(current[k]*pattern_value);
                next[j]+=(current[k]*base_pattern[((k+1)/(j+1))%BASE_PATTERN_LEN]);
            }
            for (int k=0; k<input_len*COPIES; k++)
            {
                next[k]=abs(next[k]%10);
                //printf("value of position %d is %d\n", k, next[k]);
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
    for (int i=0; i<MAX_SIZE; i++)
        free(range_sums[i]);
    free(range_sums);

    
    return 0;
}
