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

int calculate_sum_with_range_sums(int ** range_sums, int single_length, int from, int to)
{
    int sum=0;
    int from_copy_number=from/single_length;
    int to_copy_number=to/single_length;
    int from_offset=from%single_length;
    int to_offset=to%single_length;
    int full_copies=to_copy_number-from_copy_number-1;

    printf("    rs: single_length %d from %d (copy=%d offset=%d) to %d (copy=%d offset=%d)\n", single_length, from, from_copy_number, from_offset, to, to_copy_number, to_offset);

    if (from_copy_number==to_copy_number)
    {
        printf("    rs: from and to are in the same copy. returning sum direct from array %d\n", range_sums[from_offset][to_offset]);
        return range_sums[from_offset][to_offset];
    }
    
    printf("    rs: adding %d to get the end of the from copy (%d to %d)\n", range_sums[from_offset][single_length-1], from_offset, single_length-1);
    sum+=range_sums[from_offset][single_length-1]; // add the portion to get to the next whole run. only needs to be done if the request doesn't start on a whole range
    
    
    printf("    rs: adding %d to get the start of the to copy (%d to %d)\n", range_sums[0][to_offset], 0, to_offset);
    sum+=range_sums[0][to_offset]; // add the portion for after the whole runs. only needs to be done if the request doesn't end at a whole run
    
    if (full_copies > 0)
    {
        printf("    rs: adding %d full copies of value %d (from %d to %d)\n", full_copies, range_sums[0][single_length-1], 0, single_length-1); 
        sum+=(range_sums[0][single_length-1]*full_copies); // add all the copies of the next whole run
    }
    
    printf("    rs: returning final value of %d\n", sum%10);
    
    return sum%10;
}

// this will determine the number of cycles, get the sum of a cycle, then and then brute force the last bit
int calculate_sum_with_cycles(int * current, int input_length, int element_number, int * base_pattern, int base_pattern_length)
{
    printf("calculating sum with cycles for element %d\n", element_number);
    // a cycle_length should be the lcm of the input_length (repeated string) and the pattern repeat (element number * base_pattern_length)
    int cycle_length=(input_length*(element_number*BASE_PATTERN_LEN))/gcd(input_length, (element_number*BASE_PATTERN_LEN)); // this is the lcm of element_number and the base length (4)
    int num_cycles=(input_length*COPIES)/cycle_length;
    int overflow_start=cycle_length*num_cycles;
    int overflow_length=(input_length*COPIES)%cycle_length;
    printf("  the cycle_length is %d. There are %d cycles and overflow of %d elements starting at position %d\n", cycle_length, num_cycles, overflow_length, overflow_start);
    int cycle_sum=0;
    
    for (int i=0; i<cycle_length; i++)
    {
        cycle_sum+=(current[i]*base_pattern[((i+1)/(element_number))%BASE_PATTERN_LEN]);
    }
    printf("  the cyles sum is %d", cycle_sum);
    cycle_sum*=num_cycles;
    printf(" with %d cycles is %d", num_cycles, cycle_sum);
    cycle_sum=cycle_sum%10;
    printf(" which results in %d\n", cycle_sum);
    
    
    int overflow_sum=0;
    for (int i=overflow_start; i<(input_length*COPIES); i++)
    {
        overflow_sum+=(current[i]*base_pattern[((i+1)/(element_number))%BASE_PATTERN_LEN]);
    }
    printf("  the overflow sum is %d", overflow_sum);
    overflow_sum=overflow_sum%10;
    printf("  which results in %d\n", overflow_sum);
    int total_sum=abs((cycle_sum+overflow_sum)%10);
    printf("  the total sum is %d\n", total_sum);
    return total_sum;
}

int calculate_sum_no_cycles(int * current, int input_length, int element_number, int ** range_sums)
{
    printf("calculating sum no_cycles for element %d\n", element_number);
    int positive_start_position = element_number-1;
    int positive_end_position = positive_start_position+element_number-1;
    int negative_start_position = (3*element_number)-1;
    int negative_end_position = negative_start_position+element_number-1;
    
    int include_negatives=1;
    
    if (positive_end_position >= (input_length*COPIES))
    {
        positive_end_position = (input_length*COPIES)-1;
    }
    
    if (negative_start_position >= (input_length*COPIES))
    {
        include_negatives=0;
    }
    
    if (negative_end_position >= (input_length*COPIES))
    {
        negative_end_position = (input_length*COPIES)-1;
    }
    
    int total_sum=0;
    printf("  positive calculated from %d to %d\n", positive_start_position, positive_end_position);
    int positive_sum=calculate_sum_with_range_sums(range_sums, input_length, positive_start_position, positive_end_position);
    total_sum+=positive_sum;
    int negative_sum=0;
    
    if (include_negatives==1)
    {
        printf("  negative calculated from %d to %d\n", negative_start_position, negative_end_position);
        negative_sum=calculate_sum_with_range_sums(range_sums, input_length, negative_start_position, negative_end_position);
        total_sum-=negative_sum;
        printf("  total sum is %d from positive sum %d and negative sum %d\n", total_sum, positive_sum, negative_sum);
    }
    else
    {
        printf("  total sum is %d from positive sum %d and no negative sum\n", total_sum, positive_sum);
    }
    total_sum=abs(total_sum%10);
    printf("  returning %d for total sum\n", total_sum);
    return total_sum;
}

int calculate_element_sum(int * current, int input_length, int element_number, int * base_pattern, int base_pattern_length, int ** range_sums)
{
    if (element_number * base_pattern_length <= (input_length*COPIES))
        return calculate_sum_with_cycles(current, input_length, element_number, base_pattern, base_pattern_length);
    else
        return calculate_sum_no_cycles(current, input_length, element_number, range_sums);
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
            
            next[j]=calculate_element_sum(current, input_len, j+1, base_pattern, BASE_PATTERN_LEN, range_sums);
            //int lcm=((j+1)*BASE_PATTERN_LEN)/gcd(j+1,BASE_PATTERN_LEN);
            //printf("Element Number %d\n", j+1);
            //printf(" %d goes from %d to %d\n", j, 0, lcm);
            //for (int k=j; k<lcm; k++)
            //{
                //int offset=((k+1)/(j+1))%BASE_PATTERN_LEN;
                //printf(" offset for k=%d is %d\n", k, offset);
                //int pattern_value=base_pattern[offset];
                //printf(" pattern value for k=%d is %d\n", k, pattern_value);
                //next[j]+=(current[k]*pattern_value);
                //next[j]+=(current[k]*base_pattern[((k+1)/(j+1))%BASE_PATTERN_LEN]);
            //}
            //for (int k=0; k<input_len*COPIES; k++)
            //{
            //    next[k]=abs(next[k]%10);
                //printf("value of position %d is %d\n", k, next[k]);
            //}
        }
        printf("\n");
        copy_next_to_current(current, next, input_len*COPIES);
    }
    
    printf("*****Final output (starting at offset %d: ", output_offset);
    for (int i=0; i<input_len*COPIES; i++)
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
