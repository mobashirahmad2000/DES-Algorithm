#include <iostream> 
#include <string>   
#include <cmath>    
						   
using namespace std;
  

string round_keys[16];         // intialize  array of size [16] because DES algo work in 16 rounds and this will store the round keys
string plain_t;                  // initialze string variable plain_t to store  plain text input for the DES algorithm

/* In below function named as CDTB (Conversion Decimal To Binary) with parameter decimal is converting decimal to binary, 
An empty string named binary is to store binary result, while loop is for purpose of binary conversion, which continues till decimal not equal to 0.
% modulus operator to get remainder as 0 or 1 because binary only contains num in 0 & 1 only and Ternary operator is giving result on the logic of 
while loop  and + is used for concatenation of binary, dec = dec/2  converts a decimal number to its binary representation and assigned back values to decimal
the last while loop is designed to ensures that the binary representation is at least 4 digits long by leading zeros, In each iteration,(0 + binary) a 0 
is added to the left side of binary */

string CDTB(int decimal) { 
    string binary;           
    while(decimal != 0) {      
        binary = (decimal % 2 == 0 ? "0" : "1") + binary; 
        decimal = decimal / 2; 
  }                              
while(binary.length() < 4) { 
        binary = "0" + binary;
    }
    return binary;
}

/* Now Below function CBTD (Conversion Binary To Decimal) with parameter binary in which decimal will store the  final decimal value, counter is for position
 of current bit proceeding. declared variable size = binary length for the forloop working. In for loop int i = size-1 because array start from 0. 
 i>=0 to ensures loop iterates through all positions in binary string from right to left. i-- help loop to moves towards the leftmost bit of the binary string.
 if (binary[i] == '1') this if condition checks if current bit is 1 (true) than corresponding power of 2 should be added to the decimal value because 0 power 
 anything=0. pow() power function is used to add 2 raised to power counter (which hold decimal value of current bit). counter++ for position of next bit.
*/

int CBTD(string binary) {
    int decimal = 0;
    int counter = 0;
    int size = binary.length();
 for(int i = size - 1; i >= 0; i--) {
    if(binary[i] == '1') {
            decimal += pow(2, counter);
        }
    counter++;
    }
    return decimal;
}

/*                        AFTER GOING THROUGH PERMUTAION CHOICE 1 (PC1) IT REDUCE THE 64 BIT KEY TO 56 BIT KEY 
                                        AND DIVIDE INTO TWO EQUAL HALVES OF 28 28 BIT KEYS 
                     By equaly dividing 56 bits into half 28 & 28, Below two functions is typically used to perform a circular left shifts

 1st below function perform a circular left shifts once on a portion of the key. for loop will run 28 times from 1 index to 27 & by ignoring 0 index and 
 store the value in string named shifted. shifted += key_bit[0]; this logic will store the ignored 1st value at the end index of string shifted. 
 This is how this function performs a left shift by one position.
 
 This function will only execute when specific certain rounds of the DES key schedule applied during the 1st, 2nd, 9th, and 16th 
rounds. (This is like rule of DES Algorithm specificaly on 1,2,9,16 round).
*/

string shift_left_once(string key_bit) {
    string shifted = "";
    for(int i = 1; i < 28; i++) {
    shifted += key_bit[i];
    }
    shifted += key_bit[0];
    return shifted;
}

/*  Same as upper left shift function (same logic & functionality except for loop of twice (i<2)) only change is that it will do left shift operation on the 
input key_bit twice.This function will execute in all rounds EXCEPT 1st, 2nd, 9th, and 16th rounds. */

string shift_left_twice(string key_bit) {
    string shifted = "";
for(int i = 0; i < 2; i++) {
    for(int j = 1; j < 28; j++) {
        shifted += key_bit[j];
        }
        shifted += key_bit[0];
        key_bit = shifted;
        shifted = "";
    }
    return key_bit;
}

/* To make our Message more secure we use xor gate to make it more complex, In below xor function initializing int size equal to the size of string b to run 
for loop  continuously as long as i<size. Now there is if statement that checks if value of string  a != value of string b than give result 1(true) else give
result 0(false) [as of xor opeartion  0_1 = 1, 1_0 = 1, 0_0 = 0, 1_1 = 0] ( 1 is true and 0 is false) */

string Xor(string a, string b) {
    string result = "";
    int size = b.size();
    for(int i = 0; i < size; i++) {
    if(a[i] != b[i]) {
         result += "1";
        } else {
           result += "0";
        }
    }
    return result;
}

