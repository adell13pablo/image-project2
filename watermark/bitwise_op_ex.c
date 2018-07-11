//Bitwise operations

#include<stdio.h>

int main(){

char pixels[8] = {9,15,13,28, 99, 5, 0, 75}; //RGBA

char secret_number = 15;  //0000 1111 in binary as 1 char is 8 bits
int bitToEmbed = 7; //8 bits but it goes from 0 to 7

for(int i =0; i<8; i++ ){

//Extract the bit from the secret_number

int bit = secret_number >> bitToEmbed; //shifting 7 bits to the right to get the 3rd bit
bit &= 1; //this gives 0000 0001 and we get the bit
//Also check if secret secret_number
pixels[i] = pixels[i] & (0xFE); //If number is 1001 we & with 1110 = 1000
pixels[i] |= (bit ? 1 : 0); //if it is 0 put 0 if it is one
//Clean up

if(--bitToEmbed == -1){ //decrease so move bit to extrac
bitToEmbed = 7;
//Next secret secret_number
//get next secret byte here

}


}

for(int i =0; i < 8; i++){
printf("%d ", pixels[i]);

}

//Get the number back
int bitToExtract = 7;
int secret = 0;
for(int i =0; i<8; i++){
int bit = pixels[i] & 1; //Either 1 or 0
secret |= bit << bitToExtract;

if(--bitToExtract == -1){
bitToExtract = 7;
}
printf("%d ", secret);
}
 //Revisar esto


}
