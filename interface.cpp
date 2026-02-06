#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include "structures.cpp"
using namespace std;

//Defining the newtork:
Network fb;

//a helper function to check if a given string holds a valid int'
bool valint(string s){
    bool valid = true;
    for(char i : s){
        int ascii = i;
        if(ascii< 48 or ascii>57 ){
            valid = false;
            break;
        }
    } 
    return valid; 
}
//defining a text processing function : does all the processing
void process_text(string inp){
    // analysing if valid using the fact that all inputs don't exceed word length 3 except maybe the one corr to posting
    //storing the input as diff words in a vector
    stringstream ss(inp);
    string word;
    vector<string> words;
    while (ss >> word) {
        words.push_back(word);
    }
    if(words.size()== 2){
        //err msg is covered in the corr network method
        //either add user or list friends;
        if(words[0]== "ADD_USER"){
            fb.insert_user(words[1]);
            
        }else if(words[0]== "LIST_FRIENDS"){
            fb.list_friendss(words[1]);
        }else{
            cout<<"Invalid Input"<<endl;
        }
    }else if(words.size()== 3 and words[0]!= "ADD_POST"){
        //all other methods except post;
        if(words[0]== "ADD_FRIEND"){
            fb.add_friend(words[1], words[2]);
        }else if(words[0]=="SUGGEST_FRIENDS"){
            //checking if the third word is a valid integer:
            if(valint(words[2])){
                //----- Work on the output format
                fb.suggest_friends(words[1],stoi(words[2]));
            }else{
                cout<<"Invalid Input : N Must Be A Non -ve Integer other than -1"<<endl;
            }
        }else if(words[0]=="DEGREES_OF_SEPARATION"){
           int dos =  fb.degrees_of_sepr(words[1],words[2]);
           if(dos!= -2){
            //one or both usnms exist
            cout<<"The Degree of Sepr is "<<dos<<endl;
           }
            //owise taken care by structures wala fxn
        }else if(words[0]=="OUTPUT_POSTS"){
            if(words[2] != "-1" and valint(words[2])){
                fb.output_postss(words[1],stoi(words[2]));
            }else if(words[2] == "-1"){
                fb.output_postss(words[1],-1);
            }else{
                cout<<"Invalid Input : N Must Be A Non -ve Integer"<<endl;
            }
        }else{
            cout<<"Invalid Input"<<endl;
        }
    }else{
        //add post
        if(words[0]=="ADD_POST" and words.size()>=3){
            string cont = "";
            for(int i =2; i<words.size();i++){
                cont += words[i] + " ";
            }
            fb.createe(words[1],cont);
        }else{
            cout<<"Invalid Input"<<endl;
        }
    }
}

int main()
{
    //setup : a command prompt that keeps running until stopped
    cout<<"To stop/end the programme, enter STOP"<<endl;
    while (1)
    {
        //take input
        string temp = "";
        getline(cin, temp);
        //process and give output if necessary;
        if (temp == "STOP")
        {
            break;
        }
        process_text(temp);
    }
    
    return 0;
}