/* below function is generating keys from original key. the permutaion choice 1 and 2 in which pc1 selects 56 bits from 64 by ignoring the 8 bits 
and later on split these 56 bits into two 28-bit halves, both half undergoes CIRCULAR left shifts during each round and in pc2 it is reduced to 48 bits by again
ignoring 8 bits from pc1, bits are selected based on the DES standard. The specific bit selection patterns in PC1 and PC2 are designed to enhance the security
of the  algorithm  by creating unique and unpredictable round keys. */

void generate_keys(string key) {
    int pc1[56] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
    };

    int pc2[48] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    };

    string perm_key = "";
    for(int i = 0; i < 56; i++) {  // simply initializing reduced pc1 keys to empty string perm_key
    perm_key += key[pc1[i] - 1];
    }

    string left = perm_key.substr(0, 28); // initializing both half 28 bits of pc1[56] to string named left and right.
    string right = perm_key.substr(28, 28); // substr tells select bits of pc1 from position 0 to leght 27 and onwards 28 to 28.

/*As i already explained in above comments of both functions of left shift one and twice, this is the logic below using logical operators in for loop of when 
to shift single or twice bits. calling both functions in if else statement and initializing result of both function to named variables right and left. */

    for(int i = 0; i < 16; i++) {
       if(i == 1 || i == 2 || i == 9 || i == 16) {
            left = shift_left_once(left);
            right = shift_left_once(right);
   } 
		else{
            left = shift_left_twice(left);
            right = shift_left_twice(right);
        }

    string combined_key = left + right; //combining both halves of pc1 using concatenation 
    string round_key = "";

    for(int i = 0; i < 48; i++) {
    round_key += combined_key[pc2[i] - 1]; // inside loop round key will store result of permutation applying on combined key using pc2 table 
                  // to select & rearrange specific bits and store 48 transposed bits. (pc2[i] gives the position of the bit to be taken from combined_key) 
}
        round_keys[i] = round_key; // this round_keys array will store 16 subkeys
    }
}

