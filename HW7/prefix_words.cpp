#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <stdio.h>
#include <numeric>
#include <algorithm>
#include <string> 
#include <stack>
#include <cmath>
#include <climits>
#include <unordered_set>
#include <sstream>      // std::istringstream
using namespace std;
int N, S; // N is the number of states in our automaton, S is the size of the alphabet it uses
typedef struct node_t{
    int idx;
    int neighbour_count;
    int * neighbours;
    int ** transfer_map;
    int * transfer_connection_count;
    std::vector<vector<int> >  connection_map; 
    vector< set<int> > set_of_neighbours;
    int * connection_count;
    bool is_final;
 
}node_t;
char alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
node_t** automaton;
string word;
std::istringstream iss;
int minimum, maximum, word_length;
set<int>final_set;
 
void load_input(){
    scanf("%d %d", &N,&S); 
    string line;
    automaton = (node_t**)calloc(N,sizeof(node_t*)); // array of structs
    //cout<<"S is:"<<S<<endl;
    for(int i = 0; i < N; i++){
        automaton[i] = (node_t*)calloc(1,sizeof(node_t));
        automaton[i]->transfer_map = (int**)calloc(200, sizeof(int*));
        automaton[i]->neighbour_count = 0;
        automaton[i]->neighbours = (int*)calloc(N, sizeof(int));
        vector<set<int>> vsi(200);
        automaton[i]->set_of_neighbours = vsi;
        for(int e = 0; e< 200; e++){
            automaton[i]->transfer_map[e] = (int*)calloc(N, sizeof(int));
        }
    }
    int i = 0;
    getline(cin, line);
    int temp_num;
    char temp_char;
    scanf("%d %c", &temp_num, &temp_char);
    while(getline(cin, line)){
        //cout<<"my line is: "<<line<<" this was the line"<<endl;
        if(i>0){
            scanf("%d %c", &temp_num, &temp_char);
        }
        automaton[i]->idx = temp_num;
        if(temp_char == 'F'){
            automaton[i]->is_final = true;
            final_set.insert(temp_num);
        }else if(temp_char == '-'){
            automaton[i]->is_final = false;
        }
        automaton[i]->connection_count = (int*)calloc(S, sizeof(int));
        automaton[i]->transfer_connection_count = (int*)calloc(200, sizeof(int));
        int size = 0;
        //cout<<"idx: "<<i<<endl;
        for(int x = 0; x < S; x++){ // for every letter ...e.g. a 2 3 b 5 6 c 7 8
            //cout<<"X is: "<<x<<endl;
 
            string rest = line.substr(line.find(std::string() + " "+alphabet[x])+1); // tmp is the part which follows the specific letter so, e.g. c 5 6
            //cout<<"Z linky nam zbyva: "<<rest<<endl;
            string midpart;
            if(x < S-1){
                //cout<<"looking for suitable substring....that ends with: "<< alphabet[x+1]<<" since x is: "<< x<<endl;
                midpart = rest.substr(1, rest.find(std::string() + " "+alphabet[x+1])); // the separation is done by double spaces.. we now only have numbers which follow the letter
            }else{
                //cout<<"doing rest\n";
                midpart = rest.substr(rest.find(std::string() + alphabet[x])+1, rest.length()-1);
            }
            //cout<< "midpart is: "<<midpart<<endl;
            //cout<< "letter: "<<alphabet[x]<<endl;
            for(int z = 0; z < midpart.size();z++){ // ted mame rozparsovany akorat string, kde jsou indexy nodes
                if(midpart[z] != ' '){
                    string tmp = std::string()+midpart[z];
                    while(midpart[z+1]!= ' '){ // we parse until: a) we're at the end of the string, b .. follows a space and another letter c..follow two spaces
                        tmp.push_back(midpart[z+1]);
                        z++;
                        if(z == midpart.length())break;
                    }
                    stringstream obj(tmp);
                    int position = 0;
                    obj >> position;
                    //cout<< "position: "<<position<<endl;
                    automaton[i]->set_of_neighbours[alphabet[x]-'0'].insert(position);
                     
                }
            }
        }
        i++;
        if(i == N){
            getline(cin, word);
            word_length = word.length();
            break;
        }
    }
}
void print_set(set<int> curr){
    for(int state: curr){
        cout << ' '<<state;
    }
    cout<<endl;
}
 
 
void set_check(set<int> some_set, int generation){
    if(some_set.size()>0){
        for(int final_state: final_set){
            set<int>::iterator it = some_set.find(final_state);
            if (it != some_set.end())
            {
                if(generation < minimum)minimum = generation;
                if(generation>maximum)maximum = generation;
            }
        }
    }
}
void word_search(set<int>my_set){
    set<int> tempset;
    int generation = word_length+1;
    while(!my_set.empty()){
        tempset.clear();
        for(int state: my_set){
            for(int i = 0; i < 200; i++){
                for(int s: automaton[state]->set_of_neighbours[i]){
                    tempset.insert(s);
                }
            }
        }
        set_check(tempset, generation);
        my_set = tempset;
        generation++;
    }
 
}
 
set<int> prefix_search(){
    set<int>ret;
    ret.insert(0);
    set<int>temp;
    for(int i = 0; i < word_length; i++){
        for(int state : ret){
            for(int s : automaton[state]->set_of_neighbours[word[i]-'0']){
                temp.insert(s);
            }
        }
        //cout<<" index of set: "<<i<< "word[i]: "<< word[i] <<endl;
        //print_set(temp);
        ret = temp;
        temp.clear();
    }
    set_check(ret, word_length);
    return ret;
}
 
 
int main() {
    minimum = INT_MAX;
    maximum = INT_MIN;
    load_input();
    set<int>initial_pop = prefix_search();
    //print_set(initial_pop);
    word_search(initial_pop);
    printf("%d %d\n",minimum, maximum );
    return 0;
}