#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "wolfpack.h"





void print_packet_sf(const unsigned char *packet) {
   //printf("-----------printing fc start-----------\n");


   char extracted[100];
   for (int i = 0; i < 100; i++) {
       extracted[i] = packet[i];
   }




   char length_bytes[3];
   for (int i = 0; i < 3; i++) {
       //printf("%02x", extracted[17+i]);
       length_bytes[i] = (extracted[17+i]);
   }


  
   // Convert the bytes to an integer value
   int length = ((int)length_bytes[0] << 16) | ((int)length_bytes[1] << 8) | ((int)length_bytes[2]);


   //printf("Length: %d\n", length);


       // Print the extracted array
   for (int i = 0; i < 24; i++) {
       printf("%02x", extracted[i] & 0xFF);
       if((i+1)==5)printf("\n");
       if((i+1)==10)printf("\n");
       if((i+1)==11)printf("\n");
       if((i+1)==12)printf("\n");
       if((i+1)==15)printf("\n");
       if((i+1)==17)printf("\n");
       if((i+1)==20)printf("\n");
       if((i+1)==24)printf("\n");
   }
   for(int i = 0; i < (length - 24); i++){
       printf("%c", packet[24+i]);
   }
   printf("\n");




}


void convert_ulong_to_hex(unsigned long value, unsigned char *dest) {
   int i;


   // Convert the unsigned long to a hexadecimal number
   for (i = 4; i >= 0; i--) {
       dest[i] = value & 0xFF;
       value >>= 8;
   }
}


void convert_short_to_hex(unsigned short value, unsigned char *dest) {
   int i;


   // Convert the unsigned long to a hexadecimal number
   for (i = 4; i >= 0; i--) {
       dest[i] = value & 0xFF;
       value >>= 8;
   }
}





