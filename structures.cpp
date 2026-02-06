#include<iostream>
#include<vector>
//for conv to lowercase
#include<string>
//for bfs: degrees of sepr
#include<queue>
#include<ctime>
//for inf
#include <climits>
//for tolower
#include <cctype>
//for max
#include<algorithm>
//for hashmap
#include<unordered_map>
using namespace std;
//some systemwide basic helpers
//tolower
string lower(string& a){
    string b = "";
    for(int i=0; i<a.size(); i++){
        b += tolower(static_cast<unsigned char>(a[i]));
    }
    return b;
}

//--------------------------------------------------------------------------------
//Post : Each Post
class post{
    private:
        //post properties
        string cont = "";
        time_t timecreated = 0;
        
    public:
        post(string content){
            timecreated = time(0);
            cont = content;
        }
        //returns the time of creation
        time_t get_time(){
            return timecreated;
        }
        //returns the content
        string get_content(){
            return cont;
        }      
};
//a node corr to each post:
struct pnode
{       
        post* corrpost;
        pnode* parent;
        pnode* lchild;
        pnode* rchild;
        int height = 0;
        int bal = 0;
        //methods:
        pnode( post* p){
            parent = nullptr;
            corrpost = p;
            lchild =rchild = nullptr;
        }
        //updates height
        void update_height(){
            if(lchild==nullptr and rchild==nullptr){
                height = 0;
            }else if(lchild==nullptr){
                height = rchild->height + 1;
            }else if(rchild==nullptr){
                height = lchild->height + 1;
            }else{
                height = 1+max(lchild->height,rchild->height);
            }
        }
        //updates balance
        void update_bal(){
            if(lchild== nullptr and rchild == nullptr){
                bal = 0;
            }else if(lchild==nullptr){
                bal = -1 - rchild->height;
            }else if(rchild==nullptr){
                bal = lchild->height + 1 ;
            }else{
                bal = lchild->height - rchild->height;
            }
        }
};
//A Users Post : An AVL Based Storage for all the users posts
class userposts{
    private:
        //root post
        pnode* root;
        int size= 0;
        //helper functions:
        //right rotate
        void rr(pnode* tor){
            //assuming left child of the elem to rotate (tor) exists; assuming only valid inputs for rotate will be given
            pnode* nroot = tor->lchild;
            
            if (nroot->rchild != nullptr)
            {
                nroot->rchild->parent = tor;
            }
            tor->lchild = nroot->rchild;
            nroot->rchild = tor;
            //updating the original parents at the end to avoid redundancies
            if(tor->parent != nullptr){
                if (tor->parent->lchild == tor)
                {
                    tor->parent->lchild = nroot;
                }else{
                    tor->parent->rchild = nroot;
                }   
            }else{
                root = nroot;
            }
            nroot->parent = tor->parent;
            tor->parent = nroot;
            nroot->rchild->update_height();
            nroot->update_height();
            nroot->rchild->update_bal();
            nroot->update_bal();
        }
        //left rotate
        void ll(pnode* tor){
            //assuming right child of the elem to rotate (tor) exists; assuming only valid inputs for rotate will be given
            pnode* nroot = tor->rchild;
            if (nroot->lchild != nullptr)
            {
                nroot->lchild->parent = tor;
            }
            tor->rchild = nroot->lchild;
            nroot->lchild = tor;
            //updating the original parents at the end to avoid redundancies
            if(tor->parent != nullptr){
                if (tor->parent->lchild == tor)
                {
                    tor->parent->lchild = nroot;
                }else{
                    tor->parent->rchild = nroot;
                }   
            }else{
                root = nroot;
            }
            nroot->parent = tor->parent;
            tor->parent = nroot;
            nroot->lchild->update_height();
            nroot->update_height();
            nroot->lchild->update_bal();
            nroot->update_bal();
        }
        //insert helper
        void inserthelper(pnode* post){
            size++;
            if(root == nullptr){
                //no root exists, first post;
                root = post;
                
            }else{
                pnode* curr = root;
                pnode* prev = root;
                // as it is storing by time, It is guaranteed that the newer post will be created later.
                while(curr->rchild!= nullptr){
                    curr = curr->rchild;
                }
                ///////VVIMP, read the comment in the next line.
                curr -> rchild = post;//might have to create a function to create a post for us to input into this fxn
                post->parent = curr;
                
            }
        }
        //inorder helper
        void traverse_helper(pnode* st, int n, int& curr){
            if(st == nullptr){

            }else{
                traverse_helper(st->rchild, n, curr);
                if(curr<n){
                    curr++;
                    cout<<st->corrpost->get_content()<<endl;
                    traverse_helper(st->lchild, n, curr);
                }else if(curr == n){
                    curr++;
                    cout<<st->corrpost->get_content()<<endl;
                }
            }
        }
    public:
        userposts(){
            root = nullptr;
        }
        //insert
        //will have to dynamically insert while creating in the interface file
        void insert(pnode* newpost){
            inserthelper(newpost);//updates the rel pointers as well
            //keep updating height and balance until you reach a imbalance or the root
            pnode* temp = newpost;
            while(temp!= nullptr and temp->bal <2 and temp->bal >-2){
                if(temp==root){
                    temp = temp->parent;
                    break;
                }else{
                    temp->parent->update_height();
                    temp->parent->update_bal();
                    temp = temp->parent;
                }
            }
            //if we find an imbalance
            if(temp!= nullptr and size!= 1){
                if(temp->bal > 0){
                    //left unbalanced
                    if(temp->lchild->bal > 0){
                        //left heavy
                        rr(temp);
                    }else{
                        //right heavy
                        ll(temp->lchild);
                        rr(temp);
                    }
                }else{
                    //right unbalanced
                    if(temp->rchild->bal > 0){
                        //left heavy
                        rr(temp->rchild);
                        ll(temp);
                    }else{
                        //right heavy
                        ll(temp);
                    }
                }
            }            
        }
        void output(int n){
            int curr = 1;
            if(size==0 and n!=0){
                cout<<"No Posts Yet";
            }
            if(n==-1){
                n = INT_MAX;
            }
            //doing nothing when n = 0, as good as a pass;
            traverse_helper(root,n,curr);
        }
        

};
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//Graph Node : Each User IN The Network
class User;
//Each Node in the AVL tree of friends.
struct UNode;

