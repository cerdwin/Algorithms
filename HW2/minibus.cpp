#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <string.h>
#include <queue>
#include <deque>
 using namespace std;
 #define min(a, b) ((a < b) ? a : b)
 #define max(a, b) ((a < b) ? (b) : (a))
int towns, cities, capital, connections, towns_visited, cities_visited, a, b, my_distance, result;
int *** travel_plan;
  
typedef struct{
    int id;
    int lowlink;
    int index;
   int end_count;
   int * ending_edges;
   int is_safe;
    int component;
    int component_members;
}town_t;
  
typedef struct component_t{
    int id;
    int value;
    int entry_journey_value;
    int best_result;
    int * neighbours;
    int neighbour_count;  
  
}component_t;
 
typedef struct edge_t{
    int start;
    int end;
}edge_t;
 
edge_t ** edge_array;
int * open_nodes;
int * template_component;
int lowlink_counter = 0;
int SCC = 0;
//int ** components;
component_t ** component_array;
town_t ** town_array;
int ** component_adjacency_matrix;
deque <int> component_dequeue;
void load_data(){
    scanf("%d %d", &towns, &connections); // we record no. of towns, connections and which is capital
    town_array = (town_t**)calloc(towns,sizeof(town_t*)); // we make room to store structs of towns// we make room to store structs of towns
    // I initialise an array of towns
    for(int i = 0; i < towns; i++){
        town_array[i] = (town_t*)calloc(1, sizeof(town_t));
        town_array[i]->id = i; // each town has an id
        town_array[i]->ending_edges = (int *)calloc(1000,sizeof(int));
        town_array[i]->is_safe = 1;
    }
    // I make an array of edges
    int origin;
    int destination;
    edge_array = (edge_t**)calloc(connections,sizeof(edge_t*));
    for(int i = 0; i < connections; i++){
        scanf("%d %d", &origin, &destination);
        edge_array[i] = (edge_t*)calloc(1, sizeof(edge_t*));
        edge_array[i]->start = origin;
        edge_array[i]->end = destination;
        town_array[origin]->ending_edges[town_array[origin]->end_count++] = destination;
    }  
}  
void safe(){ // we mark towns as "safe", if they are not connected to other towns in different components via an edge
    for(int i = 0; i< connections; i++){
        if(town_array[edge_array[i]->start]->component != town_array[edge_array[i]->end]->component){
            if(town_array[edge_array[i]->start]->is_safe == 1){
                town_array[edge_array[i]->start]->is_safe = 0;
                component_array[town_array[edge_array[i]->start]->component]->value--;
            }
            if(town_array[edge_array[i]->end]->is_safe == 1){
                town_array[edge_array[i]->end]->is_safe = 0;
                component_array[town_array[edge_array[i]->end]->component]->value--;
            }
        }
    }
}
//////////////////////////////////////////////////////
void tarjan(int current){ // Tarjan algorithm finding strongly connected components, starting in the first node
    town_array[current]->index = town_array[current]->lowlink = lowlink_counter++;
    open_nodes[current] = 1; // I make sure I can distinguish between towns currently being processed and those which are not
    component_dequeue.push_back(current);
    for(int i = 0; i < town_array[current]->end_count; i++){
        if(open_nodes[town_array[current]->ending_edges[i]] == 0){ // not yet visited
            tarjan(town_array[current]->ending_edges[i]);
            town_array[current]->lowlink = min(town_array[current]->lowlink, town_array[town_array[current]->ending_edges[i]]->lowlink);
        }else if(open_nodes[town_array[current]->ending_edges[i]] == 1){ // uzavirame cuklus
            town_array[current]->lowlink = min(town_array[current]->lowlink, town_array[town_array[current]->ending_edges[i]]->index);   
        }
    }
    if(town_array[current]->index == town_array[current]->lowlink){ // head of scc
        int current_members = 0;
        int temp = -1;
        //// creating a new component ////
        component_array[SCC] = (component_t*)calloc(1,sizeof(component_t));
        component_array[SCC]->id = SCC;
        component_array[SCC]->neighbours = (int*)malloc(sizeof(int)*SCC);
        component_array[SCC]->neighbour_count = 0;
        component_array[SCC]->value = 0;
        while(temp != current ){
            int temp = component_dequeue.back();
            component_dequeue.pop_back();
            town_array[temp]->component = SCC;
            component_array[SCC]->value++; // we then decrease the value whenever we find a node which is not safe and belongs to it
            open_nodes[temp] = -1; // closing the node
            if(temp == current)break;
        }
         
        SCC++;
 
    }
 
}
void set_up_components(){
    for(int i = 0; i < connections; i++){
        if(town_array[edge_array[i]->start]->component != town_array[edge_array[i]->end]->component){
            if(component_adjacency_matrix[town_array[edge_array[i]->start]->component][town_array[edge_array[i]->end]->component] == 0){
                component_adjacency_matrix[town_array[edge_array[i]->start]->component][town_array[edge_array[i]->end]->component] = 1;
                component_array[town_array[edge_array[i]->start]->component]->neighbours[component_array[town_array[edge_array[i]->start]->component]->neighbour_count++] = town_array[edge_array[i]->end]->component;
            }
             
        }
    }
}
int resolve(int current_component, int value_so_far){
    if(value_so_far < component_array[current_component]->entry_journey_value)return 0;
    if(component_array[current_component]->best_result != 0)return component_array[current_component]->best_result;
    int my_max = 0;
    component_array[current_component]->entry_journey_value = value_so_far;
    for(int i = 0; i < component_array[current_component]->neighbour_count; i++){
        my_max = max(my_max, resolve(component_array[component_array[current_component]->neighbours[i]]->id, value_so_far+component_array[current_component]->value));       
    }
    int midresult = my_max + component_array[current_component]->value;
    component_array[current_component]->best_result = midresult;
    return midresult;
}
void find_path(){
    for(int i = 0; i < SCC; i++){
        if(component_array[i]->best_result == 0)result = max(result, resolve(i, 0)); // if we haven't explored a certain component, we try to find the most valuable journey
    }
 }
int main (){
     
    load_data(); // first I load all data,s tore them into arrays of structs representing towns
    // I mark all my towns as open and find strongly connected components
    open_nodes = (int*)calloc(towns,sizeof(int));
    component_array = (component_t**)malloc(sizeof(component_t*)*towns/2);
    for(int i = 0; i < towns; i++){
        if(open_nodes[i] == 0)tarjan(i); // we find strongly connected components using the Tarjan algorithm
    }
    //printf("All %d SCCs found\n", SCC);
    safe(); // determines whether each city is safe, i.e. is connected only to members of the same component
    //printf("All safe nodes determined\n");
    component_adjacency_matrix = (int **)calloc(SCC,sizeof(int*));
    for(int i = 0; i < SCC; i++){
        component_adjacency_matrix[i] = (int*)calloc(SCC, sizeof(int));
    }
    set_up_components();
    //printf("All components set\n");
    find_path();
   // printf("Path calculated\n");
    printf("%d", result);
    return 0;
}