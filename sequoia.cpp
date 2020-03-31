#include "sequoia.h"

//Returns a pointer to the first node in this subtree containing x
//Returns null pointer if it can't find x
SequoiaNode* SequoiaNode::search(int x)
{
  if (x == value)
  {
      return this;
  }
  else if (x < value && left != nullptr)
  {
    return left->search(x);
  }
  else if (x > value && right != nullptr)
  {
    return right->search(x);
  }
  else
  {
     return nullptr;  
  }
}

//Inserts a new node into this Sequoia
//Updates root if necessary
//Does *not* update height or ensure tallness
void Sequoia::insert(int x)
{
  if (root != nullptr)
  {
    root->insert(x);
  }
  else
  {
    root = new SequoiaNode(x);
  }
  size++;

  while (root->parent != nullptr)
  {
    root = root->parent;
  }
}

//Removes a node from this Sequoia
//Updates root if necessary
//No effect if the tree doesn't contain the given value
//Does *not* update height or enforce tallness
void Sequoia::remove(int x)
{
  if (root == nullptr)
  {
    return;
  }
  
  SequoiaNode* victim = root->search(x);

  if (victim == nullptr)
  {
    return;
  }
  else
  {
    if (victim == root)

      if (root->left != nullptr)
      {
        root = root->left;
      }
      else
      {
        root = root->right;
      }
      
    victim = victim->remove();

    delete victim;
  }
}

//Basic BST insertion function
//Does *not* update height or ensure tallness
void SequoiaNode::insert(int x)
{
  bool bIns = false,bInsLeft;

  if (x < value)
  {
    if (left != nullptr)
    {
       left->insert(x);
    }
    else
    {
      left = new SequoiaNode(x);

      left->parent = this;

      bIns = true,bInsLeft = true;
    }
  }
  else
  {
    if (right != nullptr)
    {
      right->insert(x);
    }
    else
    {
      right = new SequoiaNode(x);

      right->parent = this;

      bIns = true, bInsLeft = false;
    }
  }

  // if the new node inserted in this call, update height and tallness
  if (bIns)
  {
    SequoiaNode* other = bInsLeft ? right : left;

    if (other == nullptr)

      updateHeight();

    fixBalanceInsert();
  }
}

void SequoiaNode::updateHeight()
{
  // traverses tree up from the current node
  for (SequoiaNode* current = this; current != nullptr; current = current->parent)
  {
    int leftH = current->left->getHeight(), rightH = current->right->getHeight();

    // sets height to max of left subheight and right subheight
    current->height = leftH;

    if (current->height < rightH)

      current->height = rightH;

    // plus 1
    current->height++;
  }
}

//Basic BST removal function
//Does *not* update height or ensure tallness
//Returns pointer to the node removed (2 child case)
//Sets children to null but does not delete node
//(see Sequoia::remove(x))
SequoiaNode* SequoiaNode::remove()
{
  bool bDel = true;

  if (left == nullptr && right == nullptr)
  {
    if (parent != nullptr)
      if (parent->left == this)
      {
         parent->left = nullptr;
      }
      else
      {
        parent->right = nullptr;
      }
  }
  else if (left != nullptr && right == nullptr)
  {
    left->parent = parent;

    if (parent->left == this)
    {
       parent->left = left;
    }
    else
    {
      parent->right = left;
    }
  }
  else if (left == nullptr && right != nullptr)
  {
    right->parent = parent;

    if (parent->left == this)
    {
      parent->left = right;
    }
    else
    {
      parent->right = right;
    }
  }
  else
  {
    bDel = false;

    SequoiaNode* swap = left;

    int temp = value;

    while (swap->right != nullptr)

      swap = swap->right;

      value = swap->value;

      swap->value = temp;

    return swap->remove();
  }

  left = right = nullptr;

  // if the node removed in this call, update height and tallness
  if (bDel)
  {
    parent->updateHeight();

    parent->fixBalanceRemove();
  }

  return this;
}

bool SequoiaNode::rotate()
{
  bool bParent = (parent != nullptr), bInsLeft = bParent && (parent->left == this);

  int leftH = left->getHeight(), rightH = right->getHeight();

  if (leftH >= 2 * rightH || rightH >= 2 * leftH)
  {
    return false; // good already
  }
  else // bad, will rotate
  {
    bool bCase1 = (leftH >= rightH);

    if (bCase1) // rotate to the left
    {
      if (bParent)
      {
        if (bInsLeft)
        {
          parent->left = right;
        }
        else
        {
          parent->right = right;
        }
      }

      SequoiaNode* rightLeft = right->left;

      right->parent = parent;

      right->left = this, parent = right;

      right = rightLeft;

      if (rightLeft != nullptr)
      {
        rightLeft->parent = this;
      }
    }
    else // rotate to the right
    {
      if (bParent)
      {
        if (bInsLeft)
        {
          parent->left = left;
        }
        else
        {
          parent->right = left;
        }
      }

      SequoiaNode* leftRight = left->right;

      left->parent = parent;

      left->right = this, parent = left;

      left = leftRight;

      if (leftRight != nullptr)
      {
        leftRight->parent = this;
      }
    }

    updateHeight();

    return true;
  }
}

// Function for updating the nodes of a Sequoia
// following an insertion to maintain "tallness"
// Not called in driver, so not required to be implemented
// If not implementing, must fix height and tallness elsewhere
void SequoiaNode::fixBalanceInsert()
{
  for (SequoiaNode* current = this; current != nullptr; current = current->parent)

    current->rotate();
}

// Function for adjusting the height of a Sequoia
// following a deletion to maintain "tallness"
// Not called in driver, so not required to be implemented
// If not implementing, must fix height and tallness elsewhere
void SequoiaNode::fixBalanceRemove()
{
  for (SequoiaNode* current = this; current != nullptr; current = current->parent)
  {
    if (current->rotate())
    {
      break; // only need to rotate once
    }
  }
}
