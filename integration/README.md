# Integration of C and python libraries

Both the libraries are currently able to communicate using sockets over TCP/IP stack on port ```4455```. But there can be many modifications in this case and we may implement them in the future as well.

## Flow of communication
- The Prover is started
- The verifier is started
- Now the connection is established
- The first message is sent by the prover to verifier
- Now the verifier can message or query the prover. Regarding the query please read the main README.md on home page of repository. It covers the queries with examples and demonstrations.
- Prover responds to the queries with relevant information.
- To close the communication the verifier pressed ```B```.

## Data Transfer format and its processing

### QUERY : get

For the query of format ```get <index>``` the prover responds with a string having the following informations separated by a space
- size of Merkle Tree
- index
- Size of Merkle Proof
- Root hash
- Leaf hash
- Merkle Proof

On terminal it looks as follows:

```
get 3
[PROVER] g18 3 5 1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131 6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0 17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce 684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889 6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3 d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df 
Verified! The data sent is correct

```
Breaking down the information

- ```g``` represents that it is a get instruction
- 18 is the size of Merkle Tree
- 3 is the index
- 5 is the size of Merkle Proof
- 1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131 is the root hash in hexadecimal format
- 6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0 is the leaf hash of index 3 in hexadecimal format
- Below bullet points are the elements of the Merkle Proof
- 17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b 
- f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce
- 684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889
- 6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3
- d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df

To extract all this information from a single string ```parse_get()``` function defined in ```server.c``` in used.

The length of the whole string is safely less than ```270*70``` or ```18900``` because we are using sha256 as the hash function and there can be a total of only ```2^256``` sha256 hashes each of length ```64``` in hexadecimal format. So here we get ```256*64```. Also leaf hash and root hash are there in the string therefore the exact length can be considered to be ```258*64```. Since there are spaces and integers which will be less than or equal to ```2^256``` since they are size of Merkle tree, index which is but obvious less than size of Merkle Tree and size of proof which is log of size of Merkle Tree.
So it is totally safe if we consider ```18900``` as the maximum length of string though this is also much greater than the actual string can ever be.

Since each element of the string is a standard ASCII value therefore each string element is 1 byte. So the maximum size of string that is sent to verifier is 18900 bytes or approximately ```18.5 MB```.

### QUERY : set

For the query of format ```set <index> <value> <data_type>``` the prover responds with a string having the following informations separated by a space
- size of Merkle Tree
- index
- Size of Merkle Proof
- Old root hash
- Old leaf hash
- New root hash
- New leaf hash
- Merkle Proof

On terminal it looks as follows:

```
set 3 45 int
[PROVER] s18 3 5 1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131 6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0 98aaf248675ae116e4a9828e2c23a86cecc2af8151f68046693fde790b0c08f8 0b0dec6b68a0af52293c4f4e5c8046f78b71b51f88688aa09cbd448da2e62ff6 17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce 684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889 6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3 d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df 
Verified! The data sent is correct
```
Breaking down the information

- ```s``` represents that it is a set instruction
- 18 is the size of Merkle Tree
- 3 is the index
- 5 is the size of Merkle Proof
- 1a9902a9e309d97ca1968208eb86bdbb9d8c4e4f2087ff37f26d281366215131 is the old root hash in hexadecimal format
- 6466af8187ef517b640c1d68179fa0a4e906c58e79473ae8cf133cf74a8c12f0 is the old leaf hash of index 3 in hexadecimal format.
- 98aaf248675ae116e4a9828e2c23a86cecc2af8151f68046693fde790b0c08f8 is the new root hash of index 3 in hexadecimal format.
- 0b0dec6b68a0af52293c4f4e5c8046f78b71b51f88688aa09cbd448da2e62ff6 is the new leaf hash of index 3 in hexadecimal format.
- Below bullet points are the elements of the Merkle Proof
- 17eedb89e621a7e29742498c31a0cdd4e0a8d94ae2a8de9ae834dd4f3559909b 
- f4e8bbacc532685d0991e1a6220630a1168f410ad9efcd607ec2f2987791afce
- 684c86919301215987294c876063c4bd76ffdc7516bc94e499c005fc420cc889
- 6450d8d53fb8c2aa593df6fdbeab5a61b4c86054ba1c18f485134d1d260064b3
- d254e436d788da47283e475937074f685b8ebf5f00f0984b0f8f7ef40e64c9df

To extract all this information from a single string ```parse_set()``` function defined in ```server.c``` in used.

The length of the whole string is safely less than ```270*70``` or ```18900``` because we are using sha256 as the hash function and there can be a total of only ```2^256``` sha256 hashes each of length ```64``` in hexadecimal format. So here we get ```256*64```. Also old and new leaf hash as well as old and new root hash are there in the string therefore the exact length can be considered to be ```260*64```. Since there are spaces and integers which will be less than or equal to ```2^256``` since they are size of Merkle tree, index which is but obvious less than size of Merkle Tree and size of proof which is log of size of Merkle Tree.
So it is totally safe if we consider ```18900``` as the maximum length of string though this is also much greater than the actual string can ever be.

Since each element of the string is a standard ASCII value therefore each string element is 1 byte. So the maximum size of string that is sent to verifier is 18900 bytes or approximately ```18.5 kB```.



