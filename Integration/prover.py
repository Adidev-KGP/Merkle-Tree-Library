from __future__ import print_function
from email import message
from re import T
import socket

from urllib3 import proxy_from_url
from data import transactions

import sys
sys.path.append("")

from Prover.merkle import MerkleTree , element_hash


obj=MerkleTree(transactions)

client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server_address = ('localhost',4455)

client.connect(server_address)
print("Connecting to port: ",server_address)
message = input()

client.sendall(message.encode())

while message!="end":
    data = client.recv(1000).decode()

    if(data=="end"):
        print("[DSICONNECTED] Connection closed")
        break

    print("[Server] ",data)
    if(data.split(" ")[0] == "get"):
        leaf=obj.get(int(data.split(" ")[1]))
        message = leaf.hex()
        client.sendall(message.encode())
        
    elif(data.split(" ")[0]=="verify"):
        leaf=obj.get(int(data.split(" ")[1]))
        proof = obj.prove_leaf(int(data.split(" ")[1]))
        message = ""
        #message.append(str(obj.__len__()))
        message=message+str(obj.__len__())+" "
        #message.append(data.split(" ")[1][0])
        message=message+data.split(" ")[1][0]+" "
        #message.append(str(len(proof)))
        message=message+str(len(proof))+" "
        #message.append(obj.root.hex())
        message=message+obj.root.hex()+" "

        message=message+leaf.hex()+" "
        for i in proof:
            message=message+i.hex()+" "
        message.strip()
        client.sendall(message.encode())

    elif(data.split(" ")[0]=="set"):

        index = int(data.split(" ")[1])
        data_type = data.split(" ")[3]
        value = data.split(" ")[2]
        if(data_type=="string"):
            value=bytes(value)
        elif(data_type=="int"):
            value = int(value)
        elif(data_type=="double"):
            value = float(value)
        value = value.encode()

        proof = obj.prove_leaf(3)
        
        proof_update = obj.set(index , value)
        message=  ""
        message=message+str(obj.__len__())+" "
        message=message+data.split(" ")[1][0]+" "
        message=message+str(len(proof))+" "

        for i in range(0,3):
            message=message+(proof_update[i]).hex()+" "
        
        message=message+element_hash(value).hex()+" "
        for i in proof_update[3:]:
            message=message+i.hex()+" "
        message.strip()
        client.sendall(message.encode())
        

    else:
        message = input()
        client.sendall(message.encode())
        
client.close()
