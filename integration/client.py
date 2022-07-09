# pylint: disable=missing-module-docstring
from __future__ import print_function
import socket
import sys
sys.path.append("")

from prover.merkle import MerkleTree, element_hash
from data import transactions

#object of Class MerkleTree is created
OBJ = MerkleTree(transactions)

CLIENT = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
SERVER_ADDRESS = ('localhost', 4455)

CLIENT.connect(SERVER_ADDRESS)
print("Connecting to port: ", SERVER_ADDRESS)
MESSAGE = input()

CLIENT.sendall(MESSAGE.encode())

while MESSAGE != "end":
    DATA = CLIENT.recv(1000).decode()
    DATA = DATA.strip()

    if DATA == "end":
        print("[DSICONNECTED] Connection closed")
        break

    print("[Server]", DATA)

    #this if statement deals with intsructions from the verifier starting with get
    #data sent to verifier is in the following sequence
    #(length, index, proof_size, root_hash, leaf_hash, proof)
    if DATA.split(" ")[0] == "get":
        leaf = OBJ.get(int(DATA.split(" ")[1]))
        proof = OBJ.prove_leaf(int(DATA.split(" ")[1]))
        MESSAGE = "g"
        MESSAGE = MESSAGE+str(OBJ.__len__())+" "
        MESSAGE = MESSAGE+DATA.split(" ")[1]+" "
        MESSAGE = MESSAGE+str(len(proof))+" "
        MESSAGE = MESSAGE+OBJ.root.hex()+" "
        MESSAGE = MESSAGE+leaf.hex()+" "

        for i in proof:
            MESSAGE = MESSAGE+i.hex()+" "
        MESSAGE.strip()

        CLIENT.sendall(MESSAGE.encode())

    #this elif statement deals with intsructions from the verifier starting with set
    #data sent to verifier is in the following sequence
    #(length, index, proof_size, old_root_hash, old_leaf_hash, new_root_hash, new_leaf_hash, proof)
    elif DATA.split(" ")[0] == "set":
        index = int(DATA.split(" ")[1])
        DATA_type = DATA.split(" ")[3]
        VALUE = DATA.split(" ")[2]

        if DATA_type == "string":
            VALUE = VALUE.encode()
        elif DATA_type == "int":
            VALUE = int(VALUE)
            VALUE = VALUE.to_bytes(20, 'big')
        elif DATA_type == "double":
            VALUE = float(VALUE)

        proof = OBJ.prove_leaf(index)

        proof_update = OBJ.set(index, VALUE)
        MESSAGE = "s"
        MESSAGE = MESSAGE+str(OBJ.__len__())+" "
        MESSAGE = MESSAGE+DATA.split(" ")[1]+" "
        MESSAGE = MESSAGE+str(len(proof))+" "

        for i in range(0, 3):
            MESSAGE = MESSAGE+(proof_update[i]).hex()+" "

        MESSAGE = MESSAGE+element_hash(VALUE).hex()+" "
        for i in proof_update[3:]:
            MESSAGE = MESSAGE+i.hex()+" "
        MESSAGE.strip()
        CLIENT.sendall(MESSAGE.encode())

    #this elif statement deals with intsructions from the verifier starting with add
    #data sent to verifier is in the following sequence
    #(length, index, proof_size, root_hash, leaf_hash, proof)
    elif DATA.split(" ")[0] == "add":
        DATA_type = DATA.split(" ")[2]
        VALUE = DATA.split(" ")[1]
        if DATA_type == "string":
            VALUE = VALUE.encode()
        elif DATA_type == "int":
            VALUE = int(VALUE)
            VALUE = VALUE.to_bytes(20, 'big')

        OBJ.add(VALUE)

        INDEX = OBJ.__len__() - 1

        leaf = OBJ.get(INDEX)
        proof = OBJ.prove_leaf(INDEX)
        MESSAGE = "a"
        MESSAGE = MESSAGE+str(INDEX + 1)+" "
        MESSAGE = MESSAGE+str(INDEX)+" "
        MESSAGE = MESSAGE+str(len(proof))+" "
        MESSAGE = MESSAGE+OBJ.root.hex()+" "
        MESSAGE = MESSAGE+leaf.hex()+" "

        for i in proof:
            MESSAGE = MESSAGE+i.hex()+" "
        MESSAGE.strip()

        CLIENT.sendall(MESSAGE.encode())

    else:
        MESSAGE = input()
        CLIENT.sendall(MESSAGE.encode())

CLIENT.close()
