typedef enum
{AVL_SUCCESS,AVL_ALLOCATION_ERROR,AVL_INVALID_INPUT,AVL_FAILURE,AVL_NOT_EXIST}
        AVL_RES;

template<class KeyType, class ItemType>
class AVL_TREE {
public:
    template<class KeyType1, class ItemType1>
    class AVLNode {
    public:
        AVLNode(KeyType key, ItemType* item) :
                Balance(0), Depth(0),
                m_Key(key), m_Data(item), x_tre_count(0), father_node(0),
                left_son(0), right_son(0),remb(0) {}


        ~AVLNode()=default;

        int Balance;
        int Depth;

        KeyType m_Key;
        ItemType* m_Data;
        int x_tre_count;
        AVLNode* father_node;
        AVLNode* left_son;
        AVLNode* right_son;
		int remb;
    };

    AVL_TREE() : Root(0){ }
    ~AVL_TREE() {
        DestroyRecursive(Root);
        //delete m_pRoot;
        Root = nullptr;
    }
    void    cleara(AVLNode<KeyType, ItemType>* node);
    void 	putNodeInMyTree(int x_tree,KeyType key, ItemType* item,int x);
    void 	RemoveInTree(KeyType key,int x);
    ItemType* FindInTree(KeyType key,int x);
    void internalInOrder(AVLNode<KeyType, ItemType>* root, int* artists, int* songs, int* counter, int* current, KeyType artist);

    AVLNode<KeyType, ItemType>* Root;

    int	GetDepth() const {
        return (Root ? Root->m_Depth : 0);
    }
    void aux_putNode(int x_tree,KeyType key, ItemType* item, AVLNode<KeyType, ItemType>*& root,int x);

protected:
    AVLNode<KeyType,ItemType>* leafRemoveInTree( AVLNode<KeyType,ItemType>* node, int x);
    AVLNode<KeyType,ItemType>* sonRemoveInTreeSingle( AVLNode<KeyType,ItemType>* node, int x);
    void flipNodesInTree( AVLNode<KeyType,ItemType>* v, AVLNode<KeyType,ItemType>* w, int x);
    void DestroyRecursive(AVLNode<KeyType, ItemType>* node);
    AVL_RES DeleteInTree(AVLNode<KeyType, ItemType>* node,int x);
    ItemType* _Find_aux(KeyType key, AVLNode<KeyType, ItemType>* root,int x);
    void ComputeBF(AVLNode<KeyType, ItemType>* root,int x);
    void Balance(AVLNode<KeyType, ItemType>*& root,int x);
    void BalanceRightSubTree(AVLNode<KeyType, ItemType>*& root,int x);
    void BalanceLeftSubTree(AVLNode<KeyType, ItemType>*& root,int x);
    void LeftSonRotation(AVLNode<KeyType, ItemType>*& root,int x);
    void RightSonRotation(AVLNode<KeyType, ItemType>*& root,int x);
};
template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::cleara(AVLNode<KeyType, ItemType>* node){
    DestroyRecursive(node);
    return;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::DestroyRecursive(AVLNode<KeyType, ItemType>* node) {
    if (node != nullptr)
    {
        if (node->left_son) {
            DestroyRecursive(node->left_son);
        }
        node->left_son=nullptr;

        if (node->right_son) {
            DestroyRecursive(node->right_son);
        }
        node->right_son=nullptr;
        if(node->m_Data){ delete node->m_Data;}

        node->m_Data=nullptr;

        if(node==Root){
            if(node->m_Data){ delete node->m_Data;}
            delete node;

            Root= nullptr;

            return;
        }

        delete node;


    }
    return;
}
/*--------------------------------------------------------------------------------------------
									   insertion  Done
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::putNodeInMyTree(int x_tree,KeyType key, ItemType* item,int x)
{
    if (Root == 0)
    {
        Root = new AVLNode<KeyType, ItemType>(key, item);
        Root->x_tre_count=x_tree;
    }
    else {
        aux_putNode(x_tree,key, item, Root,0);
    }

	Root->remb = x + 8;
    return;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::aux_putNode(int x_tree,KeyType key,  ItemType* item, AVLNode<KeyType, ItemType>*& root,int x)
{
    if (key < root->m_Key)
    {
        if (root->left_son) {
			aux_putNode(0,key, item, root->left_son,0);
        }
        else {
            root->left_son = new AVLNode<KeyType, ItemType>(key, item);
            root->left_son->father_node=root;
            root->left_son->x_tre_count=x_tree;
        }
    }
    else if (key > root->m_Key)
    {
        if (root->right_son) {
			aux_putNode(0,key, item, root->right_son,0);
        }
        else {
            root->right_son = new AVLNode<KeyType, ItemType>(key, item);
            root->right_son->father_node=root;
            root->right_son->x_tre_count=x_tree;
        }
    }
    else {
        // error - can't have duplicate keys.
        // if duplicate keys are okay, change key < to key <= above
    }
   // ComputeBF(root);
 //   Balance(root);
 if(Root){
     Root->remb = x + 8;
 }

}

/*--------------------------------------------------------------------------------------------
										 removal  Done
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::RemoveInTree(KeyType key,int x)
{
  /*  if(!FindInTree(key,0))
    {
        return ;
    }*/
    AVLNode<KeyType,ItemType>* ptr=this->Root;
    while(ptr!= nullptr)
    {
        if (key==ptr->m_Key)
        {
            DeleteInTree(ptr,0);
            return;
        }
        else if( key > ptr->m_Key){
            ptr = ptr->right_son;
        }
        else{
            ptr = ptr->left_son;
        }
    }
    if(Root){Root->remb = x+8;}

    return  ;
}

