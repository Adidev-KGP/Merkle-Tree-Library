# Verifier

The role of verifier is to verify Merkle Proofs.

Verifier are usually such systems which do not have much memory to store data. Such systems rely on powerful external machines to store data.Only a specific data maybe of interest to the verifier at a certain time and the verifier can obtain that data from those external machines. But what if the external machine sends the wrong information. To solve this issue Merkle Tree is used. The external machines/client sends the data and merkle proof to the verifier. The verifier combines the hash of data with the Merkle Proof and compares the final output. If the final output is equal to the root hash of Merkle Tree then the verifier is sure that it has been sent the correct data.

How does exactly the verifier combine the data hash and merkle proofs?
Read further.

Consider the following Merkle Tree.

                root(h1234567)
                     / \
                h1234   h567
                /   \     /  \
              h12  h34   h56  \
              / \   / \  / \   \
             h1 h2 h3 h4 h5 h6 h7

To know how Merkle Tree is computed refer to 
#### [prover/README.md](https://github.com/Adidev-KGP/Merkle-Tree-Library/tree/main/prover)

We will see how the Merkle Proof are used for elements in the left branch as well in the right branch

## Element in the left branch(O(logN))

lets consider the index 2 or h3

Merkle Proof of h3 = {h4,  h12, h567}

index = 2

Now for h3 we need to think that we should combine h3 and h4 in  which order(h3, h4) OR (h4, h3)

lets us add 1 to index , so index is 3. 

3 is odd therefore (h3, h4) order is correct.

combine{(h3, h4)} = h34

divide index with 2 and ceil it, therfore index is now ceil(3/2) = 2

so index = 2

now 2 is even so (h12, h34) ordrer is valid because for even index we combine the obtained element hash from right and from left if index is odd.

combine{(h12, h34)} = h1234

index = ceil(2/2); index = 1

index is odd therefore h1234 will be combined from left 

combine{(h1234, h567)} = h1234567

So finally we got our correct root hash h1234567

The process described is implemented in the function ```verify_merkle_proof_leaves_power_of_2()```

So this process gives us the correct direction of merging data hash and merkle proofs to get the correct root hash. It works fine if the data is either in the left branch or the number of leaves in the Mekrle Tree is a power of 2. If you find it hard to understand try it with pen and paper and draw few samples and then all the explantion will make sense.


## Element in the right branch(O(2^logN) in worst case)

lets consider index 5 or h6

Merkle Proof of h6={h5, h7, h1234}

lets describe a function com(char dir, int index, leaf, proof) which takes a charater parameter ```'r'``` OR ```'l'``` stating the direction right/left from which to combine

lets start with left direction OR ```'l'```
```
                            com('l', 0, h6, proof)
                            leaf= combine(h6, h5)=h65
                                /          \
                               /            \
                              /              \
                com('l', 1, h65, proof)      com('r', 1, h65, proof)
                leaf=combine(h65,h7)=h657    leaf=combine(h7,h65)=h765
                   /                \
                  /                  \
                 /                    \
com('l', 2, h657, proof)    com('r',2,h657,proof)
    leaf=h6571234
    /
   /
com('l',3,h6571234,proof)
3==proof_size, so return 1
if root == leaf else return 0

```
as we can see that the function call started with ```'l'``` and at the first step of Recursion Tree the leaf is inconsistent(it should have been h56 but its h65) and so it is gauranteed that this pattern of combination with leaf will lead to wrong hash in the further recursive calls.

lets start with right direction OR ```'r'```

```
                            com('r', 0, h6, proof)
                            leaf= combine(h5, h6)=h56
                                /          \
                               /            \
                              /              \
                com('l', 1, h56, proof)      com('r', 1, h56, proof)
                leaf=combine(h56,h7)=h567    leaf=combine(h7,h56)=h756
                   /                \
                  /                  \
                 /                    \
com('l', 2, h567, proof)    com('r',2,h567,proof)
    leaf=h5671234              leaf=h1234567
    /           \                /               \
   /                        com('l',3,h1234567)   \
com('l',3,h5671234,proof)    return 1              \
3==proof_size, so return 1
if root == leaf else return 0
This branch returns 0

```

as we can see that the function call started with ```'r'``` and at the first step of Recursion Tree the leaf is consistent(it should have been h56 and it got calculated to h56 as well) and so it is possible that this pattern of combination with leaf may lead to correct hash in the further recursive calls. This is what exactly happens as the function reaches the last recursive call.

So we can see that if the data is in the right branch and the number of leaves in the Merkle tree is not a power of 2 then we need to follow this recursive approach.

This may take O(logN) in best case and O(2^logN) in worst case.

This recursion approach is implemented in ```verify_merkle_right_branch()``` in ```verifier.c```

So we use the ```verify_merkle_proof_leaves_power_of_2()``` if the leaf is in the left branch or the number of leaves in the Merkle Tree is a power of 2 regardless of the position of any leaf.

We use ```verify_merkle_right_branch()``` if the leaf is in the right branch and the number of leaves in the Merkle Tree is not a power of 2.

Since we know that left branch has ${2^k}$ leaves such that k is the largest integer possible for ${2^k}$ < N , it can be concluded that

- the sure probability of merkle proof verification to be O(log(N)) is ${2^k}$/N where if N is a power of 2 then the probability becomes 1.
- the probability of merkle proof verification to be O(${2 ^l}$) can be less than or equal to 1-${2^k}$/N.
where l = ${log{_2}{n}}$

Inside ```verify_merkle_proof()``` both these functions are called depending on the situation and position of data in the Merkle Tree.