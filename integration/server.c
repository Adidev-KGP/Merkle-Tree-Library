#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server.h"
#include <math.h>
#include "../verifier/verifier.h"

#define PORT 4455


void parse_get(char buffer[], int *len, int *ind, int *proof_size, byte root[], byte leaf[], byte proof[][32]){
  /*
      get x fetches the following information from the prover in the given sequence:
      1) size of Merkle Tree
      2) index x
      3) proof_size
      4) root_hash
      5) leaf_hash
      6) Merkle proof

      Eg - get 3
      [PROVER] g18 3 5 
      1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131 -->root_hash
      6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0 -->leaf_hash
      17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b 
      f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce 
      684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889 
      6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3 
      d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df

      g here represents that it was a get instruction

      parse_get() breaks down this information and helps to store them
      into relevamt datatypes
  */
  char *token = strtok(buffer, " ");
  char extracted_data[270][70];
  int c = 0;
  while( token != NULL ) {
      strcpy(extracted_data[c], token);
      token = strtok(NULL, " ");
      c++;
   }

  /* extracting size of Merkle Tree */
  int *length;
  length = len;
  *length = 0;
  for(int i = 1; i < strlen(extracted_data[0]); i++){
      *length += pow(10, strlen(extracted_data[0])-1-i)*(extracted_data[0][i]-'0');
  }
  /* extracting size of Merkle Tree */

  /* extracting index of leaf */
  int *index;
  index = ind;
  *index = 0;
  for(int i = 0; i < strlen(extracted_data[1]); i++){
      *index += pow(10,strlen(extracted_data[1])-1-i)*(extracted_data[1][i]-'0');
  }
  /* extracting index of leaf */

  /* extracting size of proof */
  int *proof_len;
  proof_len = proof_size;
  *proof_len = 0;
  for(int i = 0; i < strlen(extracted_data[2]); i++){
      *proof_len += pow(10,strlen(extracted_data[2])-1-i)*(extracted_data[2][i]-'0');
  }
  /* extracting size of proof */

  hexstr_to_data(extracted_data[3], root);  //extracting root hash

  hexstr_to_data(extracted_data[4], leaf);  //extracting leaf hash

  char hex_proof[260][65];
  for(int i = 0; i < *proof_size; i++){
    strcpy(hex_proof[i], extracted_data[i+5]);
  }
  
  final_processed_data(hex_proof, *proof_size, proof);  //extracting Merkle Proof
}

void parse_set(char buffer[], int *len, int *ind, int *proof_size, byte old_root[], byte old_leaf[], byte new_root[], byte new_leaf[], byte proof[][32]){
  /*
      set x <value> <Datatype> fetches the following information from the prover
      in the given sequence:
      1) size of Merkle Tree
      2) index x
      3) proof_size
      4) old_root_hash
      5) old_leaf_hash
      6) new_root_hash
      7) new_leaf_hash
      8) Merkle proof

      Eg - set 3 45 int
      [PROVER] s18 3 5 
      1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131  -->old_root_hash
      6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0  -->old_leaf_hash
      98aaf248675ae116e4a9828e2c23a86cecc2af8151f68046693fde790b0c08f8  -->new_root_hash
      0b0dec6b68a0af52293c4f4e5c8046f78b71b51f88688aa09cbd448da2e62ff6  -->new_leaf_hash
      17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b 
      f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce 
      684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889 
      6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3 
      d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df

      s here represents that it was a set instruction
      
      parse_set() breaks down this information and helps to store them
      into relevamt datatypes
  */

    char * token = strtok(buffer, " ");
    char extracted_data[270][70];
    int c = 0;
    while(token != NULL) {
        strcpy(extracted_data[c], token);
        token = strtok(NULL, " ");
        c++;
    }

    /* extracting size of Merkle Tree */
    int *length;
    length = len;
    *length = 0;
    for(int i = 1; i < strlen(extracted_data[0]); i++){
        *length += pow(10,strlen(extracted_data[0])-1-i)*(extracted_data[0][i]-'0');
    }
    /* extracting size of Merkle Tree */

    /* extracting index of leaf */
    int *index;
    index = ind;
    *index = 0;
    for(int i = 0; i < strlen(extracted_data[1]); i++){
        *index += pow(10,strlen(extracted_data[1])-1-i)*(extracted_data[1][i]-'0');
    }
    /* extracting index of leaf */

    /* extracting size of proof */
    int *proof_len;
    proof_len = proof_size;
    *proof_len = 0;
    for(int i = 0; i < strlen(extracted_data[2]); i++){
        *proof_len += pow(10,strlen(extracted_data[2])-1-i)*(extracted_data[2][i]-'0');
    }
    /* extracting size of proof */

    hexstr_to_data(extracted_data[3], old_root);  //extracting old root hash
    hexstr_to_data(extracted_data[4], old_leaf);  //extracting old leaf hash
    hexstr_to_data(extracted_data[5], new_root);  //extracting new root hash
    hexstr_to_data(extracted_data[6], new_leaf);  //extracting new leaf hash

    char hex_proof[260][65];
    for(int i = 0; i < *proof_size; i++){
      strcpy(hex_proof[i], extracted_data[i+7]);
    }

    final_processed_data(hex_proof, *proof_size, proof);  //extracting Merkle proof
}