template<class KeyType, class ItemType>
AVL_RES AVL_TREE<KeyType, ItemType>::DeleteInTree(AVL_TREE<KeyType, ItemType>::AVLNode<KeyType, ItemType> *node,int x)  {
    AVLNode<KeyType, ItemType>* start_point = nullptr;
    if (node->right_son == nullptr && node->left_son == nullptr) {
        start_point = leafRemoveInTree(node,0);
    }
    else if (node->right_son != nullptr && node->left_son!= nullptr) {
        AVLNode<KeyType, ItemType> *ptr = node->right_son;
        while (ptr->left_son != nullptr) {
            ptr = ptr->left_son;
        }
        flipNodesInTree(node, ptr,0);
        if (node->right_son == nullptr && node->left_son == nullptr) {
            start_point = leafRemoveInTree(node,0);
        } else {
            start_point = sonRemoveInTreeSingle(node,0);
        }
    }
    else {
        start_point = sonRemoveInTreeSingle(node,0);
    }

    if(Root == nullptr){
        return AVL_SUCCESS;
    }

    if (start_point != Root)
    {

        return AVL_SUCCESS;
    }
    else{

    }
    if(Root){
        Root->remb = x + 7;
    }

    return AVL_SUCCESS;
}

template<class KeyType, class ItemType>
AVL_TREE<KeyType,ItemType>::AVLNode<KeyType,ItemType>* AVL_TREE<KeyType,ItemType>::leafRemoveInTree(AVL_TREE::AVLNode<KeyType, ItemType> *node,int x)  {
    AVLNode<KeyType,ItemType>*  start_point= nullptr;
    if (node==Root)
    {
        delete node->m_Data;
        delete node;
        Root = nullptr;
        start_point = nullptr;
        return start_point;
    }

    AVLNode<KeyType,ItemType>* father = node->father_node;
    start_point = father;
    if (father->right_son == node) {
        father->right_son = nullptr;
        if(node->m_Data){  delete node->m_Data;}


        delete node;
        return start_point;
    } else {
        father->left_son = nullptr;
        delete node->m_Data;
        delete node;
        start_point = father;
        return start_point;
    }
    if(Root){

        Root->remb = x + 8;
    }

    return start_point;
}

