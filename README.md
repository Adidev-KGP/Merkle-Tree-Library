# Project Description

There are many devices which have very limited storage and in-computation memory.
For example Hardware Wallets(HWWs) which are security devices to protect cryptographic keys because of the constraints in embedded development, have hard constraints on the amount of resources (computation, RAM, storage) available in these settings.

A solution is for devices such as the HWW to outsource holding this data to an external and more powerful device (the client), like the laptop connected to the HWW. But the client could be compromised by malware, and therefore could attempt an attack on the HWW or its user by feeding wrong or outdated information.
Merkle trees allow to make this approach safe, by producing a short commitment to the entire outsourced data. Every access request from the HWW is responded to by the client with the requested data, plus a short proof that the data is indeed correct. The HWW acts as a verifier by checking each proof before making use of any response, and will refuse to proceed on a verification failure.

Merkle trees can easily represent static arrays/lists. Updatable lists are also possible with dynamic Merkle trees; in this model, the HWW could request to update a certain element of the array, or to append a new element at the end of the list; the client would reply with the new Merkle root, and a correctness proof of the update that the HWW is able to verify.

So basically if the HWW is to get some data from the client it can receive it but to be sure that the correct data has been served to it a merkle proof is sent and using the hash of data and merkle proof HWW can verify if the data it has been sent is correct or not.

If you don't know much about Merkle Trees and Merkle Proofs you can read this  
### [article](https://decentralizedthoughts.github.io/2020-12-22-what-is-a-merkle-tree/) .

# Aim of the Project

- Maintian a library that can run in devices like hardware wallets to run the role of the verifier for the outsourced data. Current implementation is in C.

- This project aims to maintain a  standalone library for the client to produce Merkle trees, produce proofs, update a leaf by its index while producing a proof of correct update.

# About the Project

This project has 2 separate libraries:
- Pyhton library(Porver)
- C library(Verifier)

## Properties of Python library(Prover) 
- Produce a Merkle Tree from a given list of hashes
- Replace a particular leaf index with another value and produce proof of correct update(set() method of class MerkleTree in merkle.py)
- Produce Merkle Proof for a particular index.

## Properties of C library(Verifier)
- To verify using the data hash and merkle proof that the data sent to it is correct.
- It can give instructions to the Prover to replace a particular data/leaf with another one.

## Folder structure
- Python library (prover/merkle.py)
- C library (verifier/verifier.c)
- Unit tests for Python library (tests/test_Prover)
- Unit tests for C library (tests/test_Verifier)
- Integration of Python library (integration/client.py)
- Integration of C library (integration/server.c)

## READMEs
- #### [prover/README.md](https://github.com/Adidev-KGP/Merkle-Tree-Library/tree/main/prover)
- #### [verifier/README.md](https://github.com/Adidev-KGP/Merkle-Tree-Library/tree/main/verifier)
- #### [integration/README.md](https://github.com/Adidev-KGP/Merkle-Tree-Library/tree/main/integration)

## Building and running the Integration ([demo](https://www.youtube.com/watch?v=8nOzZWyNWwo))

Activate the verifier using the following commands.
``` 
~/Merkle-Tree_Library$ make
~/Merkle-Tree_Library$ ./a.out
```
Activate the prover using the following commands on a different terminal if you are on the same system.
```
~/Merkle-Tree_Library$ python3 ./integration/client.py
```
Once both the verifier and the prover have been activated you can start the communication between them.

Let me show you a demonstration of communication(prover starts the communication)

```
Prover's Terminal:
Owner:~/Merkle-Tree_Library$ python3 ./integration/client.py
Connecting to port:  ('localhost', 4455)
hello
[Server] get 3
[Server] set 3 45 int
[Server] get 7
[DSICONNECTED] Connection closed
Owner:~/Merkle-Tree_Library$ 

```

```
Verifier's Terminal:
Owner:~/Merkle-Tree_Library$ make 
gcc -g    integration/server.c verifier/verifier.c -lm -lcrypto   
Owner:~/Merkle-Tree_Library$ ./a.out
[LISTENING] Port Number: 4455
[CONNECTED] New Connection
[PROVER] hello
get 3
[PROVER] g18 3 5 1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131 6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0 17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce 684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889 6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3 d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df 
Verified! The data sent is correct
set 3 45 int
[PROVER] s18 3 5 1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131 6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0 98aaf248675ae116e4a9828e2c23a86cecc2af8151f68046693fde790b0c08f8 0b0dec6b68a0af52293c4f4e5c8046f78b71b51f88688aa09cbd448da2e62ff6 17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce 684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889 6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3 d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df 
Verified! The data sent is correct
get 7
[PROVER] g18 7 5 98aaf248675ae116e4a9828e2c23a86cecc2af8151f68046693fde790b0c08f8 efa4f1bc3a3b0b12b01b747c4cdfa962c6b542d12e2279a6550a22d080b293ea 67690b8e921ae3efb5f951edc6ac14bf88b9763706912b9d6be7e6ee8e8182aa a9a0a83e566ba4e79bafff14356d02705d39c9e4ede9707059323254f44b550c eb877bb09fb337642f8cebf28a10a51663fd613a8a68436e001f5e5252cc2c68 6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3 d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df 
Verified! The data sent is correct
B
[DISCONNECTED] Connection closed
Owner:~/Merkle-Tree_Library$ 

```
### Instructions to run the Integration

