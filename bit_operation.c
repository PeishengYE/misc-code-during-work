#include <stdio.h>
#include <stdlib.h>


/*
 * good bits operation in the following link
http://graphics.stanford.edu/~seander/bithacks.html#SwappingBitsXOR
*/

void print_out_bits(unsigned int input);
unsigned int swapBits(unsigned int input);


unsigned int swapBits(unsigned int input){
    int tmp_odd, tmp_event, ret;
    unsigned int odd_mask = 0x55555555U;
    unsigned int event_mask = 0x55555555U << 1;
    /* left shift one bit*/
    tmp_odd = input&odd_mask;
    tmp_event = input&event_mask;

   /* mask the odd bit */
    ret = (tmp_odd <<1)  | (tmp_event>>1);
    return ret;

}

void print_out_bits(unsigned int input){
    unsigned int i = 0;
    unsigned int m = 1<<31;
    for(i=0; i<32; i++){
        if((m>>i)&input){
            printf("1");
        }else{
            printf("0");
        }
		if(i%4 == 3){
           printf(" ");
		}
    }
    printf("\n");
}

/* be careful of not overlap */

unsigned int  swapIndividualBitsWithXOR( unsigned int input, unsigned int position_i,
        unsigned int position_j, int number_bits ){

	unsigned int ret,tmp;    
	tmp = ((input >> position_i) ^ (input >> position_j)) & ((1U << number_bits) - 1); // XOR temporary
	ret = input ^ ((tmp << position_i) | (tmp << position_j));
	return ret;
		
}

void swap(unsigned int *i, unsigned int *j)
{
	*i ^= *j;
	*j ^= *i;
	*i ^= *j;

}

void set_bits(unsigned int *input, int postion_start, int number_bits)
{
	unsigned int tmp;

	tmp = ((1U << number_bits)-1);
	*input |= (tmp << postion_start);
}

void clear_bits(unsigned int *input, int position_start, int number_bits)
{
	unsigned int tmp;
	tmp = ~(((1U << number_bits) -1) << position_start);
	*input &= tmp;

}

void set_bits_to_a_pattern(int *input, int position_start, int number_bits, unsigned int pattern)
{
	clear_bits(input, position_start, number_bits);
	*input |= (pattern << position_start);

}
/* determining an integer is a power of 2 */

unsigned int is_power_of_two(){
	unsigned int f;
   f = (v & (v-1)) == 0;
   return f;
}

int main(){
    unsigned int input = 0x5576afb4U;
    unsigned int input1 = 0x5576afb4U;
    unsigned int input2 = 0x55989834U;
    unsigned int output;

	printf("---------------original------------------------------\n");
    print_out_bits(input);
	set_bits(&input, 4, 4);
	printf("---------------from postion 4, set 4 bits------------\n");
    print_out_bits(input);
	clear_bits(&input, 5, 10);
	printf("---------------from postion 5, clear 10 bits------------\n");
    print_out_bits(input);

	printf("---------------from postion 10, set 5 bits to pattern 0x5------------\n");
    set_bits_to_a_pattern(&input, 10, 5, 0x5 );
    print_out_bits(input);
    //output = swapBits(input);
    //print_out_bits(output);

	printf("\n---------------swap 4 bits between position 3 and position 10 ------------\n");
    output = swapIndividualBitsWithXOR( input, 3, 10, 4); 
    print_out_bits(output);

	printf("\n---------------swap int with XOR ------------\n");
	printf("---------------Original input 1 ------------\n");
    print_out_bits(input1);
	printf("---------------Original input 2 ------------\n");
    print_out_bits(input2);

	printf("---------------Result of swap int with XOR ------------\n");
	swap(&input1, &input2);
	printf("--------------- input 1 ------------\n");
    print_out_bits(input1);
	printf("--------------- input 2 ------------\n");
    print_out_bits(input2);


}



