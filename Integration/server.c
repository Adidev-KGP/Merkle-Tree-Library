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
#include "../Verifier/verifier.h"

#define byte unsigned char

#define PORT 4455



void parse_verify(char buffer[], int *len, int *ind, int *proof_size, byte root[], byte leaf[], byte proof[][32]){
  
  char * token = strtok(buffer, " ");
  char extracted_data[270][70];
  int c=0;
  while( token != NULL ) {
      strcpy(extracted_data[c],token);
      //printf( "%s\n", extracted_data[c]); //printing each token
      token = strtok(NULL, " ");
      c++;
   }
  int *length;
  length=len;
  *length=0;
  for(int i=0;i<strlen(extracted_data[0]);i++){
      *length+=pow(10,strlen(extracted_data[0])-1-i)*(extracted_data[0][i]-'0');
  }

  int *index;
  index=ind;
  *index=0;
  for(int i=0;i<strlen(extracted_data[1]);i++){
      *index+=pow(10,strlen(extracted_data[1])-1-i)*(extracted_data[1][i]-'0');
  }

  int *proof_len;
  proof_len=proof_size;
  *proof_len=0;
  for(int i=0;i<strlen(extracted_data[2]);i++){
      *proof_len+=pow(10,strlen(extracted_data[2])-1-i)*(extracted_data[2][i]-'0');
  }

  hexstr_to_data(extracted_data[3],root);

  hexstr_to_data(extracted_data[4],leaf);

  char hex_proof[260][65];
  for(int i=0;i<*proof_size;i++){
    strcpy(hex_proof[i],extracted_data[i+5]);
  }

  final_processed_data(hex_proof, *proof_size, proof);

}

void parse_set(char buffer[], int *len, int *ind, int *proof_size, byte old_root[], byte old_leaf[], byte new_root[], byte new_leaf[], byte proof[][32]){

    char * token = strtok(buffer, " ");
    char extracted_data[270][70];
    int c=0;
    while( token != NULL ) {
        strcpy(extracted_data[c],token);
        //printf( "%s\n", extracted_data[c]); //printing each token
        token = strtok(NULL, " ");
        c++;
    }
    int *length;
    length=len;
    *length=0;
    for(int i=0;i<strlen(extracted_data[0]);i++){
        *length+=pow(10,strlen(extracted_data[0])-1-i)*(extracted_data[0][i]-'0');
    }

    int *index;
    index=ind;
    *index=0;
    for(int i=0;i<strlen(extracted_data[1]);i++){
        *index+=pow(10,strlen(extracted_data[1])-1-i)*(extracted_data[1][i]-'0');
    }

    int *proof_len;
    proof_len=proof_size;
    *proof_len=0;
    for(int i=0;i<strlen(extracted_data[2]);i++){
        *proof_len+=pow(10,strlen(extracted_data[2])-1-i)*(extracted_data[2][i]-'0');
    }

    hexstr_to_data(extracted_data[3],old_root);

    hexstr_to_data(extracted_data[4],old_leaf);

    hexstr_to_data(extracted_data[5],new_root);

    hexstr_to_data(extracted_data[6],new_leaf);

    char hex_proof[260][65];
    for(int i=0;i<*proof_size;i++){
      strcpy(hex_proof[i],extracted_data[i+7]);
    }

    final_processed_data(hex_proof, *proof_size, proof);

}

void server() {

  // Variables and structures
  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  int buffer_size  =265*70;
  char buffer[buffer_size];

  // Server socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  listen(server_fd, 20);
  printf("[LISTENING] Port Number: %d\n", PORT);
  
  char *end="end";

  client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
  printf("[CONNECTED] New Connection\n");

  while (strcmp(buffer,end)!=0) {

    memset(buffer, '\0', buffer_size*sizeof(char));
    recv(client_fd, buffer, buffer_size, 0);
    
    printf("[CLIENT] %s\n", buffer);
    
    fgets(buffer,buffer_size,stdin);
    

    if(buffer[0]=='v'){
      //printf("ok!");
      
      send(client_fd, buffer, strlen(buffer), 0);
      memset(buffer, '\0', buffer_size*sizeof(char));
      recv(client_fd, buffer, buffer_size, 0);
      printf("[CLIENT] %s\n", buffer);

      int len;int ind;int prf_s;byte root[32];byte leaf[32];byte proof[260][32];
      parse_verify(buffer,&len,&ind,&prf_s,root,leaf,proof);
      /*printf("length : %d\n",len);
      printf("index : %d\n",ind);
      printf("proof size : %d\n",prf_s);
      for(int i=0;i<32;i++){
        printf("root : %d ",root[i]);
      }
      for(int i=0;i<32;i++){
        printf("leaf : %d ",leaf[i]);
      }

      for(int i=0;i<prf_s;i++){
        for(int j=0;j<32;j++){
          printf("proof : %d ",proof[i][j]);
        }printf("\n");
      }*/

      int n= verify_merkle_proof(root, len, ind, leaf, proof, prf_s);
      if(n==1)printf("Verified! The data sent is correct\n");
      else printf("False information given\n");
      //printf("%p",&buffer);
      //parse_verify(buffer);


      fgets(buffer,buffer_size,stdin);
      if(buffer[0]=='B'){
        send(client_fd, "end", 3, 0);
        break;
      } 
      send(client_fd, buffer, strlen(buffer), 0);
      continue;
    
    }else if(buffer[0]=='s'){
      send(client_fd, buffer, strlen(buffer), 0);
      memset(buffer, '\0', buffer_size*sizeof(char));
      recv(client_fd, buffer, buffer_size, 0);
      printf("[CLIENT] %s\n",buffer);

      int len;int ind;int prf_s;
      byte old_root[32];byte old_leaf[32];
      byte new_root[32];byte new_leaf[32];
      byte proof[260][32];

      parse_set(buffer, &len, &ind, &prf_s, old_root,
                old_leaf,new_root,new_leaf,proof);
      
      int prev=verify_merkle_proof(old_root, len, ind, old_leaf, proof, prf_s);
      int new=verify_merkle_proof(new_root, len, ind, new_leaf, proof, prf_s);
      if(prev&&new)printf("Verified! The data sent is correct\n");
      else printf("False information given\n");

      fgets(buffer,buffer_size,stdin);
      if(buffer[0]=='B'){
        send(client_fd, "end", 3, 0);
        break;
      } 
      send(client_fd, buffer, strlen(buffer), 0);
      continue;

    }else if(buffer[0]=='B'){
        send(client_fd, "end", 3, 0);
        break;
    }
    send(client_fd, buffer, strlen(buffer), 0);
}

  close(client_fd);
  printf("[DISCONNECTED] Connection closed\n");

  
}

int main(){
  server();
}