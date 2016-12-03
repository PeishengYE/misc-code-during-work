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
    }
    printf("\n");
}


void swapIndividualBitsWithXOR( unsigned int input, unsigned int position_i,
        unsigned int position_j ){

unsigned int i, j; // positions of bit sequences to swap
unsigned int n;    // number of consecutive bits in each sequence
unsigned int ret;    

unsigned int x = ((input >> position_i) ^ (input >> position_j)) & ((1U << n) - 1); // XOR temporary
ret = input ^ ((x << i) | (x << j));
return ret;
    
}


int main(){
    unsigned int input = 0x55U;
    unsigned int output;

    print_out_bits(input);

    output = swapBits(input);

    print_out_bits(output);


}



