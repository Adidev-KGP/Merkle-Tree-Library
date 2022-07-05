#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

#define byte unsigned char
#include "verifier.h"


int hexstr_to_num(char str[]){

    /*

        This function converts a  1 byte hexadecimal string into  a decimal number
        which is the value in bytes of the heaxadecimal string.

        Eg: "ae" -> 174 

    */

    int num = 0;
    if(str[0]>='0' && str[0]<='9'){
        num += 16*(str[0]-'0');

        if(str[1]>='0' && str[1]<='9') num += (str[1]-'0');
        else num += (str[1]-'a'+10);

    }else {
        num +=16*(str[0]-'a'+10);
        
        if(str[1]>='0' && str[1]<='9') num += (str[1]-'0');
        else num += (str[1]-'a'+10);
    }
    return num;
}

void hexstr_to_data(char *hex_str , byte byte_data[]){

    /*

        This function converts a 32 byte long hexadecimal string into actual bytes and 
        stores the values of each byte in the pointer parameter byte_data[].

        Eg: 360f84035942243c6a36537ae2f8673485e6c04455a0a85a0db19690f2541480

            is stored as bytes in byte_data[] as integer values which represent bytes

            {
                1 , 150 , 43 , 20 , 135 , 222 , 69 , 169 , 213 ,
                118 , 34 , 23 , 48 , 209 , 107 , 113 , 169 , 
                202 , 79 , 65 , 93 , 13 , 192 , 159 , 23 , 200 , 
                41 , 36 , 22 , 157  ,86 , 153
            }

    */

    int c=0;
    for(int i=0;i<64;i+=2){
        char str[3];
        str[0]=hex_str[i];
        str[1]=hex_str[i+1];
        str[2]='\0';
        byte_data[c] = hexstr_to_num(str);
        c++;
    }
}

void final_processed_data(char hex_data[][65] , int size , byte final_data[][32]){

    for(int i=0;i<size;i++){
        hexstr_to_data(hex_data[i], final_data[i]);
    }

}

void sha256(byte data[] , byte sha256_hash[] , int n){
    EVP_MD_CTX *mdctx;

    int md_len;
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);

    EVP_DigestUpdate(mdctx, data , n);
    EVP_DigestFinal_ex(mdctx, sha256_hash, &md_len);
    EVP_MD_CTX_destroy(mdctx);
    EVP_cleanup();
}


void element_hash(byte data[] , byte ele_hash[] , int n){
    byte main_data[n+1];
    main_data[0]=0;
    for(int i=1;i<n+1;i++){
        main_data[i] = data[i-1];
    }

    sha256(main_data , ele_hash , n+1);
}

void combine_hashes(byte data1[] , byte data2[] , byte com_hash[]){
    byte com_data[65];
    com_data[0]=1;
    for(int i=1;i<=32;i++){
        com_data[i] = data1[i-1];
    }

    for(int i=33;i<=64;i++){
        com_data[i] = data2[i-33];
    }

    sha256(com_data , com_hash , 65);
}

int verify_merkle_proof_leaves_power_of_2(byte root[] , int len , int index , byte leaf[] , byte proof[][32] ,int proof_size){

    /*
        
    Given a Merkle Tree having number leaves equal to some power of 2, 
    verify_merkle_proof_leaves_power_of_2() computes the root hash of such 
    a Merkle Tree from the Merkle Proof and the leaf hash and returns
    the final root hash it calculated.
    Time complexity : O(log2(N)) where N is the number of leaves
    
    
    */

    index+=1;
    byte ver_root[32];

    if(index !=len){
        for(int i=0;i<proof_size;i++){
            if(index %2 ==0){
                combine_hashes(proof[i],leaf,ver_root);
                for(int i=0;i<32;i++)leaf[i] = ver_root[i];
                
            }else{
                combine_hashes(leaf,proof[i],ver_root);
                for(int i=0;i<32;i++)leaf[i] = ver_root[i];
            }
            index = ((index/2) + (index%2 !=0));
        }
    }else if(index == len){
        for(int i=0;i<proof_size;i++){
            combine_hashes(proof[i],leaf,ver_root);
            for(int i=0;i<32;i++)leaf[i] = ver_root[i];
        }
    }

    int c=0;
    for(int i=0;i<32;i++){
        if(ver_root[i]== root[i])c=1;
        else{
            c=0;
            break;
        }
    }
    if(c==1)return 1;
    else return 0;

}






