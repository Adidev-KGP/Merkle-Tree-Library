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
    if(str[0] >= '0' && str[0] <= '9'){
        num += 16*(str[0]-'0');

        if(str[1] >= '0' && str[1] <= '9') num += (str[1]-'0');
        else num += (str[1]-'a'+10);

    }else {
        num += 16*(str[0]-'a'+10);
        
        if(str[1] >= '0' && str[1] <= '9') num += (str[1]-'0');
        else num += (str[1]-'a'+10);
    }
    return num;
}

void hexstr_to_data(char *hex_str, byte byte_data[]){
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

    int c = 0;
    for(int i = 0; i<64; i+=2){
        char str[3];
        str[0] = hex_str[i];
        str[1] = hex_str[i+1];
        str[2] = '\0';
        byte_data[c] = hexstr_to_num(str);
        c++;
    }
}

void final_processed_data(char hex_data[][65], int size, byte final_data[][32]){
    /*
        This function converts accepts pointer to a char array.
        hex_data[][65] has the elements in hexadecimal string 
        which are converted into bytes ans stored in final_data.

    */

    for(int i = 0; i<size; i++){
        hexstr_to_data(hex_data[i], final_data[i]);
    }

}

void sha256(byte data[], byte sha256_hash[], int n){

    //initilized a pointer to the type EVP_MD_CTX
    EVP_MD_CTX *mdctx;

    int md_len;
    mdctx = EVP_MD_CTX_create();

    //pointer mdctx is initialized with the EVP_SHA256() operation
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);

    //EVP_DigestUpdate keeps on adding bytes after bytes to give SHA256 hash
    EVP_DigestUpdate(mdctx, data , n);
    EVP_DigestFinal_ex(mdctx, sha256_hash, &md_len);

    //pointer mdctx in destroyed
    EVP_MD_CTX_destroy(mdctx);
    EVP_cleanup();
}


void element_hash(byte data[], byte ele_hash[], int n){
    /*
        Given a pointer to array in unsigned char,
        this function finds the SHA256 hash of that data.
    */

    byte main_data[n+1];
    main_data[0] = 0;
    for(int i = 1; i < n+1; i++){
        main_data[i] = data[i-1];
    }
    sha256(main_data, ele_hash, n+1);
}

void combine_hashes(byte data1[], byte data2[], byte com_hash[]){
    /*
        Given two data namely data1 and data2 as unsigned char pointers,
        this function combines both the data bytes and computes the SHA256 hash.
    */
    byte com_data[65];
    com_data[0] = 1;
    for(int i = 1; i <= 32; i++){
        com_data[i] = data1[i-1];
    }

    for(int i = 33; i <= 64; i++){
        com_data[i] = data2[i-33];
    }
    sha256(com_data, com_hash, 65);
}

int verify_merkle_proof_leaves_power_of_2(byte root[], int len, int index, byte leaf[], byte proof[][32], int proof_size){
    /*
    Given a Merkle Tree having number leaves equal to some power of 2, 
    verify_merkle_proof_leaves_power_of_2() computes the root hash of such 
    a Merkle Tree from the Merkle Proof and the leaf hash and returns
    whether the final hash it calculated equals the root hash.
    Returns 1 is the calculated_hash == root hash else 0.

    Time complexity : O(log2(N)) where N is the number of leaves
    */

    index += 1;
    byte ver_root[32];

    if(index != len){
        for(int i = 0; i<proof_size; i++){
            if(index%2 == 0){
                combine_hashes(proof[i], leaf, ver_root);
                for(int i = 0; i < 32; i++)leaf[i] = ver_root[i];
                
            }else{
                combine_hashes(leaf, proof[i], ver_root);
                for(int i = 0; i < 32; i++)leaf[i] = ver_root[i];
            }
            index = ((index/2) + (index%2 !=0));
        }
    }else if(index == len){
        for(int i = 0; i<proof_size; i++){
            combine_hashes(proof[i], leaf, ver_root);
            for(int i = 0; i < 32; i++)leaf[i] = ver_root[i];
        }
    }

    int c = 0;
    for(int i = 0; i < 32; i++){
        if(ver_root[i] == root[i])c = 1;
        else{
            c = 0;
            break;
        }
    }
    if(c == 1)return 1;
    else return 0;
}


int verify_merkle_right_branch(char dir, byte root[], int index, byte leaf[], byte proof[][32], int proof_size){
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
        int c = 0;
        for(int i = 0; i < 32; i++){
            if(leaf[i] == root[i])c = 1;
            else{
                c = 0;
                break;
            }
        }
        return c;
    }

    if(dir == 'l'){
        combine_hashes(leaf, proof[index], leaf);
    }else if(dir == 'r'){
        combine_hashes(proof[index], leaf, leaf);
    }

    byte leaf_cpy[32];
    for(int i=0;i<32;i++){
        leaf_cpy[i]=leaf[i];
    }

    int x = verify_merkle_right_branch('l', root, index+1, leaf_cpy, proof, proof_size);
    for(int i=0;i<32;i++){
        leaf_cpy[i]=leaf[i];
    }

    int y = verify_merkle_right_branch('r', root, index+1, leaf_cpy, proof, proof_size);
    for(int i=0;i<32;i++){
        leaf_cpy[i]=leaf[i];
    }
    
    return x||y;
}

int verify_merkle_proof(byte root[], int len, int index, byte leaf[], byte proof[][32], int proof_size){
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
    int left = 1;
    while(left < len){
        left *= 2;
    }left /= 2;

    if(left*2 == len){
        int x = verify_merkle_proof_leaves_power_of_2(root, len, index, leaf, proof, proof_size);
        return x;
    }else{

        if(index+1 <= left){
            int x = verify_merkle_proof_leaves_power_of_2(root, len, index, leaf, proof, proof_size);
            return x;
        }else{
            if(index+1 == len){
                int x = verify_merkle_right_branch('r', root, 0, leaf, proof, proof_size);
                return x;
            }

            if((index+1)%2 == 0 && (index+1) != len){
                int x = verify_merkle_right_branch('r', root, 0, leaf, proof, proof_size);
                return x;
            }else if((index+1)%2 !=0 && (index+1) != len){
                int x =verify_merkle_right_branch('l', root, 0, leaf, proof, proof_size);
                return x;
            }
        }

    }

}

int check(byte root[], int index, byte leaf[], byte proof[][32], int proof_size){
    if(index == proof_size){
        int c=0;
        for(int i=0;i<32;i++){
            if(leaf[i] == root[i])c = 1;
            else{
                c=0;
                break;
            }
        }
        printf("%d\n",c);
        return c;
    }
    
    combine_hashes(proof[index],leaf,leaf);
    int x = check(root,index+1, leaf, proof, proof_size);
    return x;
}
