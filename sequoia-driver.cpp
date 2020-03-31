#include "sequoia.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;

const string INPUT_FILE("input.txt");
const string OUTPUT_FILE("output.txt");

//Checks whether the height of this node and all of its
// descendents are calculated correctly
//Returns the height of this node if correct
//Throws a runtime_error if incorrect
int SequoiaNode::checkHeight() const
{
  int lht = 0, rht = 0;
  if (left != nullptr)
    lht = left->checkHeight();
  if (right != nullptr)
    rht = right->checkHeight();
  if (lht > rht && height != lht + 1)
  {
    cout << "Height of node " << value << " incorrect.\n"
         << "Left height = " << lht << '\n'
         << "Right height = " << rht << '\n'
         << "Node height = " << height << endl;
    throw runtime_error("Bad height");
  }
  return height;
}

//Returns true if the height is calculated correctly
// in every node in this tree
bool Sequoia::checkHeight() const
{
  try
  {
    root->checkHeight();
    return true;
  }
  catch (runtime_error& ex)
  {
    return false;
  }
}

//Returns whether the tree rooted at this SequoiaNode is tall
//Assumes height is calculated correctly
bool SequoiaNode::isTall() const
{
  int lht = 0, rht = 0;
  if (left != nullptr)
  {
    lht = left->height;
    if (!left->isTall())
      return false;
  }
  if (right != nullptr)
  {
    rht = right->height;
    if (!right->isTall())
      return false;
  }
  if (lht >= 2*rht || rht >= 2*lht)
    return true;
  else
  {
    cout << "Node " << value << " is not tall:\n"
         << "Left height = " << lht << '\n'
         << "Right height = " << rht << '\n';
    return false;
  }
}

//Returns whether a Sequoia is tall
//Assumes height is calculated correctly
bool Sequoia::isTall() const
{
  if (root == nullptr)
    return true;
  else
    return root->isTall();
}

//Print Sequoia to given ostream
ostream& operator<<(ostream& out, const Sequoia& tree)
{
  if (tree.root == nullptr)
    return out << "empty";
  else
    return out << tree.root;
}

//Print the tree rooted at this SequoiaNode to out
ostream& operator<<(ostream& out, const SequoiaNode* node)
{
  out << '(' << node->value;
  if (node->left != nullptr)
    out << " L:" << node->left;
  if (node->right != nullptr)
    out << " R:" << node->right;
  return out << ')';
}

int main()
{
  vector<int> ins, del;
  ifstream in(INPUT_FILE);
  ofstream out(OUTPUT_FILE);
  string line;
  istringstream iss;
  int temp;
  Sequoia seq;

  if (!in.is_open())
  {
    cout << "Error opening " << INPUT_FILE << endl;
    return 1;
  }
  if (!out.is_open())
  {
    cout << "Error opening " << OUTPUT_FILE << endl;
    return 1;
  }

  //Read first line of input.txt into ins
  getline(in, line);
  iss.str(line);
  while (iss >> temp)
    ins.push_back(temp);
  iss.clear();

  //Read second line of input.txt into del
  getline(in, line);
  iss.str(line);
  while (iss >> temp)
    del.push_back(temp);
  in.close();

  //Insert all values of ins into seq
  //Check for problems in tree
  for (int i : ins)
  {
    seq.insert(i);
    if (!seq.checkHeight() || !seq.isTall())
    {
      cout << "Sequoia invalid after adding " << i
           << ":  " << seq << endl;
      return 1;
    }
  }
  //cout << seq << endl;
  out << seq << endl;

  //Remove all values in del from seq
  //Check for problems in tree
  //Assume everything in del is in tree
  for (int x : del)
  {
    seq.remove(x);
    if (!seq.checkHeight() || !seq.isTall())
    {
      cout << "Sequoia invalid after removing " << x
           << ":  " << seq << endl;
      return 1;
    }
  }
  //cout << seq << endl;
  out << seq << endl;
  out.close();

  return 0;
}