//The AVL Tree containing friends
class ftree{
    private:
        //root post
        UNode* root;
        int size= 0;
        //helper functions:
        void rr(UNode* tor);
        void ll(UNode* tor);
        void inserthelper(UNode* frd);
        void traverse_helper(UNode* st, vector<User*>& mittar);
    public:
        ftree(){
            root = nullptr;
        };
        void insert(UNode* newpost);
        vector<User*> listfriends();
};
//Proper Implementation of UNode
struct UNode{
    User* friend_id;
    UNode* lchild;
    UNode* rchild;
    UNode* parent;
    int height =0 ;
    int bal = 0;
    UNode(User* fr){
        parent = nullptr;
        //assuming initialization of a node cannot take place without a user attached to it
        friend_id = fr;
        lchild =rchild = nullptr;
    }
    void update_height(){
        if(lchild==nullptr and rchild==nullptr){
            height = 0;
        }else if(lchild==nullptr){
            height = rchild->height + 1;
        }else if(rchild==nullptr){
            height = lchild->height + 1;
        }else{
            height = 1+max(lchild->height,rchild->height);
        }
    }
    //updates balance
    void update_bal(){
        if(lchild== nullptr and rchild == nullptr){
            bal = 0;
        }else if(lchild==nullptr){
            bal = -1 - rchild->height;
        }else if(rchild==nullptr){
            bal = lchild->height + 1 ;
        }else{
            bal = lchild->height - rchild->height;
        }
    }
};      
//Proper Implemetation of User
class User{
    private:
        string username;
        ftree friends;
        userposts myposts;
        unordered_map<User*,int> friendlist;
    public:
        User(string usr){
            username = usr;
        }
        //returns username
        string get_username(){
            return username;
        }
       
        //lists friends alphabetically
        void list_friends(){
            vector<User*> temp = friends.listfriends();
            for(auto i : temp){
                cout<<i->get_username()<<endl;
            }
        }
        // returns the friendlist 
        vector<User*> ret_friend_list(){
            return friends.listfriends();
        }
        void output_posts(int n){
            myposts.output(n);
        }
        void create_post(string cont){
            post* temp = new post(cont);
            myposts.insert(new pnode(temp));
        }
         //adds friends (only to this user, not to its counterpart: taken care by well the network questions)
        int add_friend(User* newfr){
            //making sure it already doesn't exist:
            if(friendlist.count(newfr)==0){
                friends.insert(new UNode(newfr));
                friendlist[newfr] = 1;
                return 0;
            }else{
                return 1;
            }
        }

};

//-----------------------------------------------------------------------------------
//Ftree Methods executed;
void ftree::rr(UNode* tor){
    UNode* nroot = tor->lchild;
    //assuming left child of the elem to rotate (tor) exists; assuming only valid inputs for rotate will be given
    
    if (nroot->rchild != nullptr)
    {
        nroot->rchild->parent = tor;
    }
    tor->lchild = nroot->rchild;
    nroot->rchild = tor;
    //updating the original parents at the end to avoid redundancies
    if(tor->parent != nullptr){
        if (tor->parent->lchild == tor)
        {
            tor->parent->lchild = nroot;
        }else{
            tor->parent->rchild = nroot;
        }   
    }else{
        root = nroot;
        }
        nroot->parent = tor->parent;
        tor->parent = nroot;
        nroot->rchild->update_height();
        nroot->update_height();
        nroot->rchild->update_bal();
        nroot->update_bal();
    }