unsigned int packetize_sf(const char *message,
   unsigned char *packets[], unsigned int packets_len,
   unsigned int max_payload, unsigned long src_addr,
   unsigned long dest_addr, unsigned short flags) {


 // Compute the total length of the message.
 unsigned int msg_len = strlen(message);
//  printf("length of the message: %d\n",msg_len);


 // Compute the number of packets needed to store the message.
 unsigned int num_packets = (msg_len + max_payload - 1) / max_payload;
//  printf("number of packets needed to store: %d\n",num_packets);


 // Check if we have enough space in the packets array.
 int key = 0;
 if (num_packets > packets_len) {
//    printf("key turned on");
   num_packets = packets_len;
   key = 1;
 }


 // Initialize the variables for computing the fragment offset and checksum.
 unsigned long frag_offset = 0;
 unsigned long checksum = 0;






 // Iterate over the message and create the packets.
 for (unsigned int i = 0; i < num_packets; i++) {
//    printf("\n\n----start of %dth packet generating----\n\n",i);
   // Allocate memory for the new packet.
   unsigned int payload_len = (i < num_packets - 1) ? max_payload : (msg_len % max_payload);
   if(key){
       if((i >= num_packets - 1))
        //    printf("\n check point\n");
           payload_len = max_payload;
   }
//    printf("%dth packet's payload length: %d\n", i, payload_len);
   unsigned int packet_len = 24 + payload_len;
//    printf("%dth packet length: %d\n", i, packet_len);
   packets[i] = malloc(packet_len);
   unsigned char result[packet_len];
  
   // if (packet_data == NULL) {
   //   // Allocation failed.
   //   return i;
   // }






   // Copy the payload from the message to the packet.
   unsigned char payload [payload_len];
   memcpy(payload, message + i * max_payload, payload_len);
//    printf("%dth packet's payload: %s\n", i,payload);




   //converting unsigned long source and destination into char arrays
   unsigned char src_ad[5];
   unsigned char dest_ad[5];
   //printf("dest_addr: %lx\n",dest_addr);
   unsigned long temp = dest_addr;
   convert_ulong_to_hex(src_addr,src_ad);
   convert_ulong_to_hex(temp,dest_ad);
   //convert_short_to_hex(dest_addr,dest_ad);


   // unsigned long temp = 67899;
   // for (int t = 4; t >= 0; t--) {
   //     dest_ad[i] = temp & 0xff;
   //     temp >>= 8;
   // }


//    printf("src_ad: ");
//    for (int t = 0; t < 5; t++) {
//        printf("%02x", src_ad[t]);
//    }
//    printf("\ndest_ad: ");
//    for (int t = 0; t < 5; t++) {
//        printf("%02x", dest_ad[t]);
//    }
//    printf("\n");


  
   // char array for fragment offset
   unsigned long frag_copy = frag_offset;
   unsigned char frag[3];
   for(int t = 2; t >=0; t--){
       frag[t] = frag_copy & 0xff;
       frag_copy >>=8;
   }






   // char array for flags
   unsigned long flagtemp = flags;
   unsigned char flag[2];
   for(int t = 1; t >= 0; t--){
       flag[t] = flagtemp & 0xff;
       flagtemp >>=8;
   }


   //char array for total length
   unsigned long lengthtemp = packet_len;
   unsigned char total_length[3];
   for(int t = 2; t >= 0; t--){
       total_length[t] = lengthtemp & 0xff;
       lengthtemp >>= 8;
   }


   //char array for checksum
  
   checksum = src_addr + dest_addr + 32 + 64 + frag_offset + flags + packet_len;
   unsigned char sum[4];
   for(int t = 3; t >= 0; t--){
       sum[t] = checksum & 0xff;
       checksum >>=8;
   }


   unsigned char source_port[1];
   source_port[0] = '\x20';
   unsigned char destionation_port[1];
   destionation_port[0] = '\x40';


//    printf("source_port: %02x\n", source_port[0]);
//    printf("destionation_port: %02x\n", destionation_port[0]);
//    printf("frag: ");
//    for (int t = 0; t < 3; t++) {
//        printf("%02x", frag[t]);
//    }
//    printf("\nflag: ");
//    for (int t = 0; t < 2; t++) {
//        printf("%02x", flag[t]);
//    }
//    printf("\ntotal_length: ");
//    for (int t = 0; t < 3; t++) {
//        printf("%02x", total_length[t]);
//    }
//    printf("\nchecksum: ");
//    for (int t = 0; t < 4; t++) {
//        printf("%02x", sum[t]);
//    }
//    printf("\n");


   size_t pos = 0;
   memcpy(result + pos, src_ad, 5);
   pos += 5;
  
   memcpy(result + pos, dest_ad, 5);
   pos += 5;
   memcpy(result + pos, source_port, 1);
   pos += 1;
   memcpy(result + pos, destionation_port, 1);
   pos += 1;
   memcpy(result + pos, frag, 3);
   pos += 3;
   memcpy(result + pos, flag, 2);
   pos += 2;
   memcpy(result + pos, total_length, 3);
   pos += 3;
   memcpy(result + pos, sum, 4);
   pos += 4;
   memcpy(result + pos, payload, payload_len);
   pos += payload_len;


    unsigned long checksum_temp = checksum_sf(result);
    for(int t=23; t >= 20; t--){
        result[t] = checksum_temp & 0xff;
        checksum_temp >>= 8;
    }



   printf("%dth packet: ",i);
   for(unsigned int t = 0; t < packet_len; t++){
       printf("%02x", result[t]);
       packets[i][t] = result[t];
   }
   printf("\n");
  
   frag_offset += payload_len;
  


 }


 return num_packets;
}





void reverseArray(char* arr, int length) {
   int i, j;
   char temp;


   // Reverse the array using bitwise operators
   for (i = 0, j = length - 1; i < j; i++, j--) {
       temp = arr[i];
       arr[i] = arr[j];
       arr[j] = temp;
   }
}




