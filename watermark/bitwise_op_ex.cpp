//Bitwise operations

#include<stdio.h>

int main(){

char pixels[8] = {9,15,13,28, 99, 5, 0, 75}; //RGBA

char secret_number = 15;  //0000 1111 in binary as 1 char is 8 bits
int bitToExtract = 7; //8 bits but it goes from 0 to 7

for(int i =0; i<4; i++ ){

//Extract the bit from the secret_number

int bit = secret_number >> bitToExtract; //shifting 3 bits to the right to get the 3rd bit
bit &= 1; //this gives 0000 0001 and we get the bit
//Also check if secret secret_number
pixels[i] = pixels[i] & (0xFE); //If number is 1001 we & with 1110 = 1000
pixels[i] = (bit ? 1 : 0); //if it is 0 put 0 if it is one
//Clean up

if(--bitToExtract == -1){ //decrease so move bit to extrac
bitToExtract = 7;
//Next secret secret_number
//get next secret byte here

}


}

for(int i =0; i < 8; i++){
printf("%d ", pixels[i]);

}



}