//left rotate
void ftree::ll(UNode* tor){
    //assuming right child of the elem to rotate (tor) exists; assuming only valid inputs for rotate will be given
    UNode* nroot = tor->rchild;
    if (nroot->lchild != nullptr)
    {
        nroot->lchild->parent = tor;
    }
    tor->rchild = nroot->lchild;
    nroot->lchild = tor;
    //updating the original parents at the end to avoid redundancies
    if(tor->parent != nullptr){
        if (tor->parent->lchild == tor)
        {
            tor->parent->lchild = nroot;
        }else{
            tor->parent->rchild = nroot;
        }   
    }else{
        root = nroot;
    }
        
    nroot->parent = tor->parent;
    tor->parent = nroot;
    nroot->lchild->update_height();
    nroot->update_height();
    nroot->lchild->update_bal();
    nroot->update_bal();
}
//insert helper
void ftree::inserthelper(UNode* frd){
    if(root == nullptr){
        //no root exists, first post;
        root = frd;
        size++;
    }else{
        UNode* curr = root;
        UNode* prev = root;
        bool found = false;
        //insertion
        while(curr!= nullptr){
            prev = curr;
            if(curr->friend_id->get_username() > frd->friend_id->get_username()){
                curr = curr->lchild;
            }else{
                curr = curr->rchild;
            }
            
        }
        size++;
        if(!found){
            if(prev->friend_id->get_username() > frd->friend_id->get_username() ){
                prev->lchild = frd;
            }else{
                prev->rchild = frd;
            }
            frd->parent = prev;
        }
            
    }
}
//traversal helper
void ftree::traverse_helper(UNode* st, vector<User*>& mittar){
    if(st == nullptr){

    }else{
        traverse_helper(st->lchild, mittar);
        mittar.push_back(st->friend_id);
        traverse_helper(st->rchild, mittar);
    }
}