unsigned int checksum_sf(const unsigned char *packet) {


   //printf("-----------checksum fc start-----------\n");
   // char extracted[100];
   // for (int i = 0; i < 100; i++) {
   //     extracted[i] = packet[i];
   // }
  
   unsigned char src_addr[5];
   for (int i = 0; i < 5; i++) {
       src_addr[i] = (packet[i]);
   }
  
   //reverseArray(src_addr,5);
   for(int i = 0; i < 5; i++){
       //printf("%x,",src_addr[i]);
   }
   //printf("\n");


   //printf("%llu\n",big_to_little_endian_char(src_addr, 5));
//   0  1  2  3  4
//   00,00,00,30,39


//   int a1 = 0x0000003039 -> 3930000000


   long a1 = ((long)src_addr[0] << 32) | ((long)src_addr[1] << 24) | ((long)src_addr[2] << 16) | ((long)src_addr[3] << 8) | ((long)src_addr[4]);
   //printf("%lx\n", a1);
   // int a1 =  ((int)src_addr[2] << 16) | ((int)src_addr[3] << 8) | ((int)src_addr[4]);


   unsigned char dest_addr[5];
   for (int i = 0; i < 5; i++) {
       dest_addr[i] = (packet[5+i]);
   }
   //reverseArray(dest_addr,5);
   long a2 = ((long)dest_addr[0] << 32) | ((long)dest_addr[1] << 24) | ((int)dest_addr[2] << 16) | ((int)dest_addr[3] << 8) | ((int)dest_addr[4]);
   // int a2 = ((int)dest_addr[2] << 16) | ((int)dest_addr[3] << 8) | ((int)dest_addr[4]);
  
   unsigned char src_pt[1];
   for (int i = 0; i < 1; i++) {
       src_pt[i] = (packet[10+i]);
   }
   long a3 = ((long)src_pt[0]);


   unsigned char dest_pt[1];
   for (int i = 0; i < 1; i++) {
       dest_pt[i] = (packet[11+i]);
   }
   long a4 = ((long)dest_pt[0]);


   unsigned char frag_off[3];
   for (int i = 0; i < 3; i++) {
       frag_off[i] = (packet[12+i]);
   }
   //reverseArray(frag_off,3);
   long a5 = ((long)frag_off[0] << 16) | ((long)frag_off[1] << 8) | ((long)frag_off[2]);




   unsigned char flags[2];
   for (int i = 0; i < 2; i++) {
       //printf("%02x", extracted[15+i]);       
       flags[i] = (packet[15+i]);
   }
   //reverseArray(flags,2);
   long a6 = ((long)flags[0] << 8) | ((long)flags[1]);


  
   unsigned char length[3];
   for (int i = 0; i < 3; i++) {
       length[i] = (packet[17+i]);
   }
   //reverseArray(length,3);
   long a7 = ((long)length[0] << 16) | ((long)length[1] << 8) | ((long)length[2]);






   // printf("a1: %lx\n", a1);
   // printf("a2: %lx\n", a2);
   // printf("a3: %lx\n", a3);
   // printf("a4: %lx\n", a4);
   // printf("a5: %lx\n", a5);
   // printf("a6: %lx\n", a6);
   // printf("a7: %lx\n", a7);


   long atotal = a1 + a2 + a3 + a4 + a5 + a6 + a7;
  
   //printf("before modulo: %lx\n", atotal);


   atotal = (unsigned int)(atotal % ((long)pow(2, 32)-1));
  




   return atotal;
}


