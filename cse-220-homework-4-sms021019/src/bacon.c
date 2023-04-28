#include "bacon.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Add other #includes here if you want.
int encrypt(const char *plaintext, char *ciphertext) {


    int lenPlain = strlen(plaintext);
    int lenCipher = strlen(ciphertext);
    int lenCount= 0;
    for(int t = 0; t < lenCipher-1; t++){
        if(ciphertext[t] != ' '){
            lenCount++;
            // printf("lenght: %d\n",lenCount);
        }
    }

    if(lenCount < 6){
        // printf("returned -1");
        return -1;
    }

    int temp = lenCount / 6;
    
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ !\\\"#$%&'(),-./0123456789:;?";
    printf("%s\n",alphabet);

    char inputText[lenPlain + 1];
    strcpy(inputText, plaintext);

    int i, j, num, count = 0;
    
    char binstrfin[100] = "";
    char binstr[100] = "";

    // Convert inputText to uppercase
    for (i = 0; inputText[i] != '\0'; i++) {
        inputText[i] = toupper(inputText[i]);
    }
    printf("inputtext: %s\n", inputText);
    


    for (i = 0; i < temp-1; i++) {
       
        int indexValue = indexOf(alphabet, inputText[i]);

        printf("indexOf input text: %d\n", indexValue);
        if (indexValue != -1) {
            num = indexOf(alphabet, inputText[i]);
            j = 6;
            
            binstr[0] = '\0';
            while (j != 0) {
                
                if ((num % 2) == 0) {
                    strcat(binstr, "0");
                    num /= 2;
                } else {
                    strcat(binstr, "1");
                    num /= 2;
                }
               
                j--;
            }
            count++;
            reverse(binstr);
            strcat(binstrfin,binstr);
        } else {
            printf("returned -1\n");
            return -1;
        }
    }

    for(i = 0; i < 6; i++){
        strcat(binstrfin, "1");
    }
    

    int a = 0;
    int b = 0;
    char modified[100] = "";
    strcpy(modified, ciphertext);
    while (a < strlen(binstrfin)) { 
        if (ciphertext[b] != ' ') {
            if(binstrfin[a] == '1'){
                ciphertext[b] = toupper(ciphertext[b]);
                a++;
                b++;
            }else{
                ciphertext[b] = tolower(ciphertext[b]);
                a++;
                b++;
            }         
        }else{
            b++;
        }   
    }
    return count;
}




// decrypt START--------------------------------------------


int decrypt(const char *ciphertext, char *plaintext) {
    

    char inputText[strlen(ciphertext) + 1];
    strcpy(inputText, ciphertext);
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ !\\\"#$%&'(),-./0123456789:;?";
    char upperAlph[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char lowerAlph[] = "abcdefghijklmnopqrstuvwxyz";
    if(strlen(plaintext) == 0){
        return -1;
    }
    if(errorCheck2(ciphertext) == -2){
        return -2;
    }

    int i = 0;

    char binstr[100] = "";
    int EOMcount = 0;
    int count = 0;
    for(i = 0; i < strlen(ciphertext); i++){
        if(ciphertext[i] != ' '){
            if(indexOf(upperAlph,inputText[i]) != -1){
                strcat(binstr,"1");
                EOMcount++;
                count++;
                if(EOMcount == 6){
                    break;
                }
            }else if(indexOf(lowerAlph,inputText[i]) != -1){
                count++;
                strcat(binstr,"0");
            }
        }
        if(count == 6){
            count = 0;
            EOMcount = 0;
        }
    }
    count = 0;
    int num1 = strlen(binstr) / 6;
    for(i = 0; i < num1; i++){
        if(i == (num1 - 1)){
            plaintext[i] = '\\';
            plaintext[i+1] = '0';
            break;
        }
        int tempNum = firstSixBits(binstr);
        if(tempNum > 52 && tempNum != 63){
            plaintext = "*******************";
            return -3;
        }
        plaintext[i] = alphabet[tempNum];
        removeFirstSixBits(binstr);
    }
    return (num1 -1);
}

//decrypt END-----------------------------------------------

// helping functions ---------------------------
int errorCheck2(char* str) {
    int i, count = 0;
    int len = strlen(str);
    for (i = 0; i < len; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            count++;
            if (count == 6) {
                return i-5; // return the index of the first uppercase character in the sequence
            }
        } else {
            count = 0;
        }
    }
    return -2; // return -2 if no 6 consecutive uppercase characters were found
}

int firstSixBits(char* binary) {
    int i;
    int num = 0;
    for (i = 0; i < 6; i++) {
        num = (num << 1) + (binary[i] - '0'); // shift the current value of num left by one and add the current bit
    }
    return num;
}

void removeFirstSixBits(char* binary) {
    int len = strlen(binary);
    if (len <= 6) {
        binary[0] = '\0'; // if the input string is too short, set it to an empty string
    } else {
        memmove(binary, binary+6, len-6+1); // move the remaining bits to the beginning of the string
    }
}


int indexOf(char *str, char *c) {
    int i;

    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == c) {
            return i;
        }
    }

    return -1;
}
void reverse(char str[]) {
    int i, j;
    char temp;

    for (i = 0, j = strlen(str) - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}


// helping functions END---------------------------







