# pylint: disable=line-too-long
import pytest
from Prover.merkle import Node, MerkleTree

#variable LEAF_ELEMENTS stores the elements that are leaves in the Merkle Tree
LEAF_ELEMENTS = [b'merkle', b'tree', b'library',
                 b'hardware wallet', b'python',
                 b'cryptography', b'hashing funcitons',
                 b'sha256']

#OBJect of MerkleTree class has been created as OBJ
OBJ = MerkleTree(LEAF_ELEMENTS)

#root stores the current root hash of the Merkle Tree whoose leaves are in list variable transactions
root = OBJ.root

#list variable LEAF_HASHES stores the sha256 digest of the leaves of the Merkle Tree
LEAF_HASHES=[   b'\x8f&\xd9HqnYd\xfc\xdf}B\\\xdd\x95\xd6\xa2u\xa572\x19~\xfe\x1d\xd9v\xa1\x1a\x0b\xe6x',
                b'f\x01\xef\x14\x03\x82\xce\xb3\xeaVE\xb3\xbc\x8d{:\xf8\xcd\x1f\x1f\x98\xb7-\x85\x88\x90\xb5 ;\xbf\x9e.',
                b'\x01f\xa0\xddW\xac\xdf\xe4\x9c\xa3\xf6\xbc\xf2\xe3\xf9\xc1\xe0L@\xa1\xdaP\xddZ\xa99\xf2*#\x8a\xe8(',
                b'\x18k\xf1G\xcd\x8e\x81\xda\xa4\xea\x1e\xe3s\xcf+\xcd|\x15\x10\x7f\xb2\xd8|!Ya!\xc0\xf5lz\xbf',
                b'Lm\xe7)v\xf5,\xd9\x8c\xa61B\x9a0\xdbrR\xfa\xeb\x97\xc3\xc3\xeb2\xb4d\xaa\x12\x9e#\xf1\xda',
                b'\x9a^\x8c\x93L\xb9\x86\xb2w\xde\xe2`\xbb\xac\xd2\xd6\x88S\x0bT\x93\xce\xb8j4W\xce\xa7=\x02:\xd8',
                b'Cm\xa2B\xf5\xb9\x95z\x0c\xfc\xa8%\xb0d\xe0\xf8\xb7\x90n\xafX\x8d5)\x13\xb0\x9b\xfd\x85\x1cf\xc7',
                b'\xe0\x12\xe0\x154%f\x0b\xe3\xfd|O\xffI\xb2\tJ\x88:\xbd\x8b\x05\xea\x8d\x18\xa1E\xcb\xd7\xed\xccg']


@pytest.mark.Node
def test_node():
        """
        test_Node() tests the class Node
        of merkle.py which creates and
        maintains the nodes of Merkle Tree
        
        """

        res = True

        """
        n1 to n8 store the leaf nodes
        of the Merkle Tree.
        Their left node, right node and
        parent is unknown during initialisation

        """

        n1 = Node(None, None, None, LEAF_HASHES[0])
        n2 = Node(None, None, None, LEAF_HASHES[1])
        n3 = Node(None, None, None, LEAF_HASHES[2])
        n4 = Node(None, None, None, LEAF_HASHES[3])
        n5 = Node(None, None, None, LEAF_HASHES[4])
        n6 = Node(None, None, None, LEAF_HASHES[5])
        n7 = Node(None, None, None, LEAF_HASHES[6])
        n8 = Node(None, None, None, LEAF_HASHES[7])


        """
        n12 to n78 store the nodes formed
        from combining the leaf nodes
        Their value  and parent is
        unknown during initialisation.
        
        Their value is computed using the recompute_value()
        method of the Node class.

        """

        n12 = Node(n1, n2, None, None)
        n12.recompute_value()
        n34 = Node(n3, n4, None, None)
        n34.recompute_value()
        n56 = Node(n5, n6, None, None)
        n56.recompute_value()
        n78 = Node(n7, n8, None, None)
        n78.recompute_value()

        """
        n1234 and n5678 store the nodes formed
        from combining the nodes n12 to n78
        Their value  and parent is
        unknown during initialisation.
        
        Their value is computed using the recompute_value()
        method of the Node class.

        """


        n1234 = Node(n12, n34, None, None)
        n1234.recompute_value()
        n5678 = Node(n56, n78, None, None)
        n5678.recompute_value()

        """
        The root node is finally obtained by 
        combining the nodes n1234 and n5678
        and the value is computed using the
        recompute_value() method of Node class.

        Since this is the root node therefore
        its parent is set to None.
        
        """

        root_node = Node(n1234, n5678, None, None)
        root_node.recompute_value()
        root_hash = root_node.value

        if(root_hash == root):
            print("test-passed")
        else:
            print("test-failed")
            res = False
        
        assert res is True
