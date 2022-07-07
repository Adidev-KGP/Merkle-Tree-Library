# pylint: disable=line-too-long
import pytest
from prover.merkle import MerkleTree, Node, make_tree

#variable LEAF_ELEMENTS stores the elements that are leaves in the Merkle Tree
LEAF_ELEMENTS = [b'merkle', b'tree', b'library',
                 b'hardware wallet', b'python',
                 b'cryptography', b'hashing funcitons',
                 b'sha256']

#object of MerkleTree class has been created
OBJ = MerkleTree(LEAF_ELEMENTS)

#variable root stores the current root hash of the Merkle Tree
ROOT = OBJ.root

#list variable LEAF_HASHES stores the sha256 digest of the elements in LEAF_ELEMENTS
LEAF_HASHES = [b'\x8f&\xd9HqnYd\xfc\xdf}B\\\xdd\x95\xd6\xa2u\xa572\x19~\xfe\x1d\xd9v\xa1\x1a\x0b\xe6x',
               b'f\x01\xef\x14\x03\x82\xce\xb3\xeaVE\xb3\xbc\x8d{:\xf8\xcd\x1f\x1f\x98\xb7-\x85\x88\x90\xb5 ;\xbf\x9e.',
               b'\x01f\xa0\xddW\xac\xdf\xe4\x9c\xa3\xf6\xbc\xf2\xe3\xf9\xc1\xe0L@\xa1\xdaP\xddZ\xa99\xf2*#\x8a\xe8(',
               b'\x18k\xf1G\xcd\x8e\x81\xda\xa4\xea\x1e\xe3s\xcf+\xcd|\x15\x10\x7f\xb2\xd8|!Ya!\xc0\xf5lz\xbf',
               b'Lm\xe7)v\xf5,\xd9\x8c\xa61B\x9a0\xdbrR\xfa\xeb\x97\xc3\xc3\xeb2\xb4d\xaa\x12\x9e#\xf1\xda',
               b'\x9a^\x8c\x93L\xb9\x86\xb2w\xde\xe2`\xbb\xac\xd2\xd6\x88S\x0bT\x93\xce\xb8j4W\xce\xa7=\x02:\xd8',
               b'Cm\xa2B\xf5\xb9\x95z\x0c\xfc\xa8%\xb0d\xe0\xf8\xb7\x90n\xafX\x8d5)\x13\xb0\x9b\xfd\x85\x1cf\xc7',
               b'\xe0\x12\xe0\x154%f\x0b\xe3\xfd|O\xffI\xb2\tJ\x88:\xbd\x8b\x05\xea\x8d\x18\xa1E\xcb\xd7\xed\xccg']

@pytest.mark.make_tree
def test_make_tree():

    """
    test_make_tree() tests the make_tree() function
    which builds the Merkle Tree and returns its root hash
    """

    _c = 0
    nodes = []
    res = True

    for i in range(0, 8):
        nodes.append(Node(None, None, None, LEAF_HASHES[_c]))
        _c += 1

    root1 = make_tree(nodes, 0, 8).value

    if root1 == ROOT:
        print("test-passed")
    else:
        print("test-failed")
        res = False

    assert res is True
