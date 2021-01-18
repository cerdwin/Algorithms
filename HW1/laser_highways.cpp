#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <string.h>
#include <queue>
 using namespace std;
int towns, cities, capital, connections, towns_visited, cities_visited, a, b, my_distance, result;
int *** travel_plan;
  
typedef struct{
    int is_capital;
    int id;
    int layer;
    int is_connected;
    int best_distance;
    int best_neighbour;
    int towns_connected;
    int * connection_array;
    int * durations;
    int boss;
    int boss_distance;
  
    int * component;
    int component_members;
}town_t;
  
typedef struct edge_t{
    int weight;
    int first;
    int second;
    friend bool operator<(const edge_t& a, const edge_t& b){
        return a.weight > b.weight;
    }
  
}edge_t;
 //source:https://stackoverflow.com/questions/47945554/struct-comparison-in-a-priority-queue/47945613
struct comparator
{
    bool operator()(const edge_t * a, const edge_t * b)
    {
        return a->weight > b->weight;
    }
};
  
std::priority_queue<edge_t*,vector<edge_t*>,comparator> edges;
//queue <edge_t*> edges;
queue <town_t*> joining_towns;
town_t ** town_array;
void load_data(){
    scanf("%d %d %d", &towns, &connections, &capital); // we record no. of towns, connections and which is capital
    town_array = (town_t**)calloc(towns,sizeof(town_t*)); // we make room to store structs of towns
    for(int i = 0; i < towns; i++){
        town_t * temp_town = (town_t*)calloc(1, sizeof(town_t));
        temp_town->id = i;
        temp_town->is_capital = temp_town->towns_connected = 0;
        temp_town->connection_array =(int *)calloc(10000,sizeof(int));
        temp_town->durations =(int *)calloc(10000,sizeof(int));
        temp_town->towns_connected = temp_town->is_connected = 0;
        temp_town->best_distance = 999999;
        temp_town->best_neighbour = -1;
        temp_town->boss = -1;
        temp_town->boss_distance = 99999;
        temp_town->layer = 99999;
        temp_town->component = (int*)malloc(sizeof(int)*towns);
        temp_town->component[0] = temp_town->id;
        temp_town->component_members = 1;
  
        town_array[i] = temp_town;
    }
    town_array[capital]->is_capital = 1; // we make note of which one is the capital
    town_array[capital]->is_connected = 1;
    town_array[capital]->best_neighbour = capital;
    town_array[capital]->best_distance = 0;
    town_array[capital]->layer = 0;
    town_array[capital]->boss = capital;
    town_array[capital]->boss_distance = 0;
  
    for(int i = 0; i < connections; i++){
        scanf("%d %d %d", &a, &b, &my_distance);
        //adding edges//
        edge_t* temp_edge = new edge_t();
        temp_edge->weight = my_distance;
        temp_edge->first = a;
        temp_edge->second = b;
        edges.push(temp_edge);
        ///
        town_array[a]->connection_array[town_array[a]->towns_connected] = b;
        town_array[a]->durations[town_array[a]->towns_connected] = my_distance;
        town_array[b]->connection_array[town_array[b]->towns_connected] = a;
        town_array[b]->durations[town_array[b]->towns_connected] = my_distance;
        town_array[a]->towns_connected++;
        town_array[b]->towns_connected++;
    } 
}  
  
  
  
  
void sort(){
    for(int i = 0; i < towns; i++){
        for(int s = town_array[i]->towns_connected-1; s >=0 ; s--){
            for(int x = 0; x < s-1; x++){
                if(town_array[i]->durations[x] > town_array[i]->durations[x+1]){
                    int temp_town = town_array[i]->connection_array[x];
                    int temp_duration = town_array[i]->durations[x];
                    town_array[i]->connection_array[x] = town_array[i]->connection_array[x+1];
                    town_array[i]->connection_array[x+1] = temp_town;
                    town_array[i]->durations[x] = town_array[i]->durations[x+1];
                    town_array[i]->durations[x+1] = temp_duration;
                }
                  
            }
              
        }
          
    }
}
  
