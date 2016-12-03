#include <stdio.h>
#include <stdlib.h>
unsigned int swapBits(unsigned int input){

	unsigned int tmp_odd_bits;
	unsigned int tmp_event_bits;
	unsigned int ret;

	tmp_odd_bits = 0x55555555U & input;
	tmp_event_bits = (0x55555555U << 1) & input;
	ret = (tmp_odd_bits << 1)|(tmp_event_bits >> 1);
	return ret;


}

void print_bits(unsigned int input){
	unsigned int start_bit = 1U<<31;
	int i;

	/*FIXME no ; after i++ */
	for(i=0; i< 32; i++){
		if((start_bit>>i)&input){
			printf("1");
		}else{
			printf("0");
		}
		/*FIXME after 3 print a space 
		 * not 0 to print a space
		 * */
		if(i%4 == 3 ){

			printf(" ");
		}
	}
	printf("\n");

}

int main(){
	unsigned int input = 0x248a123;
	print_bits(input);
	print_bits(swapBits(input));

}
