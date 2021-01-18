#include <iostream>
#include <string>
using namespace std;
int A, L, K, a, b, c, d;
int result = 0;
int short_match_index = 0; // THis tells me from which index I have 
int left_short_match_index = 0;
int right_short_match_index;
string word;
void load_input(){
    scanf("%d %d %d\n", &A, &L, &K);
    getline(cin, word);
    //cout<< "A is: "<< A<<" L is: "<< L <<"K is "<<K<<endl;
    //cout<<word<<endl;
}
 
void hamming_dist(int first, int second){
    int counter = 0;
    int f = first;
    int s = second;
    for(int i = 0; i < L; i++){
        if(word[f+i] == word[s+i])counter++;
    }
    if(L-counter == K){
        result++;
    }
}
 
 
void assymetric_hamming(int big_start, int big_end, int small_start, int small_end){
    string tmp = word.substr(big_start, L);
    bool second_mistake = false;
    int k = small_start;
    for(int i = big_start; i <= big_end; i++){
        if(word[i] != word[k]){
            if(second_mistake){
                return;
            }
            second_mistake = true;
            i+=K-1;
        }else{
            k++;
        }
    }
    result++;
}
 
void pre_compute(){
    for(int i = 0; i < word.length()-(L-K)+1;i++){
        string first_small = word.substr(i, (L-K));
        int small_right_idx = i+(L-K)-1;
        string first_big = word.substr(i, L);
        int large_right_idx = small_right_idx+K;
        for(int v = small_right_idx+1; v < word.length()-(L-K)+1; v++){
            string second_big = word.substr(v, L);
            string second_small = word.substr(v, (L-K));
            if(first_big.length() == L && second_big.length()==L && large_right_idx<v){
                hamming_dist(i,  v);
            }
            if(first_big.length() == L && large_right_idx<v && (first_big != first_small)){
                assymetric_hamming(i, i+L-1, v, v+L-K-1);
            }
            if(second_big.length()==L && (first_big != first_small)){
                assymetric_hamming(v, v+L-1, i, i+L-K-1);
            }
        }
    }
}
int main() {
    load_input();
    pre_compute();
    cout<<result<<endl;
    return 0;
}