void filling(){
    int max_layer = 0;
    joining_towns.push(town_array[capital]);
    int counter = 0;
    while(!joining_towns.empty()){
        town_t* current_city = joining_towns.front();
        joining_towns.pop();
        for(int i = 0; i < current_city->towns_connected; i++){
            counter++;
            if(town_array[current_city->connection_array[i]]->layer > current_city->layer+1){
                joining_towns.push(town_array[current_city->connection_array[i]]);
                town_array[current_city->connection_array[i]]->layer = current_city->layer+1;
                if(max_layer<current_city->layer+1){
                    max_layer = current_city->layer+1;
                }
            }
        }
    }
  
    std::priority_queue<edge_t*,vector<edge_t*>,comparator> temp_edges;
    for(int layer = 1; layer<=max_layer;layer++){
        temp_edges = edges;       
        for(int i = 0; i < connections; i++){
            edge_t* curr = temp_edges.top();
            temp_edges.pop();
            if(town_array[curr->first]->layer <= layer && town_array[curr->second]->layer <= layer && (town_array[curr->first]->is_connected==0 || town_array[curr->second]->is_connected==0)/*((town_array[curr->first]->is_connected==1 && town_array[curr->second]->is_connected==0)||(town_array[curr->first]->is_connected==0 && town_array[curr->second]->is_connected==1))*/){
                  
                if(town_array[curr->first]->is_connected == 0 && town_array[curr->second]->is_connected == 1){
                   // town_array[curr->first]->is_connected = 1;
                    for(int i = 0; i < town_array[curr->first]->component_members; i++){
                        town_array[town_array[curr->first]->component[i]]->is_connected = 1;
  
                    }
                }else if (town_array[curr->second]->is_connected == 0 && town_array[curr->first]->is_connected == 1){
                    for(int i = 0; i < town_array[curr->second]->component_members; i++){
                        town_array[town_array[curr->second]->component[i]]->is_connected = 1;
  
                    }
                }else{// both cities are "free-floating", i.e. have not been added to a component
                    if(town_array[curr->first]->component == town_array[curr->second]->component)continue; // we are already members of the same component
                      
                    for(int i = town_array[curr->first]->component_members; i<town_array[curr->first]->component_members+town_array[curr->second]->component_members; i++){
                        town_array[curr->first]->component[i] = town_array[curr->second]->component[i-town_array[curr->first]->component_members];
                    }
                    town_array[curr->first]->component_members+=town_array[curr->second]->component_members;
                    for(int i = 0; i < town_array[curr->first]->component_members; i++){
                        town_array[town_array[curr->first]->component[i]]->component = town_array[curr->first]->component;
                        town_array[town_array[curr->first]->component[i]]->component_members = town_array[curr->first]->component_members;
  
                    }
                }
                result+=curr->weight;
            }
        }
    }
  
}
  
  
  
  
void soso_filling(){
    joining_towns.push(town_array[capital]);
  
    while(!joining_towns.empty()){
        town_t * current_city = joining_towns.front(); // vezmu si prvni prvek ve fronte
        printf("%d processing city:%d, with layer:%d\n",current_city->id, current_city->id, current_city->layer );
        joining_towns.pop(); // popnu si jej z fronty
         
        if(current_city->is_connected && current_city->id != capital){
            printf(" since this city is connected, I skip\n");
            continue;
        }
        for(int i = 0; i < current_city->towns_connected;i++){
            if(town_array[current_city->connection_array[i]]->layer>current_city->layer+1){ // if a neighbour has not been connected   
                town_array[current_city->connection_array[i]]->layer=current_city->layer+1;
                  
                joining_towns.push(town_array[current_city->connection_array[i]]);
                printf(" adding:%d\n", town_array[current_city->connection_array[i]]->id);
                continue;
            }if(town_array[current_city->connection_array[i]]->layer<=current_city->layer){
                if(current_city->durations[i]< current_city->best_distance ){
                    current_city->best_distance = current_city->durations[i];
                    current_city->best_neighbour = current_city->connection_array[i];
                    printf("   my best distance is now %d to %d\n",current_city->best_distance, current_city->best_neighbour );
                }if(town_array[current_city->connection_array[i]]->is_connected && current_city->durations[i]<current_city->boss_distance){
                    current_city->boss_distance = current_city->durations[i];
                    current_city->boss = current_city->connection_array[i];
                     printf("   my BOSS distance is now %d to %d\n",current_city->boss_distance, current_city->boss );
                }
            }
  
        }
        printf("kiki, current-best neighbour->is_connected:%d:%d\n", current_city->best_neighbour, town_array[current_city->best_neighbour]->is_connected);
        if(current_city->id != capital){
            printf("eups\n");
            if(town_array[current_city->best_neighbour]->is_connected == 1){
                printf("yup\n");
                current_city->is_connected = 1;
                current_city->boss = current_city->best_neighbour;
                current_city->boss_distance = current_city->best_neighbour;
                printf(" my best neighbour is connected, so I add %d as my price to get to %d\n", current_city->best_distance, current_city->best_neighbour);
                result+=current_city->best_distance;
  
                //all ppl whose best neighbour is me add their best distances
                ///////////////////
                for(int i = 0; i< towns; i++){
                        if(town_array[i]->is_connected == 0 && (town_array[i]->best_neighbour == current_city->id || town_array[i]->best_neighbour == current_city->best_neighbour)){
                            town_array[i]->is_connected = 1;
                            printf("xyx adding %d for %d\n", town_array[i]->best_distance, town_array[i]->id);
                            result +=town_array[i]->best_distance;
                        }
                    }
            }else if(town_array[current_city->best_neighbour]->boss == -1){
                printf(" my best neighbour isn't processed yet, so I add myself to queue again\n");
                //////
                town_array[current_city->best_neighbour]->boss = current_city->boss;
                town_array[current_city->best_neighbour]->boss_distance = current_city->boss_distance;
                //////
                joining_towns.push(current_city);
            }
            else if(town_array[current_city->best_neighbour]->best_neighbour != current_city->id && town_array[current_city->best_neighbour]->is_connected == 0){
                printf("I have to wait for my best neighbour to finish...\n");
                if(current_city->boss_distance < town_array[current_city->best_neighbour]->boss_distance){
                    town_array[current_city->best_neighbour]->boss_distance = current_city->boss_distance;
                    town_array[current_city->best_neighbour]->boss = current_city->boss;    
                }
                joining_towns.push(current_city);
  
            }
            else { /// they are each others' best 
                /////tady si dej pozor na layerssss
                printf("***rosada***\n");
                if(town_array[current_city->best_neighbour]->boss_distance<=current_city->boss_distance){
                    printf(" // a. adding %d to result\n",current_city->best_distance );
                    result+=current_city->best_distance;
                    current_city->boss_distance = town_array[current_city->best_neighbour]->boss_distance; 
                    current_city->boss = town_array[current_city->best_neighbour]->boss;
                    current_city->is_connected = 1;
                      
                    // zapojim i onoho souseda
                    result+=town_array[current_city->best_neighbour]->boss_distance;
                    printf(" //adding also %d to result\n", town_array[current_city->best_neighbour]->boss_distance);
                    town_array[current_city->best_neighbour]->is_connected = 1;
                    for(int i = 0; i< towns; i++){
                        if(town_array[i]->is_connected == 0 && (town_array[i]->best_neighbour == current_city->id || town_array[i]->best_neighbour == current_city->best_neighbour)){
                            town_array[i]->is_connected = 1;
                            printf("xxx adding %d for %d\n", town_array[i]->best_distance, town_array[i]->id);
                            result +=town_array[i]->best_distance;
                        }
                    }
  
                }else{
                    printf("my boss distance, %d is better than theirs %d\n",current_city->boss_distance,town_array[current_city->best_neighbour]->boss_distance  );
                    printf(" //b. adding %d to result\n",current_city->best_distance );
                    result+=current_city->best_distance;
                    town_array[current_city->best_neighbour]->boss_distance = current_city->boss_distance;
                    town_array[current_city->best_neighbour]->is_connected = 1;
                    town_array[current_city->best_neighbour]->boss = current_city->boss;
                      
                    //zapojim i current city
                    printf(" //adding also %d to result\n", current_city->boss_distance);
                    result+=current_city->boss_distance;
                    current_city->is_connected = 1;
                    for(int i = 0; i< towns; i++){
                        if(town_array[i]->is_connected == 0 && (town_array[i]->best_neighbour == current_city->id || town_array[i]->best_neighbour == current_city->best_neighbour)){
                            town_array[i]->is_connected = 1;
                            printf("xxx adding %d for %d\n", town_array[i]->best_distance, town_array[i]->id);
                            result +=town_array[i]->best_distance;
                        }
                    }
                      
                }
            }
              
        }        
          
    }
  
}
  
int main (){
    load_data();
    sort();
    filling();
    printf("%d", result);
    return 0;
}