- Start the communication from the Prover's side

- The verifier can only give two queries to which the Prover will respond with data:

query 1:

```get <index_of_leaf>```

Example:

```get 3 ```


This instruction fetches the verifier the hash of the leaf with index x along with merkle proof and other data so that it can verify the data received by it.

Demo:

```
get 3
[PROVER] g18 3 5 1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131 6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0 17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce 684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889 6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3 d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df 
Verified! The data sent is correct

```

query 2:

```set <index_of_leaf> <value> <Datatype>```

Example:

```set 3 crypto string```

This instruction fetches the verifier the proof of correct update after replacing the old leaf at index <index> with the <value>.

Demo:

```
set 3 45 int
[PROVER] s18 3 5 1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131 6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0 98aaf248675ae116e4a9828e2c23a86cecc2af8151f68046693fde790b0c08f8 0b0dec6b68a0af52293c4f4e5c8046f78b71b51f88688aa09cbd448da2e62ff6 17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce 684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889 6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3 d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df 
Verified! The data sent is correct
```

Other instructions maybe:

```set 4 github string```

```set 7 football string```

- To end the communication press B on the Verifier's side.

To know about how data is sent to verifier and other in-depth things refer to  
#### [integration/README.md](https://github.com/Adidev-KGP/Merkle-Tree-Library/tree/main/integration).

## Building and running unit-tests([demo](https://www.youtube.com/watch?v=rr5vdA4gB94))

You will find the unit tests for Prover and Verifier in 
#### [tests/test_Prover](https://github.com/Adidev-KGP/Merkle-Tree-Library/tree/main/tests/test_Prover) 
and 
#### [tests/test_Verifier](https://github.com/Adidev-KGP/Merkle-Tree-Library/tree/main/tests/test_Verifier) respectively. 

### Running the Prover's unit-tests:

Run all the tests in one go:
```
Owner:~/Merkle-Tree_Library$ ./run_test.sh test_pv

Demo:

Owner:~/Merkle-Tree_Library$ ./run_test.sh test_pv

running-test_node
test-passed
test_node - success

running-test_combine_hashes
test no. 1 passed
test no. 2 passed
test no. 3 passed
test no. 4 passed
test no. 5 passed
test no. 6 passed
test no. 7 passed
test no. 8 passed
test_combine_hashes - success

running-test_element_hash
test no. 1 passed
test no. 2 passed
test no. 3 passed
test no. 4 passed
test no. 5 passed
test no. 6 passed
test no. 7 passed
test no. 8 passed
test_element_hash - success

running-test_sha256
test no. 1 passed
test no. 2 passed
test no. 3 passed
test no. 4 passed
test no. 5 passed
test no. 6 passed
test no. 7 passed
test no. 8 passed
test_sha256 - success

running-test_merkle_proofs
test no. 1 passed
test no. 2 passed
test no. 3 passed
test no. 4 passed
test no. 5 passed
test no. 6 passed
test no. 7 passed
test no. 8 passed
test no. 9 passed
test no. 10 passed
test no. 11 passed
test no. 12 passed
test no. 13 passed
test no. 14 passed
test no. 15 passed
test no. 16 passed
test no. 17 passed
test no. 18 passed
test no. 19 passed
test no. 20 passed
test no. 21 passed
test no. 22 passed
test no. 23 passed
test no. 24 passed
test no. 25 passed
test_merkle_proofs - success

running-test_set
test-passed
test_set - success

running-test_ceil_lg
test no. 1 passed
test no. 2 passed
test no. 3 passed
test no. 4 passed
test no. 5 passed
test no. 6 passed
test no. 7 passed
test_ceil_lg - success

running-test_floor_lg
test no. 1 passed
test no. 2 passed
test no. 3 passed
test no. 4 passed
test no. 5 passed
test no. 6 passed
test no. 7 passed
test_floor_lg - success

running-test_is_power_of_2
test no. 1 passed
test no. 2 passed
test no. 3 passed
test no. 4 passed
test no. 5 passed
test no. 6 passed
test no. 7 passed
test_is_power_of_2 - success

running-test_largest_power_of_2_less_than
test no. 1 passed
test no. 2 passed
test no. 3 passed
test no. 4 passed
test no. 5 passed
test no. 6 passed
test no. 7 passed
test_largest_power_of_2_less_than - success

running-test_make_tree
test-passed
test_make_tree - success

Overall tests succeeeded
Owner:~/Merkle-Tree_Library$ 

```