//insert
//will have to dynamically insert while creating in the interface file
void ftree::insert(UNode* newpost){
    inserthelper(newpost);//updates the rel pointers as well
    //keep updating height and balance until you reach a imbalance or the root
    UNode* temp = newpost;
    while(temp!= nullptr and temp->bal <2 and temp->bal >-2){
        if(temp != root){
            temp->parent->update_height();
            temp->parent->update_bal();
            temp = temp->parent;
        }else{
            temp = temp->parent;
            break;
        }
    }
    //if we find an imbalance
    if(temp!= nullptr and size!= 1){
        if(temp->bal > 0){
            //left unbalanced
            if(temp->lchild->bal > 0){
                //left heavy
                rr(temp);
            }else{
                //right heavy
                ll(temp->lchild);
                rr(temp);
            }
        }else{
            //right unbalanced
            if(temp->rchild->bal > 0){
                //left heavy
                rr(temp->rchild);
                ll(temp);
            }else{
                //right heavy
                ll(temp);
            }
        }
    }            
}
//lists friends
vector<User*> ftree::listfriends(){
    vector<User*> mittar;
    traverse_helper(root, mittar);
    return mittar;
}

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//Graph : The Network
// The Network/Graph containing all users; Adjacency List
//will have to work on case sensitivities;;;; ramlal vs add friend vagera to Ramlal;
class Network{
    private:
        //hashmap for storing users by their ids
        unordered_map<string, User*> userrs;
        //helper methods:
        User* ret_user(string usrnm){
            //for user chars, this would reduce redundancies
            if(userrs.count(lower(usrnm)) == 0){
                return nullptr;
            } /*else if(userrs[lower(usrnm)]->get_username() != usrnm){
                //same string but not the exact uname;
                return nullptr;
            }*/
            return userrs[lower(usrnm)];
        }
    public:
        //constructor
        Network(){

        }
        //insert user into the network
        void insert_user(string usrnam){
            //- hashing : username ->lowercase ->user
            string usrname = lower(usrnam);
            if(userrs.count(usrname)==0){
                //no such username exists
                userrs[usrname] = new User(usrnam);
            }else{
                //user already exists; 
                cout<<"Username Already Taken, Kindly Use Another Username"<<endl;
            }
        }
        //add friends (edge)
        void add_friend(string user1, string user2){
            if(ret_user(user1) == nullptr or ret_user(user2) == nullptr){
                //cannot do => error message
                cout<<"Either Of The Usernames Or Both Of Them Don't Exist"<<endl;
            }else if(lower(user1) == lower(user2)){
                cout<<"Please Enter 2 Different Usernames"<<endl;
            }else{
                int a = userrs[lower(user1)]->add_friend(userrs[lower(user2)]);
                int b = userrs[lower(user2)]->add_friend(userrs[lower(user1)]);
                if(a==1){
                    cout<<user1<<" and "<<user2<<" are already friends"<<endl;
                }
            }
        }
        //list friends
        void list_friendss(string usrnm){
            User* user = ret_user(usrnm);
            if(user == nullptr){
                cout<<"Please Enter A Correct Username"<<endl;
            }else{
                user->list_friends();
            }
        }
        //create post
        void createe(string usrnm, string cont){
            User* user = ret_user(usrnm);
            if(user == nullptr){
                cout<<"Please Enter A Correct Username"<<endl;
            }else{
                user->create_post(cont);
            }
        }
        //output posts
        void output_postss(string usm, int n){
            //the interface processing ensures that n will always be a non negative int
            User* user = ret_user(usm);
            if(user == nullptr){
                cout<<"Please Enter A Correct Username"<<endl;
            }else{
                user->output_posts(n);
            }            
        }
        //suggest friends ; change function type
        void suggest_friends(string usname, int n){
            User* user = ret_user(usname);
            if(user == nullptr){
                cout<<"Please Enter A Correct Username"<<endl;
            }else{
                // do a very controlled bfs;
                //all people who are to be recommended lie in the second level
                unordered_map<string, int> counter;
                unordered_map<string,int> explored;
                vector<pair<int, User*>> probables;
                queue<User*> cont;
                //first while loop : pushing the first children
                vector<User*> lvl1 = userrs[lower(usname)]->ret_friend_list();
                explored[lower(usname)] = 1;
                for(auto i : lvl1){
                    cont.push(i);
                    string tempr = i->get_username();
                    explored[lower(tempr)] = 1;
                }
                //we now have the list of immidiate friends; -> now let us loop over their friend and increase the count
                while(!cont.empty()){
                    string frusname = cont.front()->get_username();
                    vector<User*> temp = userrs[lower(frusname)]->ret_friend_list();
                    for(auto i : temp){
                        string temp2 = i->get_username();
                        if(explored.count(lower(temp2))==1){

                        }else{
                            if(counter.count(lower(temp2)) == 0){
                                counter[lower(temp2)] = 1;
                            }else{
                                counter[lower(temp2)] += 1;
                                //push into the avl by count or smt similar to make sure by count and then by alphabetical;
                            }
                        }
                    }
                    cont.pop();
                }
                for (const auto& pair : counter) {
                    probables.push_back({pair.second,userrs[pair.first]});
                }
                //sort probables;
                //Note : i'm using the inbuilt sort fxn
                sort(probables.begin(), probables.end(), [](const auto& a, const auto& b) {
                    if (a.first != b.first) {
                        //if count is diff
                        return a.first > b.first;
                    }
                    //owise add acc to usname
                    return a.second->get_username() < b.second->get_username(); 
                });
                int i = 0;
                while(i<probables.size() and i<n){
                    cout<<probables[i].second->get_username()<<" : have "<<probables[i].first<< " mutuals" <<endl;
                    i++;
                }
                if(n == 0){
                    cout<<" "<<endl;
                }
            
            }
        }
        
        //degrees of separation between 2 users
        int degrees_of_sepr(string u1, string u2){
            //bfs : find the shortest path:
            if(ret_user(u1) != nullptr or ret_user(u1) != nullptr){
                //both users exist;
                //bfs with distance
                if(lower(u1) == lower(u2)){
                return 0;
                }
                unordered_map<string,int> explored;
                unordered_map<string,int> distance;
                queue<User*> nodeess;
                nodeess.push(userrs[lower(u1)]);
                explored[lower(u1)] = 1;
                distance[lower(u1)] = 0;
                //initializing second nodes dist
                distance[lower(u2)] = -1;
                while(!nodeess.empty()){
                    User* temp = nodeess.front();
                    string tempusr = temp->get_username();
                    if(lower(tempusr) == lower(u2)){
                        break;
                    }else{
                        
                        nodeess.pop();
                        vector<User*> mitr = temp->ret_friend_list();
                        for(auto i : mitr){
                            auto usfr = i->get_username();
                            if(explored[lower(usfr)] != 1){
                                nodeess.push(i);
                                explored[lower(usfr)] = 1;
                                distance[lower(usfr)] = distance[lower(tempusr)]+1;
                            }
                        }
                        }
                }
                return distance[lower(u2)];
            }else{
                //either of them does not exist : -1 or error message
                cout<<"Either Of The Usernames Or Both Of Them Don't Exist"<<endl;
                //other errors
                return -2;
            }

        }

};

