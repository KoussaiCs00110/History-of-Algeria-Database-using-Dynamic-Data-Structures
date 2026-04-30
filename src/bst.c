#include "../include/bst.h"
#include "../include/data_structures.h"
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

/*TStack* addNameBST(TTree *tr, char *name, char *DoB, char *DoD): this function adds a word with dates of 
birth and death into a sorted tree*/
TStack* addNameBST(TTree *tr, char *name, char *DoB, char *DoD){
    if(tr == NULL) return NULL;
    if(strcmp(name, tr->name) == 0){
        TStack *newNode = (TStack*)malloc(sizeof(TStack));
        strcpy(newNode->name, tr->name);
        strcpy(newNode->definition, tr->definition);
        strcpy(newNode->DoB, tr->DoB);
        strcpy(newNode->DoD, tr->DoD);
        strcpy(newNode->event_date, tr->event_date);
        newNode->next = NULL;
        return newNode;
    }
    else if(strcmp(name, tr->name) < 0){
        return addNameBST(tr->left, name, DoB, DoD);
    }
    else{
        return addNameBST(tr->right, name, DoB, DoD);
    }
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

static void inorder_r (TTree *n) { if (!n) return; inorder_r(n->left); trav_append_node(n); inorder_r(n->right); }
static void preorder_r(TTree *n) { if (!n) return; trav_append_node(n); preorder_r(n->left); preorder_r(n->right); }
static void postorder_r(TTree *n){ if (!n) return; postorder_r(n->left); postorder_r(n->right); trav_append_node(n); }

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