Run the tests separately:

To run the tests separately use the following command:

```pytest -m <marker_name>```
```

Example:

pytest -m ceil_lg --disable-warnings

Owner:~/Merkle-Tree_Library$ pytest -m ceil_lg --disable-warnings
========================================= test session starts =========================================
platform linux -- Python 3.8.10, pytest-7.1.2, pluggy-1.0.0
rootdir: /home/Owner/Merkle-Tree_Library
collected 11 items / 10 deselected / 1 selected                                                       

tests/test_Prover/test_mer_math.py .                                                            [100%]

============================ 1 passed, 10 deselected, 11 warnings in 0.54s ============================

```
You will find the marker names in the tests/test_Prover/ folder and in each .py file there are functions which start with test_
Above the definition of function prototype marker names are given 

Eg:

inside tests/test_Prover/test_cls_merkle.py you will find the following on line 167 and 168

```
@pytest.mark.set
def test_set():
```

Here ```set```is the marker name of ```test_set()``` function.

So to run test_set() we give the following command:
```
pytest -m set --disable-warnings

Demo:

Owner:~/Merkle-Tree_Library$ pytest -m set --disable-warnings
========================================= test session starts =========================================
platform linux -- Python 3.8.10, pytest-7.1.2, pluggy-1.0.0
rootdir: /home/Owner/Merkle-Tree_Library
collected 11 items / 10 deselected / 1 selected                                                       

tests/test_Prover/test_cls_merkle.py .                                                          [100%]

============================ 1 passed, 10 deselected, 11 warnings in 0.42s ============================
Owner:~/Merkle-Tree_Library$ 
```

List of all test functions with their markers:

```
@pytest.mark.set
def test_set()

@pytest.mark.merkle_proof
def test_merkle_proofs()

@pytest.mark.make_tree
def test_make_tree()

@pytest.mark.sha256
def test_sha256()

@pytest.mark.element_hash
def test_element_hash()

@pytest.mark.combine_hash
def test_combine_hashes()

@pytest.mark.floor_lg
def test_floor_lg()

@pytest.mark.ceil_lg
def test_ceil_lg()

@pytest.mark.power_of_2
def test_is_power_of_2()

@pytest.mark.largest_power_of_2_less_than
def test_largest_power_of_2_less_than()

@pytest.mark.Node
def test_node()

```

### Running the Verifier's unit-tests

To run the ```test_dataextract.c``` unit-test program:
```
Owner:~/Merkle-tree_Library$ ./run_test.sh dex
```
To run the ```test_hashfunc.c``` unit-test program:
```
Owner:~/Merkle-tree_Library$ ./run_test.sh hfn
```
To run the ```test_ver_merkle_proof.c``` unit-test program:
```
Owner:~/Merkle-tree_Library$ ./run_test.sh ver_mer
```

```
Demo:
Owner:~/Merkle-Tree_Library$ ./run_test.sh dex

* Suite the_suite:
...
3 tests - 3 passed, 0 failed, 0 skipped (38 ticks, 0.000 sec)

Total: 3 tests (174 ticks, 0.000 sec), 414 assertions
Pass: 3, fail: 0, skip: 0.
Owner:~/Merkle-Tree_Library$ ./run_test.sh hfn

* Suite the_suite:
...
3 tests - 3 passed, 0 failed, 0 skipped (822 ticks, 0.001 sec)

Total: 3 tests (913 ticks, 0.001 sec), 480 assertions
Pass: 3, fail: 0, skip: 0.
Owner:~/Merkle-Tree_Library$ ./run_test.sh ver_mer

* Suite the_suite:
.
1 test - 1 passed, 0 failed, 0 skipped (1283 ticks, 0.001 sec)

Total: 1 test (1637 ticks, 0.002 sec), 30 assertions
Pass: 1, fail: 0, skip: 0.
Owner:~/Merkle-Tree_Library$ 
```

# Further Work

The project is open-source. There are many extensions to this project like adding the feature to add new leaf to the Merkle Tree OR sending data to the verifier as buffer to minimize the memory storage.

Thanks for the patience :)