unsigned int reconstruct_sf(unsigned char *packets[], unsigned int packets_len, char *message, unsigned int message_len) {
   int frags[packets_len];
   //unsigned char *payloads[packets_len];
   int bools[packets_len];
   int count = 0;
   unsigned int rightmost = message_len;
  


   for(unsigned int t = 0; t < packets_len; t++){
      
       unsigned char extracted[100];
       for(int j = 0; j < 100; j++){
           extracted[j] = packets[t][j];
       }


       //print_packet_sf(packets[t]);


       unsigned char frag_off[3];
       for (int i = 0; i < 3; i++) {
           frag_off[i] = (extracted[12+i]);
       }
       int a5 = ((int)frag_off[0] << 16) | ((int)frag_off[1] << 8) | ((int)frag_off[2]);


    //    unsigned char length[3];
    //    for (int i = 0; i < 3; i++) {
    //        length[i] = (extracted[17+i]);
    //    }
       //int a7 = ((int)length[0] << 16) | ((int)length[1] << 8) | ((int)length[2]);


       unsigned char checksum[4];
       //printf("\ncheck sum in packet: ");
       for (int i = 0; i < 4; i++) {
           checksum[i] = (extracted[20+i]);
           //printf("%x", extracted[20+i]);
       }
       //printf("\n");
       unsigned int sum = ((int)checksum[0] << 24) | ((int)checksum[1] << 16) | ((int)checksum[2] << 8) | ((int)checksum[3]);
      
       //printf("\nchecksum in function: %x\n",checksum_sf(packets[t]));
      
       // unsigned char payload[a7-24];
       // for(int i = 0; i < (a7-24); i++){
       //     payload[i] = extracted[24+i];
       //     //printf("%c", payload[i]);
       // }
      
       bools[t] = checksum_sf(packets[t]) == sum;
       frags[t] = a5;
       //payloads[t] = payload;
       //printf("stored payload value: %s\n", payloads[t]);
   }
    //printf("after forloop payload at index0: %c\n", *payloads[6]);


   //exception 4
   int ex = 0;
   for(unsigned int t = 0; t< packets_len; t++){
       //printf("%d\n", bools[t]);
       ex += bools[t];
   }
   if(ex == 0){
       printf("All packets are invalid");
       return 0;
   }
   rightmost = 0;
   for(unsigned int t = 0; t < packets_len; t++){
       if(bools[t]){
           unsigned char extracted[100];
           for(int j = 0; j < 100; j++){
               extracted[j] = packets[t][j];
           }


           // printf("\n\nreconstrucnting %dth packet into message\n\n",t);
           // printf("fragment offset: %d\n", frags[t]);
           // printf("payload: ");


           unsigned char length[3];
           for (int i = 0; i < 3; i++) {
               length[i] = (extracted[17+i]);
           }
           int a7 = ((int)length[0] << 16) | ((int)length[1] << 8) | ((int)length[2]);


           unsigned char payload[a7-24];
           for(int i = 0; i < (a7-24); i++){
               payload[i] = extracted[24+i];
               //printf("%c", payload[i]);
           }


           // int payload_len = strlen(payload);
           // printf("\nlength of payload: %d\n", payload_len);
          
           for(int i = 0; i < (a7-24); i++){
               if((unsigned int)(frags[t] + i) >= message_len){
                  break;
               }
               message[frags[t] + i] = payload[i];
               if((unsigned int)(frags[t] + i) > rightmost){
                   if(message_len > (unsigned int)(frags[t] + i)){
                       rightmost = frags[t] + i;
                   }
               }


               //printf("\nin message: %c", message[frags[t] + i]);
           }
           if((unsigned int)frags[t] < message_len) count++;


          
       }
      


   }


  
   if(rightmost == (message_len-1)){
       message[rightmost] = '\0';
   }else{
       message[rightmost+1] = '\0';
   }
   // for(int i = packets_len; i >= 0; i--){
   //     if(bools[i]){
   //         if(frags[i]+)
   //     }
   // }
   // for(int i = message_len-1; i >= 0; i--){
   //     if(message[i] != '@'){
   //         if(i == message_len-1){
   //             message[i] = '\0';
   //             break;
   //         }
   //         else{
   //             message[i+1] = '\0';
   //             break; 
   //         }
   //     }
   // }
  
   printf("\nmessage: %s\n",message);
   return count;
}