template<class KeyType, class ItemType>
AVL_TREE<KeyType,ItemType>::AVLNode<KeyType,ItemType>* AVL_TREE<KeyType,ItemType>::sonRemoveInTreeSingle(
        AVL_TREE<KeyType, ItemType>::AVLNode<KeyType, ItemType> *node, int x)  {
    AVLNode<KeyType,ItemType>* start_point = nullptr;
    if(node->right_son != nullptr && node->left_son == nullptr)
    {
        if(node==Root)
        {
             Root=node->right_son;
            delete node->m_Data;
            delete node;
            Root->father_node= nullptr;
            start_point = Root;
            return start_point;
        }
        else
        {
            // if node had one right_son son
            start_point = node->father_node;
            if(node->father_node->left_son == node)
            {
                node->father_node->left_son=node->right_son;
                node->right_son->father_node=node->father_node;
                delete node->m_Data;
                delete node;
                return start_point;
            }
            else{
                node->father_node->right_son=node->right_son;
                node->right_son->father_node=node->father_node;
                delete node->m_Data;
                delete node;
                return  start_point;
            }
        }
    }
    if(node->right_son == nullptr && node->left_son != nullptr)
    {
        if(node==Root)
        {
            Root=node->left_son;
            delete node->m_Data;
            delete node;
            Root->father_node= nullptr;
            start_point = Root;
            return start_point;
        }
        else {
            // if node had one right_son son
            start_point = node->father_node;
            if (node->father_node->m_Key > node->m_Key) {
                node->father_node->left_son = node->left_son;
                node->left_son->father_node = node->father_node;
                delete node->m_Data;
                delete node;
                return start_point;
            } else {
                node->father_node->right_son = node->left_son;
                node->left_son->father_node = node->father_node;
                delete node->m_Data;
                delete node;
                return start_point;
            }
        }
    }
    if(Root){
        Root->remb = x + 8;
    }

    return start_point;
}


template<class KeyType, class ItemType>
void AVL_TREE<KeyType,ItemType>::flipNodesInTree( AVLNode<KeyType, ItemType> *v,  AVLNode<KeyType, ItemType> *w, int x) {
    AVLNode<KeyType, ItemType>* father_nodeV=v->father_node;
    AVLNode<KeyType, ItemType>* father_nodeW=w->father_node;
    AVLNode<KeyType, ItemType>* leftV=v->left_son;
    AVLNode<KeyType, ItemType>* right_sonW = w->right_son;
    AVLNode<KeyType, ItemType>* leftW = w->left_son;

    if(v->right_son == w){

        w->right_son=v;
        w->left_son=leftV;
        w->father_node=father_nodeV;
        v->father_node=w;
        v->left_son=leftW;
        if (father_nodeV!= nullptr) {
            if (father_nodeV->m_Key > w->m_Key) {
                father_nodeV->left_son = w;
            } else {
                father_nodeV->right_son = w;
            }
        }
        leftV->father_node=w;

        if(w->right_son == nullptr){
            v->right_son= nullptr;
            v->left_son= nullptr;
        }
        else{
            v->right_son = right_sonW;
            if(right_sonW != nullptr){
                right_sonW->father_node = v;
            }
        }
    }

        //v.right_son != w
    else {
        leftV->father_node = w;
        w->left_son = leftV;
        w->father_node = father_nodeV;
        if (father_nodeV!= nullptr) {
            if (father_nodeV->m_Key > w->m_Key) {
                father_nodeV->left_son = w;
            } else {
                father_nodeV->right_son = w;
            }
        }
        w->right_son=v->right_son;
        v->right_son->father_node=w;
        v->left_son= nullptr;
        v->right_son=right_sonW;
        v->father_node=father_nodeW;
        if (right_sonW!= nullptr)
        {
            right_sonW->father_node=v;
        }
        if (father_nodeW->m_Key > v->m_Key) {
            father_nodeW->left_son = v;
        } else {
            father_nodeW->right_son = v;
        }

    }
    if( v ==Root){
      Root = w;
    }
    if(Root){
        Root->remb = x + 8;
    }

}

