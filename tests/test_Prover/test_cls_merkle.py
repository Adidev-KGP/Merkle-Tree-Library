# pylint: disable=missing-module-docstring
from typing import List
import math
import pytest
from pyparsing import Char

from prover.merkle import MerkleTree, combine_hashes, largest_power_of_2_less_than
import colors
# The list variable TRANSACTIONS contains some Bitcoin TRANSACTIONS
TRANSACTIONS = [
    b"0f228b6937809585b1d6608d8602bbf81d69d2df1c8d454083ac1197eb3699c8 -> 1AmATEZYYg8dnwBLBJGNyP4EihDzrzJDzK 0.9 BTC",
    b"0f228b6937809585b1d6608d8602bbf81d69d2df1c8d454083ac1197eb3699c8 -> 13fjLwsAJC7KsH3wPhA3vbLLcSBfBMb6r5 0.1 BTC",
    b"cdb64c957b14fa3495ad97f38431bb26d2ae948bb6420bc9a71fc3c39f3563c1 -> 1FkdushTJEDJHXatTssvzHFfGvBdBH2zdj 0.45 BTC",
    b"dfb4198557f06c6dc45dec1ae000fc440eca695ec596ad255a8dc71545974b71 -> 13hfJ4iymeSL26bQFv1ToznYorXhJa5rqN 0.05 BTC",
    b"be499505b3f0eeb6aba76b45cb0a78e4404ad9e8d7c77c40ba12d30f3ce1b385 -> 1BPtLG8TPtB4Y16GqQA6M5YVwXp2KKXv9n 0.5 BTC",
    b"f372541efb1fbeffa52880dc3e0ccc60ab17e9955c1e0fee7947c577d567d73e -> 18AP4vf78pgrzYUP22Cv1mdqoS95nGLWPH 0.05 BTC",
    b"247b7f266f9e712fa16343bf4019d2c012fa1f8ce71fea4b4fed51845d69f2a1 -> 1J8Swxzfs7FoXkhvSyKrmgDgemLFr4iKNR 1.32 BTC",
    b"f4d490210aba8fcccccdcd62686d51754cc075a3ca651abc1b0360343b506c27 -> 1CK6KHY6MHgYvmRQ4PAafKYDrg1ejbH1cE 6.38373204 BTC",
    b"f4d490210aba8fcccccdcd62686d51754cc075a3ca651abc1b0360343b506c27 -> d-8b95330a9d1cda76625e7ecf1ca34c06 OP_Returned",
    b"f4d490210aba8fcccccdcd62686d51754cc075a3ca651abc1b0360343b506c27 -> d-c2d4abee4d0a19966bd87bc3fe6ee6ea OP_Returned",
    b"457b7f567f9e712fa16343gf4019d2c012cd1f8ce71fea4b4fed51845d69f2a1 -> 5g8Rwxzfs7BoYkovSyKrmgpmemkgr4iKNR 1.47 BTC",
    b"0f228b6937809585b1d6608d8602bbf81d69d2df1c8d454083ac1197eb3699c8 -> 1AmATEZYYg8dnwBLBJGNyP4EihDzrzJDzK 0.97 BTC",
    b"0f228b6937809585b1d6608d8602bbf81d69d2df1c8d454083ac1197eb3699c8 -> 13fjLwsAJC7KsH3wPhA3vbLLcSBfBMb6r5 0.14 BTC",
    b"cdb64c957b14fa3495ad97f38431bb26d2ae948bb6420bc9a71fc3c39f3563c1 -> 1FkdushTJEDJHXatTssvzHFfGvBdBH2zdj 0.452 BTC",
    b"dfb4198557f06c6dc45dec1ae000fc440eca695ec596ad255a8dc71545974b71 -> 13hfJ4iymeSL26bQFv1ToznYorXhJa5rqN 0.053 BTC",
    b"be499505b3f0eeb6aba76b45cb0a78e4404ad9e8d7c77c40ba12d30f3ce1b385 -> 1BPtLG8TPtB4Y16GqQA6M5YVwXp2KKXv9n 0.58 BTC",
    b"f372541efb1fbeffa52880dc3e0ccc60ab17e9955c1e0fee7947c577d567d73e -> 18AP4vf78pgrzYUP22Cv1mdqoS95nGLWPH 0.05 BTC",
    b"247b7f266f9e712fa16343bf4019d2c012fa1f8ce71fea4b4fed51845d69f2a1 -> 1J8Swxzfs7FoXkhvSyKrmgDgemLFr4iKNR 1.342 BTC",
    b"0f228b6937809585b1d6608d8602bbf81d69d2df1c8d454083ac1197eb3699c8 -> 1AmATEZYYg8dnwBLBJGNyP4EihDzrzJDzK 0.9 BTC",
    b"0f228b6937809585b1d6608d8602bbf81d69d2df1c8d454083ac1197eb3699c8 -> 13fjLwsAJC7KsH3wPhA3vbLLcSBfBMb6r5 0.1 BTC",
    b"cdb64c957b14fa3495ad97f38431bb26d2ae948bb6420bc9a71fc3c39f3563c1 -> 1FkdushTJEDJHXatTssvzHFfGvBdBH2zdj 0.458 BTC",
    b"dfb4198557f06c6dc45dec1ae000fc440eca695ec596ad255a8dc71545974b71 -> 13hfJ4iymeSL26bQFv1ToznYorXhJa5rqN 0.053 BTC",
    b"be499505b3f0eeb6aba76b45cb0a78e4404ad9e8d7c77c40ba12d30f3ce1b385 -> 1BPtLG8TPtB4Y16GqQA6M5YVwXp2KKXv9n 0.5 BTC",
    b"f372541efb1fbeffa52880dc3e0ccc60ab17e9955c1e0fee7947c577d567d73e -> 18AP4vf78pgrzYUP22Cv1mdqoS95nGLWPH 0.051 BTC",
    b"935ghtf266f9e712fa16343bf4019d2c012fa1f8ce71fea4b4fed51845d69f2a1 -> 1J8Swxzfs7FoXkhvSyKrmgDgemLFr4iKNR 1.38 BTC",
]

