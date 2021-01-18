#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <list>
#include <string.h>
#include <sstream>
#include <iterator>
#include <string> 
#include <queue>
#include <deque>
#include <string>
#include <unordered_set>
using namespace std;
int Nmin, Nmax, Hmin, Hmax;
std::vector<int> T1;
std::vector<int> T2;
std::vector<int> current_results;
std::vector<int> current_network;
std::unordered_set<string>marked;
vector<vector<int> > visited;
string my_temp;
vector<string> wisited; 
std::string T1_string;
std::string T2_string;
int final_result = 0;
int second_center = -1;
 
bool im_not_last(int position){
    int network_size = current_network.size();
    if((network_size - 1) != position)return true;
    return false;
}
void load_data(){
    scanf("%d %d %d %d\n", &Nmin, &Nmax, &Hmin, &Hmax);
    int temp;
    while(1){
        temp = getchar();
        if(temp == '\n')break;
 
        T1.push_back((temp - '0'));
    }
    while(1){
        temp = getchar();
        if(temp == '\n')break;
        T2.push_back((temp - '0'));
    }
    std::stringstream result;
    std::copy(T1.begin(), T1.end(), std::ostream_iterator<int>(result, ""));
    T1_string = result.str();
    std::stringstream second_result;
    std::copy(T2.begin(), T2.end(), std::ostream_iterator<int>(second_result, ""));
    T2_string = second_result.str();
 
}
void print_vector(std::vector<int> my_vector){
    for(int i = 0; i < my_vector.size(); i++){
        printf("%d", my_vector[i]);
    }
    cout << "\n";
    return;
}
 
void match_pattern (int start, int end){
    my_temp = T1_string.substr(start, end-start+1);
    marked.insert(my_temp);
    //printf("adding :%s\n",my_temp.c_str() );
    return;
    /*
    if ( std::find(wisited.begin(), wisited.end(), my_temp) != wisited.end() )return;
    wisited.push_back(my_temp);
   std::string::size_type pos = 0;
   int length = T2_string.length();
   int temp_length = my_temp.length();
   while ((pos = T2_string.find(my_temp, pos )) != std::string::npos) {
      ++ final_result;
      pos += temp_length;
      if(pos+temp_length>length)break;
   }
    return;*/
}
 
void search_for_network(int position, int my_depth, int origin_depth, int counter, int origin_position, int max_height){
    if(position == origin_position || (my_depth == 0 && T1[position-1] == 1)){// i.e. I am the starting point for the search for a network
        my_depth++;
        search_for_network(position+1, my_depth, my_depth, counter+1, position, max_height);
    }else if(T1[position] == 0){
        my_depth++;
        if(my_depth - origin_depth <= Hmax && my_depth >= origin_depth && counter +1 <= Nmax){
            if(my_depth-origin_depth > max_height)max_height = my_depth-origin_depth;
            search_for_network(position+1, my_depth, origin_depth, counter+1, origin_position, max_height);
        }
    }else if(T1[position] == 1){
        my_depth--;
        if(counter == 1 && my_depth == origin_depth-1 && Hmin <= 0 && T1[position-1] == 0 && Nmin<= 1 && max_height >= Hmin && max_height <= Hmax){
                match_pattern(origin_position, position);
 
        }
        else if(im_not_last(position) && my_depth >= origin_depth && counter <= Nmax){
            if(counter <= Nmax && counter>= Nmin && my_depth == origin_depth && T1[position+1] == 1 && max_height >= Hmin && max_height <= Hmax){
                match_pattern(origin_position, position+1);
            }
            search_for_network(position+1, my_depth, origin_depth, counter, origin_position, max_height);
        }
    }
}
 
 
void T2_search_for_network(int position, int my_depth, int origin_depth, int counter, int origin_position, int max_height){
    if(position == origin_position || (T2[position] == 0 && my_depth == 0 && T2[position-1] == 1)){// i.e. I am the starting point for the search for a network
        my_depth++;
        T2_search_for_network(position+1, my_depth, my_depth, counter+1, position, max_height);
    }else if(T2[position] == 0){
        my_depth++;
        if(my_depth - origin_depth <= Hmax && my_depth >= origin_depth && counter +1 <= Nmax){
            if(my_depth-origin_depth > max_height)max_height = my_depth-origin_depth;
            T2_search_for_network(position+1, my_depth, origin_depth, counter+1, origin_position, max_height);
        }
    }else if(T2[position] == 1){
        my_depth--;
        if(counter == 1 && my_depth == origin_depth-1 && Hmin <= 0 && T2[position-1] == 0 && Nmin<= 1 && max_height >= Hmin && max_height <= Hmax){
                my_temp = T2_string.substr(origin_position, position-origin_position+1);
                //cout<< "WE ARE ON THE LOOK\n";
                if(marked.count(my_temp) >= 1){
                    final_result++;
                }else{
                    //cout << "we couldnt find \n";
                    //printf("%s\n in the set\n",my_temp.c_str() );
                }
 
        }
        else if(im_not_last(position) && my_depth >= origin_depth && counter <= Nmax){
            if(counter <= Nmax && counter>= Nmin && my_depth == origin_depth && T2[position+1] == 1 && max_height >= Hmin && max_height <= Hmax){
                //cout<< "my origin position is :"<< origin_position<<" WE ARE ON THE LOOK for\n";
                my_temp = T2_string.substr(origin_position, position-origin_position+2);
                //printf("%s\n in the set\n",my_temp.c_str() );
                if(marked.count(my_temp) >= 1){
                    final_result++;
                    //cout<< "we found\n";
                }else{
                    //cout << "we couldnt find \n";
                     
                }
            }
            T2_search_for_network(position+1, my_depth, origin_depth, counter, origin_position, max_height);
        }
    }
}
 
 
void helper(){
    int max = T1.size();
    int bound = max-Nmin*2;
    current_network = T1;
    for(int i = 1; i < bound; i++){
        if(T1[i] == 0){ // only start search from selected points
            search_for_network(i, 0, 0, 0, i, 0);
 
        }
    }
    max = T2.size();
    bound = max - Nmin*2;
    current_network = T2;
    //print_vector(T2);
    for(int i = 1; i < bound; i++){
        if(T2[i] == 0){ // only start search from selected points
            T2_search_for_network(i, 0, 0, 0, i, 0);
 
        }
    }
}
int main(){
    load_data();
    helper();
    printf("%d\n",final_result );
    return 0;
}