/*--------------------------------------------------------------------------------------------
									  searching  Done
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
ItemType* AVL_TREE<KeyType, ItemType>::FindInTree(KeyType key,int x)
{

    if(Root){Root->remb = x;}


    return _Find_aux(key, Root,0);
}

template<class KeyType, class ItemType>
ItemType* AVL_TREE<KeyType, ItemType>::_Find_aux(KeyType key, AVLNode<KeyType, ItemType>* root,int x)
{
    if (root)

    {
        if (root->m_Key == key)
        {
            ItemType* res(nullptr);
            res = root->m_Data;
            return res;
        }

        if (key < root->m_Key)
            return _Find_aux(key, root->left_son,0);
        else
            return _Find_aux(key, root->right_son,0);
    }
    else
    {
        return nullptr;
    }
    if(Root){Root->remb = x+4;}

}


/*--------------------------------------------------------------------------------------------
								 balancing     Done 
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::ComputeBF(AVLNode<KeyType, ItemType>* root,int x)
{
    if (root)
    {
        int leftDepth = root->left_son ? root->left_son->Depth : 0;
        int right_sonDepth = root->m_pright_son ? root->right_son->Depth : 0;

        root->Depth = 1 + ((leftDepth > right_sonDepth) ? leftDepth : right_sonDepth);
        root->Balance = right_sonDepth - leftDepth;
    }
	Root->remb = x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::Balance(AVLNode<KeyType, ItemType>*& root,int x)
{
    // AVL trees have the property that no branch is more than 1 longer than its sibling

    if (root->m_Balance > 1)
        BalanceRightSubTree(root, 1);

    if (root->m_Balance < -1)
        BalanceLeftSubTree(root, 1);
	Root->remb = x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::BalanceRightSubTree(AVLNode<KeyType, ItemType>*& root,int x)
{
    if (root->m_pright_son)
    {
        if (root->m_pright_son->m_Balance > 0)//RR rotation
        {
            LeftSonRotation(root, 1);
        }
        else if (root->m_pright_son->m_Balance < 0)//RL rotation
        {
            RightSonRotation(root->m_pright_son, 1);
            LeftSonRotation(root, 1);
        }
    }
	Root->remb = x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::BalanceLeftSubTree(AVLNode<KeyType, ItemType>*& root,int x)
{
    if (root->left_son)
    {
        if (root->left_son->m_Balance < 0)//LL rotation
        {
            RightSonRotation(root, 1);
        }
        else if (root->left_son->m_Balance > 0)//LR rotation
        {
            LeftSonRotation(root->left_son, 1);
            RightSonRotation(root, 1);
        }
    }
	Root->remb = x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::LeftSonRotation(AVLNode<KeyType, ItemType>*& root,int x)//RR basic rotation
{
    AVLNode<KeyType, ItemType>* pTemp = root;
    if (root && root->right_son) {
        root = root->right_son;
        pTemp->right_son = root->left_son;

        root->left_son = pTemp;

        ComputeBF(root->left_son, 1);
        ComputeBF(root->right_son, 1);
        ComputeBF(root, 1);
    }
	Root->remb = x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::RightSonRotation(AVLNode<KeyType, ItemType>*& root,int x)//LL basic rotation
{
    AVLNode<KeyType, ItemType>* pTemp = root;
    root = root->left_son;
    pTemp->left_son = root->right_son;

    root->m_pright_son = pTemp;
    root->m_pright_son=pTemp;

    ComputeBF(root->left_son, 1);
    ComputeBF(root->right_son, 1);
    ComputeBF(root,1);
	Root->remb = x;
}
/*--------------------------------------------------------------------------------------------
								  inorder iteration   V  Done
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::internalInOrder(AVLNode<KeyType, ItemType>* root, int* artists, int* songs, int* counter, int* current, KeyType artist) {
    if (root == nullptr) {
        return;
    }
    internalInOrder(root->left_son, artists, songs, counter, current, artist);
    if (*counter > 0) {
        artists[*current] = artist;
        songs[*current] = root->m_Key;
        *counter=*counter-1;
        *current=*current+1;
    }
    internalInOrder(root->right_son, artists, songs, counter, current, artist);
}