void server(){

  // Variables and structures
  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  int buffer_size  =270*70;
  char buffer[buffer_size];

  // Server socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  listen(server_fd, 20);
  printf("[LISTENING] Port Number: %d\n", PORT);
  
  char end[4] = "end";

  client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
  printf("[CONNECTED] New Connection\n");

  while (strcmp(buffer,end) != 0) {

    memset(buffer, '\0', buffer_size*sizeof(char));
    recv(client_fd, buffer, buffer_size, 0);  // reveiving information from PROVER
    
    printf("[PROVER] %s\n", buffer);
    if(buffer[0] == 'g'){
      int len;
      int ind;
      int prf_s;
      byte root[32];
      byte leaf[32];
      byte proof[260][32];
      parse_get(buffer, &len, &ind, &prf_s, root, leaf, proof);

      int n = verify_merkle_proof(root, len, ind, leaf, proof, prf_s);
      if(n == 1)printf("Verified! The data sent is correct\n");
      else printf("False information given\n");
    }else if(buffer[0] == 's'){
      
      int len;
      int ind;
      int prf_s;
      byte old_root[32];
      byte old_leaf[32];
      byte new_root[32];
      byte new_leaf[32];
      byte proof[260][32];

      parse_set(buffer, &len, &ind, &prf_s, old_root,
                old_leaf, new_root, new_leaf, proof);
      
      int prev = verify_merkle_proof(old_root, len, ind, old_leaf, proof, prf_s);
      int new = verify_merkle_proof(new_root, len, ind, new_leaf, proof, prf_s);

      if(prev && new){
        printf("Verified! The data sent is correct\n");
      }
      else{
        printf("False information given\n");
      }
    }else if(buffer[0] == 'a'){
      int len;
      int ind;
      int prf_s;
      byte root[32];
      byte leaf[32];
      byte proof[260][32];
      parse_get(buffer, &len, &ind, &prf_s, root, leaf, proof);

      int n = verify_merkle_proof(root, len, ind, leaf, proof, prf_s);
      if(n == 1)printf("Verified! The data has been succesfully added\n");
      else printf("OOPs! Some problem occured\n");
    }
    fgets(buffer,buffer_size,stdin);
    
    if(buffer[0] == 'g'){
      send(client_fd, buffer, strlen(buffer), 0);  // sending information from PROVER
      memset(buffer, '\0', buffer_size*sizeof(char));
      recv(client_fd, buffer, buffer_size, 0);  // reveiving information from PROVER
      printf("[PROVER] %s\n", buffer);

      int len;
      int ind;
      int prf_s;
      byte root[32];
      byte leaf[32];
      byte proof[260][32];
      parse_get(buffer, &len, &ind, &prf_s, root, leaf, proof);

      int n = verify_merkle_proof(root, len, ind, leaf, proof, prf_s);
      if(n == 1){
        printf("Verified! The data sent is correct\n");
      }
      else{
        printf("False information given\n");
      }

      fgets(buffer, buffer_size, stdin);
      if(buffer[0] == 'B'){
        send(client_fd, "end", 3, 0);  // sendving information from PROVER
        break;
      } 
      send(client_fd, buffer, strlen(buffer), 0);  // sending information from PROVER
      continue;
    
    }else if(buffer[0] == 's'){
      send(client_fd, buffer, strlen(buffer), 0);  // sending information from PROVER
      memset(buffer, '\0', buffer_size*sizeof(char));
      recv(client_fd, buffer, buffer_size, 0);  // reveiving information from PROVER
      printf("[PROVER] %s\n", buffer);

      int len;
      int ind;
      int prf_s;
      byte old_root[32];
      byte old_leaf[32];
      byte new_root[32];
      byte new_leaf[32];
      byte proof[260][32];

      parse_set(buffer, &len, &ind, &prf_s, old_root,
                old_leaf, new_root, new_leaf, proof);
      
      int prev = verify_merkle_proof(old_root, len, ind, old_leaf, proof, prf_s);
      int new = verify_merkle_proof(new_root, len, ind, new_leaf, proof, prf_s);
      
      if(prev && new){
        printf("Verified! The data sent is correct\n");
      }
      else{
        printf("False information given\n");
      }

      fgets(buffer, buffer_size, stdin);
      if(buffer[0] == 'B'){
        send(client_fd, "end", 3, 0);  // sending information from PROVER
        break;
      } 
      send(client_fd, buffer, strlen(buffer), 0);  // sending information from PROVER
      continue;

    }else if(buffer[0] == 'a'){
      send(client_fd, buffer, strlen(buffer), 0);  // sending information from PROVER
      memset(buffer, '\0', buffer_size*sizeof(char));
      recv(client_fd, buffer, buffer_size, 0);  // reveiving information from PROVER
      printf("[PROVER] %s\n", buffer);

      int len;
      int ind;
      int prf_s;
      byte root[32];
      byte leaf[32];
      byte proof[260][32];
      parse_get(buffer, &len, &ind, &prf_s, root, leaf, proof);

      int n = verify_merkle_proof(root, len, ind, leaf, proof, prf_s);
      if(n == 1){
        printf("Verified! The data has been succesfully added\n");
      }
      else{
        printf("OOPs! Some problem occured\n");
      }

      fgets(buffer, buffer_size, stdin);
      if(buffer[0] == 'B'){
        send(client_fd, "end", 3, 0);  // sendving information from PROVER
        break;
      } 
      send(client_fd, buffer, strlen(buffer), 0);  // sending information from PROVER
      continue;
    
    }else if(buffer[0] == 'B'){
        send(client_fd, "end", 3, 0);  // sending information from PROVER
        break;
    }
    send(client_fd, buffer, strlen(buffer), 0);  // sending information from PROVER
}
  close(client_fd);  //closing the connection
  printf("[DISCONNECTED] Connection closed\n");
 
}

int main(){
  server();
  return 0;
}