#object of MerkleTree class has been created as OBJ
OBJ = MerkleTree(TRANSACTIONS)

#root stores the current root hash of the Merkle Tree whoose leaves
#are in list variable TRANSACTIONS
ROOT = OBJ.root

"""
Methods to test from MerkleTree class:
1)set()
2)prove_leaf()

"""


def verify_merkle_proof_leaves_power_of_2(index: int, leaf: bytes, proof: List[bytes]):

    """
    Given a Merkle Tree having number leaves equal to some power of 2,
    verify_merkle_proof_leaves_power_of_2() computes the root hash of such
    a Merkle Tree from the Merkle Proof and the leaf hash and returns
    the final root hash it calculated.
    Time complexity : O(log2(N)) where N is the number of leaves.
    """

    _len = OBJ.__len__()

    index += 1

    if index != _len:
        for i in proof:
            if index % 2 == 0:
                ver_root = combine_hashes(i, leaf)
                leaf = ver_root
            else:
                ver_root = combine_hashes(leaf, i)
                leaf = ver_root
            index = math.ceil(index/2)

    elif index == _len:
        for i in proof:
            ver_root = combine_hashes(i, leaf)
            leaf = ver_root
    return ver_root


def ver_merkle_right_branch(index: int, proof: List[bytes], leaf: bytes, dir: Char):

    """
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

    """


    if index == len(proof):
        if leaf == OBJ.root:
            return OBJ.root
        else:
            return None

    if dir == 'l':
        leaf = combine_hashes(leaf, proof[index])
    elif dir == 'r':
        leaf = combine_hashes(proof[index], leaf)

    _x = ver_merkle_right_branch(index+1, proof, leaf, 'l')
    if _x == OBJ.root:
        return _x

    _y = ver_merkle_right_branch(index+1, proof, leaf, 'r')
    if _y == OBJ.root:
        return _y


def verify_merkle_proof(index: int, leaf: bytes, proof: List[bytes]):

    """
    verify_merkle_proof uses a combination of 2 functions to compute root hash:
    1)verify_merkle_proof_leaves_power_of_2():
        this function is used if the Merkle Tree has number of leaves equal
        to some power of 2 or if the leaf is in the left branch of Merkle Tree.

    2)ver_merkle_right_branch():
        this function is used when the number of leaves in Merkle Tree
        is not a power of 2 and the leaf is in the right branch of Merkle Tree.

    """

    #variable left stores the the largest power of 2 less than number of leaves
    left =  largest_power_of_2_less_than(OBJ.__len__())

    if left*2 == OBJ.__len__():
        """executes if the number of leaves is a power of 2"""

        return verify_merkle_proof_leaves_power_of_2(index, leaf, proof)
    else:
        """
        executes if the number of leaves is not a power of 2 
        and the leaf is in the right branch
        """
        if index +1 <= left:
            return verify_merkle_proof_leaves_power_of_2(index, leaf, proof)
        else:
            if index+1 == OBJ.__len__():
                return ver_merkle_right_branch(0, proof, leaf, 'r')

            if (index+1) %2 == 0 and index+1 != OBJ.__len__():
                return ver_merkle_right_branch(0, proof, leaf, 'r')
            elif (index+1) %2 != 0 and index+1 != OBJ.__len__():
                return ver_merkle_right_branch(0, proof, leaf, 'l')


########################################## Test Methods have been defined below ############################################

@pytest.mark.set
def test_set():
    """
    test_set() is the function to test the set method of
    MerkleTree class

    """

    #Setting transaction of leaf 3 as set_transaction
    set_transaction = b"2c487160381f60116ee86ac042c9d6e74cc525cf9fba0b89a1b477fe85d93817 -> 1MkCDCzHpBsYQivp8MxjY5AkTGG1f2baoe 6.26708335 BTC"
    proof_update = OBJ.set(3, set_transaction)

    new_root = proof_update[2]
    new_leaf = OBJ.get(3)

    prev_root = proof_update[0]
    prev_leaf = proof_update[1]

    res = True

    if (new_root == verify_merkle_proof(3, new_leaf, proof_update[3:]) and
            prev_root == verify_merkle_proof(3, prev_leaf, proof_update[3:])):

        print("test-passed")

    else:
        res = False
        print("test-failed")

    assert res is True

@pytest.mark.merkle_proof
def test_merkle_proofs():
    """
    test_merkle_proofs() is the function to test the
    correctness of the Merkle Proof produced by the
    prove_leaf method of class MerkleTree

    """

    res = True

    for i in range(0, OBJ.__len__()):

        if ROOT == verify_merkle_proof(i, OBJ.get(i), OBJ.prove_leaf(i)):
            print(f"test no. {i+1} passed")
        else:
            res = False
            print(f"test no. {i+1} {colors.RED}failed{colors.RESET}")

    assert res is True