string DES() {  // initialize main function of DES algorithm
    int initial_permutation[64] = {  
        58, 50, 42, 34, 26, 18, 10, 2, 
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,  // this table provide an initial rearrangement and shuffling of the bits that applied to the
        64, 56, 48, 40, 32, 24, 16, 8,  // 64-bit block of plaintext before further processing in the encryption or decryption process and
        57, 49, 41, 33, 25, 17, 9, 1,   // divide bits into two halves 32 32
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };

    int expansion_table[48] = {
        32, 1, 2, 3, 4, 5, 4, 5,
        6, 7, 8, 9, 8, 9, 10, 11,      // this exapnsion table helpl in expanding the left 32 bits of initial permutation table to 48 bits
        12, 13, 12, 13, 14, 15, 16, 17,// and logic of converting 32 bits to 48 is by using expansion table of des algorithm
        16, 17, 18, 19, 20, 21, 20, 21,
        22, 23, 24, 25, 24, 25, 26, 27,
        28, 29, 28, 29, 30, 31, 32, 1
    };
    
      /* Below S-boxes in DES are tables that perform a substitution of 6-bit input and gives out put in 4 bits, substitution_boxes [8][4][16] means 8 tables 
	  each has 4 rows and 16 columns when initial permutaion applied it divides 64 bits into two half halves and the right most 32 bits expand to 48 when
	   passed through xor function and result is divided into 8 equal parts and passed result through 8 substitution boxes for conversion of 6 bits into 4 */
	  
int substition_boxes[8][4][16] =  
     
    {{
         14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
         0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
         4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
         15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
     },
     {
         15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
         3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
         0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
         13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
     },
     {
         10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
         13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
         13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
         1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
     },
     {
         7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
         13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
         10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
         3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
     },
     {
         2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
         14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
         4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
         11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
     },
     {
         12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
         10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
         9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
         4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
     },
     {
         4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
         13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
         1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
         6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
     },
     {
         13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
         1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
         7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
         2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
     }
    };

    int permutation_tab[32] = {     
        16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,     // result of s-boxes (32 bits) further rearranged using this Array to make more encrypt.
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11,4, 25
    };

    int inverse_permutation[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31, // this is inverse permutation which applied after final round In both encryption and decryption 
        38, 6, 46, 14, 54, 22, 62, 30, // It rearrange the final result of cipher text & final result of plain text to its original order
        37, 5, 45, 13, 53, 21, 61, 29, // ensuring that the final result presented in the correct order of bits
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    string perm = "";
    for(int i = 0; i < 64; i++) {  // this loop is implementing the initial permutation on 64 bit plain text to rearrange these bits
    perm += plain_t[initial_permutation[i] - 1]; 
		}                                           /* initial_permutation[i] is retrieves the value at ith position in IP while plain_t is retrieving bit from
                                                                    plain text at position of  initial_permutation */
                                                 
    string left = perm.substr(0, 32); 
    string right = perm.substr(32, 32);         // dividng 64 bits into two equal halves left 32 bits and right 32 bits
 
    for(int i = 0; i < 16; i++) {  
        string right_expanded = "";
        
        for(int i = 0; i < 48; i++) { 
        right_expanded += right[expansion_table[i] - 1]; // expanding right half 32 bits to 48 bits using expansion table
        };
        
        // passing values of round_keys and right_expanded through parameters of xor function to make plain_t more encrypt 
        string xored = Xor(round_keys[i], right_expanded);
        string res = "";
 
        //Below for loop will run 8 times that represent 8 s-boxes and will process on expanded right half of xored 


    for(int i = 0; i < 8; i++) { // i*6 is selecting (row) two first and last bit from six bit segment and 1 is legnth means extract 1 char
        string row1 = xored.substr(i * 6, 1) + xored.substr(i * 6 + 5, 1);
        int row = CBTD(row1);
            
            // Below in continue-for loop i*6 + 1..4 is selecting (columns) remaning mid 4 bits of 6 bit segment
            
            string col1 = xored.substr(i * 6 + 1, 1) + xored.substr(i * 6 + 2, 1) + xored.substr(i * 6 + 3, 1) + xored.substr(i * 6 + 4, 1);
            int col = CBTD(col1);
            int s_value = substition_boxes[i][row][col]; // after substitution result is initializing in s_value
            res += CDTB(s_value); // res is containing result of substitution process for all eight S-boxes
    }

        string perm2 = ""; //  below for loop is representing bits of right half of 64 bits 
    for(int i = 0; i < 32; i++) {
        perm2 += res[permutation_tab[i] - 1]; // permutation is appllying on half 32 res string using permutation_tab res[permutation_tab[i]-1] 
}

        xored = Xor(perm2, left); // after permutation xor operation is applying on perm2 with remaning left half of bits

        left = xored;           // result of applied xor operation is initializeed with this string left
        if(i < 15) {
            string T_val = right; // in this part swapping of values is done
            right = xored;        // temporary value T_val is used to swap values of both left and right half of data
            left = T_val;
        }
    }

    string combined_text = left + right; // previously swapped left rigght halves concatenated here
    string ciphertext = "";

    for(int i = 0; i < 64; i++) { // the final permutation inverse is appliying on combined_text
        ciphertext += combined_text[inverse_permutation[i] - 1]; // final encrypted result is initializing 
}

    return ciphertext;
}

int main() {
	
	cout<<" 					DES Algorithm  					"<<endl;
	cout<<endl;
    string key = "1100101010111010101110010101101010101100101010101011001000110";
    plain_t = "0010110110001101111001101010101111001101000100110010010110001001";
    string altered_pt = plain_t;
    
    
    string cipher_t = DES();  // calling main DES function to implement DES algorithm
    generate_keys(key);  // passing original key to generated_keys function for encrypt/enhance original key
    
    cout<<"Plain text: "<<plain_t<<endl;
    cout<<"Ciphertext: "<<cipher_t<<endl;

    int i = 15;
    int j = 0;
    while(i > j) {  // reversig round keys arrays for decryption using swapping technique
        string T2_val = round_keys[i];
        round_keys[i] = round_keys[j];
        round_keys[j] = T2_val;
        i--; // after swappig i is decremented moving towards the beginning of the array
        j++; // after swappig j is incremented moving towards the end of the array
}

    plain_t = cipher_t; // initializing plain text plain_t = cipher text cipher_t for copying the ciphertext back into the plaintext
    string decrypted = DES(); // desc function called to perform the descryption process
    cout<<"Decrypted text: "<<decrypted<<endl;
    cout<<endl;

    if(decrypted == altered_pt) { // comparing decrypted text with altered text using logical operator for successfuly conversion of encryption & descryption
        cout<<" 	Plain text encrypted and decrypted successfully!!! "<<endl;
    }

    return 0;
}




