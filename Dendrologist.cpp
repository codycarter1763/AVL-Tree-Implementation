/**
 * A testbed for an augmented implementation of an AVL tree
 * @author William Duncan, Cody Carter
 * @see AVLTree.h
 * <pre>
 * File: Dendrologist.cpp
 * Date: 10/18/2023
 * CSC 3102 Programming Project # 2
 * Instructor: Dr. Duncan 
 * </pre>
 */
#include <iostream>
#include <cstdlib>  
#include <stdexcept>
#include <iomanip>
#include <fstream> 
#include <algorithm>
#include <vector>
#include "AVLTree.cpp"

using namespace std;

int main(int argc, char** argv) 
{
    string usage = "Dendrologist <order-code> <command-file>\n";
    usage += "  <order-code>:\n";
    usage += "  0 ordered by increasing string length, primary key, and reverse lexicographical order, secondary key\n";
    usage += "  -1 for reverse lexicographical order\n";
    usage += "  1 for lexicographical order\n";
    usage += "  -2 ordered by decreasing string length\n";
    usage += "  2 ordered by increasing string length\n";
    usage += "  -3 ordered by decreasing string length, primary key, and reverse lexicographical order, secondary key\n";
    usage += "  3 ordered by increasing string length, primary key, and lexicographical order, secondary key\n";  
    if (argc != 3)
    {
        cout<<usage<<endl;
        throw invalid_argument("There should be 3 command line arguments.");
    }
    
    int sortCode = stoi(argv[1]);
    string filename = argv[2];
    AVLTree<string> Tree;
function<int(string, string)> compare;
switch (sortCode) {
        case -3:
            compare = [](string s1, string s2) {
                int length = s2.length() - s1.length();
                if (length == 0) {
                    return s2.compare(s1);
                }
                return length;
            };
            Tree = AVLTree<string>(compare);
            break;
        case -2:
            compare = [](string s1, string s2) {
                return s2.length() - s1.length();
            };
            Tree = AVLTree<string>(compare);
            break;
        case -1:
            compare = [](string s1, string s2) {
                return s2.compare(s1);
            };
            Tree = AVLTree<string>(compare);
            break;
        case 0:
            compare = [](string s1, string s2) {
                int length = s1.length() - s2.length();
                if (length == 0) {
                    return s2.compare(s1);
                }
                return length;
            };
            Tree = AVLTree<string>(compare);
            break;
        case 1:
            compare = [](string s1, string s2) {
                return s1.compare(s2);
            };
            Tree = AVLTree<string>(compare);
            break;
        case 2:
            compare = [](string s1, string s2) {
                return s1.length() - s2.length();
            };
            Tree = AVLTree<string>(compare);
            break;
        case 3:
            compare = [](string s1, string s2) {
                int length = s1.length() - s2.length();
                if (length == 0) {
                    return s1.compare(s2);
                }
                return length;
            };
            Tree = AVLTree<string>(compare);
            break;
        default:
            cout << "Invalid sortcode" << endl;
            break;
    }


ifstream txtFile(filename);
string line;

while (getline(txtFile, line))
{
    istringstream iss(line);
    string command;
    iss >> command;

    if (command == "insert") 
    {
        string parameter;
        iss >> parameter;
        cout<<"Inserted "<<parameter<<endl;
        Tree.insert(parameter);
    } 
    else if (command == "delete") 
    {
        string parameter;
        iss >> parameter;
        cout<<"Deleted "<<parameter<<endl;
        Tree.remove(parameter);
    } 
    else if (command == "traverse")
    {
        string parameter;
        iss >> parameter;
        cout<<"Pre-Order Traversal "<<endl;
            Tree.preorderTraverse([](auto& data)
                {
                    cout << data <<endl;
                });
    
        cout<<"In-Order Traversal "<<endl;
            Tree.traverse([](auto& data)
                {      
                    cout << data <<endl;
                });
   
        cout<<"Post-Order Traversal "<<endl;
            Tree.postorderTraverse([](auto& data) 
                {
                    cout << data <<endl;
                });
    } 
    
    else if (command == "gen")
    {
        int ancestors;
        string parent;
        string parameter;
        iss >> parameter;

        cout<<"Geneology = ";
        if (Tree.inTree(parameter) == false)
        {
            cout<<parameter<<" UNDEFINED"<<endl;
        }
        
        else
        {
            cout<<parameter<<endl;
            if(Tree.getParent(parameter)==nullptr)
        {
            parent="NULL";
        }
        
        else 
        {
            parent=*Tree.getParent(parameter);
        }
        
        cout<<"Parent = "<<parent<<", ";
       
    vector<string*> children = Tree.getChildren(parameter);
    
    // Access the elements in the vector
    cout<<"Left Child: "; 
    if (children[0] != nullptr)
    {
        cout<<*children[0];
    } 
    else 
    {
        cout << "None";
    }
    
    cout<<", Right Child: ";
    if (children[1] != nullptr) 
    {
        cout<<*children[1];
    } 
    else 
    {
        cout << "None";
    }
    cout<<endl;

        ancestors=Tree.ancestors(parameter);
        cout<<"#ancestors = "<<ancestors;
        
        int descendant;
        descendant=Tree.descendants(parameter);
        cout<<", #descendants="<<descendant<<endl;

    }} 
    else if (command == "props") 
    {
        cout<<"Properties:"<<endl;
        cout<<"Size = "<<Tree.size()<<", Height = "<<Tree.height()<<", Diameter = "<<Tree.diameter()<<endl;
        cout<<"Fibonnaci? = ";
        if(Tree.isFibonacci() == 1)
        {
            cout<<"True";
        }
        
        else
        {
            cout<<"False";
        }
        
        cout<<", Complete? = ";
        if(Tree.isComplete() == 1)
        {
            cout<<"True";
        }
        
        else
    
        {
            cout<<"False";
        }
        cout<<endl;

    } 
    else 
    {
        cerr << "Unknown command: " << command << endl;
    }
    }

    return 0;
}
