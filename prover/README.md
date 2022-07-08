# Prover

The role of the prover is to store data and hash them to store and produce Merkle Tree and Merkle Proof corresponding to an index.

## Hashing of elements:

To hash an element convert it into bytes. Any element can be represented by a particular number of bytes so it can be said that the element can be represented by an array of bytes. In python we have the concept of byte strings to store elements of different types into bytes where each element of byte string is a byte having a value in range ```0``` to ```255```.

This byte array is the input to hash functions which returns the hash or digest of that particular element.

We are given an ordered list of inputs D[n] = {d(0), d(1), ..., d(n-1)}, where each element d(i) is a byte string. We denote with || the concatenation operator, and with D[a:b] the list {d(a), d(a+1), ..., d(b - 1)}.

We define the Merkle Tree Hash (MTH) (also called the Merkle root) as follows.

The hash of the empty list is MTH({}) = 0, a string of 32 bytes identically equal to 0.

The hash of a 1-element list (leaf node) is:

```MTH({d(0)}) = SHA-256(0x00 || d(0))```

For ```n > 1```, let k be the largest power of 2 strictly smaller than n. Then the hash of a list D[n] (internal node) is:

```MTH(D[n]) = SHA-256(0x01 || MTH(D[0:k]) || MTH(D[k:n]))```

## Merkle Proof
 
The Merkle proof (called Merkle audit path in the language for RFC 6962) for a leaf node is the minimal set of additional nodes that is necessary to compute the Merkle Tree Hash.

## Proof of correct update

It is a list containing the following in the given sequence:
- old root hash
- old leaf hash
- new root hash
- merkle proof elements

Verifer has the new leaf element which is to replace the old leaf.

Using old leaf hash and Merkle Proof the verifier confirms the old root hash and using the new leaf hash and Merkle Proof the verifier confirms the new root hash. If the old and new root hash are verified then the proof of correct update is reliable and the prover has succesfully replaced the old leaf with the new leaf.

### Remark
Since the verifier is instructing to replace a partucular index therefore the merkle proof for that index is independent of the value at that index. That's why the proof of correct update contains a single merkle proof.


## Properties of Prover:
- it can replace a particular leaf with another value on instructed by the verifier and return proof of correct update( set() method of MerkleTree class)
- it can prduce Merkle Proof corresponding to a particular leaf index
- it maintains a copy of the Merkle Tree which can be returned by it
- it can return leaf hash corresponding to a particular index
- it can return the current root hash of the Merkle Tree.