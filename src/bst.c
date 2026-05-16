#include "../include/bst.h"
#include "../include/data_structures.h"
#include <math.h>
TStack *trav_stack = NULL;

static void copy_bst_field(char *dest, const char *src){
    if(src == NULL){
        dest[0] = '\0';
        return;
    }
    strcpy(dest, src);
}

TTree* insertToBST(TTree *tr, char *name, char *definition, char *DoB, char *DoD, char *event_date){
    if(name == NULL) return tr;

    if(tr == NULL){
        TTree *newNode = (TTree*)malloc(sizeof(TTree));
        if(newNode == NULL) return NULL;

        copy_bst_field(newNode->name, name);
        copy_bst_field(newNode->definition, definition);
        copy_bst_field(newNode->DoB, DoB);
        copy_bst_field(newNode->DoD, DoD);
        copy_bst_field(newNode->event_date, event_date);
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    int cmp = strcmp(name, tr->name);
    if(cmp < 0){
        tr->left = insertToBST(tr->left, name, definition, DoB, DoD, event_date);
    }
    else if(cmp > 0){
        tr->right = insertToBST(tr->right, name, definition, DoB, DoD, event_date);
    }
    else{
        copy_bst_field(tr->definition, definition);
        copy_bst_field(tr->DoB, DoB);
        copy_bst_field(tr->DoD, DoD);
        copy_bst_field(tr->event_date, event_date);
    }

    return tr;
}
TTree* toTree(TStack *stk){

    if(stk == NULL) return NULL;
    TTree *root = NULL;
    TStack *current = stk;
    while(current != NULL){
        root = insertToBST(root, current->name, current->definition, current->DoB, current->DoD, current->event_date);
        current = current->next;
    }
    return root;
}

TTree* fillTree(FILE *f){
    TTree *root = NULL;
    char name[100], definition[500], DoB[20], DoD[20], event_date[20];
    while(fscanf(f, "%99[^,],%499[^,],%19[^,],%19[^,],%19[^\n]\n", name, definition, DoB, DoD, event_date) == 5){
        root = insertToBST(root, name, definition, DoB, DoD, event_date);
    }
    return root;
}
/**TTree* getInfoNameTree(TTree *tr, char *name): this function takes a name as input and returns the definition 
and dates from the tree. */
TTree* getInfoNameTree(TTree *tr, char *name){
    if(tr == NULL) return NULL;
    if(strcmp(name, tr->name) == 0){
        return tr;
    }
    else if(strcmp(name, tr->name) < 0){
        return getInfoNameTree(tr->left, name);
    }
    else{
        return getInfoNameTree(tr->right, name);
    }
}

/*TTree* addNameBST(TTree *tr, char *name, char *DoB, char *DoD): this function adds a word with dates of 
birth and death into a sorted tree*/
TTree* addNameBST(TTree *tr, char *name, char *DoB, char *DoD){
    if(tr == NULL){
        TTree *newNode = malloc(sizeof(TTree));
        strcpy(newNode->name, name);
        strcpy(newNode->DoB, DoB);
        strcpy(newNode->DoD, DoD);
        strcpy(newNode->definition, "");
        strcpy(newNode->event_date, "");
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if(strcmp(name, tr->name) < 0)
        tr->left = addNameBST(tr->left, name, DoB, DoD);
    else if(strcmp(name, tr->name) > 0)
        tr->right = addNameBST(tr->right, name, DoB, DoD);

    return tr;
}
/**TTree* deleteNameBST(TTree *tr, char *name): this function deletes a name from the tree*/
TTree* deleteNameBST(TTree *tr, char *name){
    if(tr == NULL) return NULL;
    if(strcmp(name, tr->name) == 0){
        if(tr->left == NULL){
            TTree *temp = tr->right;
            free(tr);
            return temp;
        }
        else if(tr->right == NULL){
            TTree *temp = tr->left;
            free(tr);
            return temp;
        }
        else{
            TTree *temp = tr->right;
            while(temp->left != NULL){
                temp = temp->left;
            }
            strcpy(tr->name, temp->name);
            strcpy(tr->definition, temp->definition);
            strcpy(tr->DoB, temp->DoB);
            strcpy(tr->DoD, temp->DoD);
            strcpy(tr->event_date, temp->event_date);
            tr->right = deleteNameBST(tr->right, temp->name);
        }
    }
    else if(strcmp(name, tr->name) < 0){
        tr->left = deleteNameBST(tr->left, name);
    }
    else{
        tr->right = deleteNameBST(tr->right, name);
    }
    return tr;
}

/*TTree* updateNameBST(TTree *tr, char *name, char *s, char *DoB, char *DoD): this function updates a 
personality tree.*/
TTree* updateNameBST(TTree *tr, char *name, char *s, char *DoB, char *DoD){
    if(tr == NULL) return NULL;
    if(strcmp(name, tr->name) == 0){
        strcpy(tr->definition, s);
        strcpy(tr->DoB, DoB);
        strcpy(tr->DoD, DoD);
    }
    else if(strcmp(name, tr->name) < 0){
        tr->left = updateNameBST(tr->left, name, s, DoB, DoD);
    }
    else{
        tr->right = updateNameBST(tr->right, name, s, DoB, DoD);
    }
    return tr;
}
//trav_append_node: this function appends a node to the stack.
static void trav_append_node(TTree *tr){
    if(tr == NULL) return;
    TStack *newNode = (TStack*)malloc(sizeof(TStack));
    strcpy(newNode->name, tr->name);
    strcpy(newNode->definition, tr->definition);
    strcpy(newNode->DoB, tr->DoB);
    strcpy(newNode->DoD, tr->DoD);
    strcpy(newNode->event_date, tr->event_date);
    newNode->next = NULL;
    if(trav_stack == NULL){
        trav_stack = newNode;
    }
    else{
        TStack *current = trav_stack;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = newNode;
    }
}

static void free_traversal_stack(TStack *stk){
    while(stk != NULL){
        TStack *next = stk->next;
        free(stk);
        stk = next;
    }
}

static void inorder_r (TTree *n) { if (!n) return; inorder_r(n->left); trav_append_node(n); inorder_r(n->right); }
static void preorder_r(TTree *n) { if (!n) return; trav_append_node(n); preorder_r(n->left); preorder_r(n->right); }
static void postorder_r(TTree *n){ if (!n) return; postorder_r(n->left); postorder_r(n->right); trav_append_node(n); }

static TTree* traversalBST(TTree *tr, void (*traverse)(TTree *)){
    free_traversal_stack(trav_stack);
    trav_stack = NULL;

    traverse(tr);

    TStack *nodes = trav_stack;
    TTree *result = toTree(nodes);

    trav_stack = NULL;
    free_traversal_stack(nodes);
    return result;
}

TTree* traversalBSTinOrder(TTree *tr){
    return traversalBST(tr, inorder_r);
}

TTree* traversalBSTpreOrder(TTree *tr){
    return traversalBST(tr, preorder_r);
}

TTree* traversalBSTpostOrder(TTree *tr){
    return traversalBST(tr, postorder_r);
}

/*void heightSizeBST(TTree *tr): this procedure returns the height and size of the tree. */
void heightSizeBST(TTree *tr, int *height, int *size){
    if(tr == NULL){
        *height = 0;
        *size = 0;
        return;
    }
    int leftHeight, rightHeight, leftSize, rightSize;
    heightSizeBST(tr->left, &leftHeight, &leftSize);
    heightSizeBST(tr->right, &rightHeight, &rightSize);
    *height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    *size = leftSize + rightSize + 1;
}

/*TTree* lowestCommonAncestor(TTree *tr, char *word1, char *word2): this function returns the lowest 
common ancestor of two nodes (words).*///dont use strcmp
TTree* lowestCommonAncestor(TTree *tr, char *word1, char *word2){
    if(tr == NULL) return NULL;
    if(strcmp(word1, tr->name) < 0 && strcmp(word2, tr->name) < 0){
        return lowestCommonAncestor(tr->left, word1, word2);
    }
    else if(strcmp(word1, tr->name) > 0 && strcmp(word2, tr->name) > 0){
        return lowestCommonAncestor(tr->right, word1, word2);
    }
    else{
        return tr;
    }
}

/*int countNodesRange(TTree *tr, int l, int h): this function counts the number of nodes that lie within a given 
range [l, h]. 
*/
int countNodesRange(TTree *tr, char *l, char *h){
    if(tr == NULL) return 0;
    if(strcmp(tr->name, l) >= 0 && strcmp(tr->name, h) <= 0){
        return 1 + countNodesRange(tr->left, l, h) + countNodesRange(tr->right, l, h);
    }
    else if(strcmp(tr->name, l) < 0){
        return countNodesRange(tr->right, l, h);
    }
    else{
        return countNodesRange(tr->left, l, h);
    }
}

/*TTree* inOrderSuccessor(TTree *tr, char *word): this function returns the in-order successor of a given node in 
the tree. */
TTree* inOrderSuccessor(TTree *tr, char *word){
    if(tr == NULL) return NULL;
    TTree *current = getInfoNameTree(tr , word);
    if(current == NULL) return NULL;
    if(current->right != NULL){
        TTree *temp = current->right;
        while(temp ->left != NULL){
            temp = temp->left;
        }
        return temp;
    }    
    else{
        TTree *successor = NULL;
        TTree *ancestor = tr;
        while(ancestor != current){
            if(strcmp(current->name , ancestor->name) < 0){
                successor = ancestor;
                ancestor = ancestor->left;    
            }else{
                ancestor = ancestor->right;
            }

        }
        return successor;
    }
}

//TTree* BSTMirror(TTree *tr): this function returns a mirror of the tree.
TTree* BSTMirror(TTree *tr){
    if(tr ==NULL)return NULL;
    TTree *mirror = (TTree*)malloc(sizeof(TTree));
    strcpy(mirror->name, tr->name);
    strcpy(mirror->definition, tr->definition);
    strcpy(mirror->DoB, tr->DoB);
    strcpy(mirror->DoD, tr->DoD);
    strcpy(mirror->event_date, tr->event_date);
    mirror->left = BSTMirror(tr->right);
    mirror->right = BSTMirror(tr->left);
    return mirror;
}
//bool isBalancedBST(TTree *tr): this function checks if a given tree is balanced.
static int height(TTree *tr){
    if(tr == NULL) return 0;

    int lh = height(tr->left);
    int rh = height(tr->right);

    return (lh > rh ? lh : rh) + 1;
}

static bool isBalanced(TTree *tr){
    if(tr == NULL) return true;
    int lh = height(tr->left);
    int rh = height(tr->right);
    if(abs(lh - rh) > 1) return false;
    return isBalanced(tr->left) && isBalanced(tr->right); 
}

static bool isBst(TTree *tr){
    if(tr == NULL) return true;
    if(tr ->left != NULL && strcmp(tr ->left->name, tr->name)>=0) return false;
    if(tr ->right != NULL && strcmp(tr ->right->name, tr->name)<=0) return false;
    return isBst(tr->left) && isBst(tr->right);
}
bool isBalancedBST(TTree *tr){
   return isBalanced(tr) && isBst(tr);
}

//TTree* BSTMerge(TTree *tr1, TTree *tr2): this function merges two trees into a single BST.
static TTree* mergeIntoTree(TTree *dest, TTree *src){
    if(src == NULL) return dest;

    dest = mergeIntoTree(dest, src->left);
    dest = insertToBST(dest, src->name, src->definition, src->DoB, src->DoD, src->event_date);
    dest = mergeIntoTree(dest, src->right);

    return dest;
}

TTree* BSTMerge(TTree *tr1, TTree *tr2){
    TTree *mergedTree = NULL;

    mergedTree = mergeIntoTree(mergedTree, tr1);
    mergedTree = mergeIntoTree(mergedTree, tr2);

    return mergedTree;
}
