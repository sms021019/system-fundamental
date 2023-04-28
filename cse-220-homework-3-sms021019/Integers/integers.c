#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void ones_to_twos(unsigned int num) {
   unsigned int ans;
 if ((num & 0x80) == 0x80) {
   ans = num + 1;
   printf("%08x\n", ans);
 } else {
   printf("%08x\n", num);
 }
}


void twos_to_ones(unsigned int num){
   unsigned int ans;
   if(num == 0x80000000){
       printf("undefined\n");


   }
   else if(num & 0x80){
       ans = num -1;
       printf("%08x\n", ans);
   } else {
       printf("%08x\n", num);
   }
}
void ones_complement_to_sign_magnitude(unsigned int num) {
   int sign_bit = 1 << 31;
   int sign = num & sign_bit;
   int magnitude = num & ~sign_bit;


   if (sign) {
       printf("%08x\n", ~magnitude);
   } else {
       printf("%08x\n", ~magnitude);
   }
}
void sign_magnitude_to_ones_complement(unsigned int num){
   int sign_bit = 1 << 31;
   int sign = num & sign_bit;
   int magnitude = num & ~sign_bit;


   if(sign){
       magnitude = ~magnitude;
       magnitude &= 0x7FFFFFFF;
       printf("%08x\n", magnitude);
   }else{
       printf("%08x\n", num);
   }
  
}
void twos_to_sign_mag(unsigned int num){
   int sign_bit = 1 << 31;
   int sign = num & sign_bit;
   int magnitude = num & ~sign_bit;


   if(sign){
       magnitude = ~magnitude + 1;
       magnitude |= 0x80000000;
       printf("%08x\n", magnitude);
   }else {
       printf("%08x\n", num);
   }
}


void sign_mag_to_twos(unsigned int num){
   int sign_bit = 1 << 31;
   int sign = num & sign_bit;
   int magnitude = num & ~sign_bit;


   if(sign){
       magnitude = ~(magnitude - 1);
       printf("%08x\n", magnitude);
   }else {
       printf("%08x\n", num);
   }
  
}






void BCD_to_bin(unsigned int num, char target_repr){
 
   unsigned int bin = 0;
   int isNegative = 3 << 30;
   int sign = num & isNegative;
   int magnitude = num & ~isNegative;


  
   for(int i = 1; magnitude != 0x00; magnitude = magnitude >> 5, i *= 10){
           bin += i * (magnitude & 0x0000001F);
   }


   if(sign == isNegative){
       if(target_repr == '2'){
           printf("%08x\n", ~bin + 1);
       }else if(target_repr == '1'){
           printf("%08x\n", ~bin);
       }else if(target_repr == 'S'){
           printf("%08x\n", 0x80000000 & bin);
       }
      
   } else if (sign == 0x00){
       printf("%08x\n", bin);
   } else{
       printf("error\n");
   }
  
}




void int_to_bcd(int num, char source_repr) {
//   printf("num: %d\n", num);
//   printf("repr: %c\n", source_repr);
   int sign_bit = 1 << 31;
   int sign = num & sign_bit;
  
   if(source_repr == '2' && sign == sign_bit){
       num = ~(num - 1);
   }else if (source_repr == '1' && sign == sign_bit){
       num = ~num;
   }else if (source_repr == 'S' && sign == sign_bit){
       num = num | ~sign_bit;
   }

   int init = 100000;
   int ans = 0;
   int temp = 0;
   for (int i = 0; i < 6; i++) {

       temp = num / init;
       num = num - temp * init;
       ans = ans << 5;

       ans = ans | temp;
       init /= 10;
       
   }
   int a =0xffffffff;
   if(ans == a){
       printf("undefined\n");
   }
   else if(sign == sign_bit){
       printf("%08x\n", 0xc0000000 | ans);
   }
   
   else {     
       printf("%08x\n", ans);

   }
}


void repr_convert(char source_repr, char target_repr, unsigned int repr){
   if(source_repr == target_repr){
       printf("%08x\n", repr);
   }
   else if(source_repr == '1' && target_repr == '2'){
       ones_to_twos(repr);
   }
   else if(source_repr == '2' && target_repr == '1'){
       twos_to_ones(repr);
   }
   else if(source_repr == '1' && target_repr == 'S'){
       ones_complement_to_sign_magnitude(repr);
   }
   else if(source_repr == 'S' && target_repr == '1'){
       sign_magnitude_to_ones_complement(repr);
   }
   else if(source_repr == '2' && target_repr == 'S'){
       twos_to_sign_mag(repr);
   }
   else if(source_repr == 'S' && target_repr == '2'){
       sign_mag_to_twos(repr);
   }
   else if(source_repr == 'D' && target_repr == '1'){
       BCD_to_bin(repr, target_repr);
   }
   else if(source_repr == 'D' && target_repr == '2'){
       BCD_to_bin(repr, target_repr);
   }
   else if(source_repr == 'D' && target_repr == 'S'){
       BCD_to_bin(repr, target_repr);
   }
   else if(source_repr == '1' && target_repr == 'D'){
       int_to_bcd(repr, source_repr);
   }
   else if(source_repr == '2' && target_repr == 'D'){
       int_to_bcd(repr, source_repr);
   }
   else if(source_repr == 'S' && target_repr == 'D'){
       int_to_bcd(repr, source_repr);
   }
   else{
       printf("error\n");
   }
  
}











// FILL IN THE BODY OF THIS FUNCTION.


// Feel free to create any other functions you like; just include them in this file.


//void repr_convert(char source_repr, char target_repr, unsigned int repr) {


//}







// DO NOT CHANGE ANY CODE BELOW THIS LINE


int main(int argc, char *argv[]) {


   (void)argc; // Suppress compiler warning


   repr_convert(argv[1][0], argv[2][0], (int)strtol(argv[3], NULL, 16));

   return 0;


}