int verify_merkle_right_branch(char dir , byte root[] , int len , int index , byte leaf[] , byte proof[][32] ,int proof_size){


    /*
        
        Given a Merkle Tree which has number of leaves not equal to some power of 2,
        ver_merkle_right_branch() computes the root hash of such a Merkle Tree 
        from the Merkle Proof and the leaf and it is known that the leaf is in the 
        right branch of the Merkle Tree where the number of leaves is not a power of 2.
        
        Although ver_merkle_right_branch() can be used to compute the root hash of any
        Merkle Tree using any of its leaves and the corresponding merkle proof
        regardless of whether the leaf is in the left branch or the right branch but 
        since this function involves recursion therefore calculating the root hash 
        of Merkle Tree using a leaf in the left branch can take as minimum as O(log2(N))
        time to as maximum as O(N) time .
        
        Therefore it is advisable to use ver_merkle_right_branch only when:
        1) The Merkle Tree has number of leaves not equal to some power of 2
        2) The leaf using which root hash is computed is in the right branch 
        of the Merkle Tree.
    
    
    */

    if(index == proof_size){
        int c=0;
        for(int i =0;i<32;i++){
            if(leaf[i] == root[i])c=1;
            else c=0;
        }

        if(c==1)return 1;
        else return 0;
    }

    if(dir == 'l'){
        combine_hashes(leaf , proof[index] , leaf);
    }else if(dir == 'r'){
        combine_hashes(proof[index] , leaf , leaf);
    }

    int x = verify_merkle_right_branch('l' , root , len , index+1 , leaf , proof , proof_size);
    if(x==1)return x;

    int y = verify_merkle_right_branch('r' , root , len , index+1 , leaf , proof , proof_size);
    if(y==1)return y;

}

int verify_merkle_proof(byte root[] , int len , int index , byte leaf[] , byte proof[][32] ,int proof_size){

    /*

        
        verify_merkle_proof uses a combination of 2 functions to compute root hash:

            1) verify_merkle_proof_leaves_power_of_2():
            this function is used if the Merkle Tree has number of leaves equal
            to some power of 2 or if the leaf is in the left branch of Merkle Tree.
            
            2) ver_merkle_right_branch():
            this function is used when the number of leaves in Merkle Tree
            is not a power of 2 and the leaf is in the right branch of Merkle Tree.
    
    */


    //variable left stores the the largest power of 2 less than number of leaves   
    int left=1;
    while(left<len){
        left*=2;
    }left/=2;

    if(left*2 == len){
        int x=verify_merkle_proof_leaves_power_of_2(root,len,index,leaf,proof,proof_size);
        return x;
    }else{

        if(index+1 <=left){
            int x =verify_merkle_proof_leaves_power_of_2(root,len,index,leaf,proof,proof_size);
            return x;
        }else{
            if(index+1 == len){
                int x =verify_merkle_right_branch('r',root,len,0,leaf,proof,proof_size);
                return x;
            }

            if((index+1)%2 ==0 && (index+1)!=1){
                int x =verify_merkle_right_branch('r',root,len,0,leaf,proof,proof_size);
                return x;
            }else if((index+1)%2 !=0 && index+1 !=len){
                int x =verify_merkle_right_branch('l',root,len,0,leaf,proof,proof_size);
                return x;
            }
        }

    }

}


//int main(){

    /*byte data1[5] = {102, 114, 111, 110, 116};
    byte data2[4] = {98, 97, 99, 107};
    byte ele_hash1[32];
    byte ele_hash2[32];
    element_hash(data1, ele_hash1, 5);
    element_hash(data2, ele_hash2, 4);

    byte com_hash[32];
    combine_hashes(ele_hash1, ele_hash2, com_hash);
    for(int i=0;i<32;i++){
        printf("%d\n",com_hash[i]);
    }*/

    /*int n;
    scanf("%d\n",&n);
    byte data[n];
    for(int i=0;i<n;i++){
        scanf("%hhd",&data[i]);
    }

    byte data_hash[32];
    element_hash(data, data_hash, n);

    for(int i=0;i<32;i++){
        printf("%d\n",data_hash[i]);
    }*/

    /*int n;
    scanf("%d",&n);

    char str[n];
    scanf("%s",str);

    printf("%s",str);

    return 0;*/

    //printf("%d",n);

//}