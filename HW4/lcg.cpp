#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>
#include <iterator>
using namespace std;
 
typedef long long int ll; 
int Mmin, Mmax, R, result;
std::vector<ll>primes;
std::vector<ll>gcd_partners;// says w how many other numbers each member of subprimes can be matched with
std::vector<ll>subprimes;
std::vector<vector<ll> > gcds;
std::vector<std::vector<ll> > grandneighbours;
 
 
// Utility function to find 
// GCD of 'a' and 'b' 
ll gcd(ll a, ll b) 
{ 
    if (b == 0){
        return a; 
    }
    return gcd(b, a % b); 
} 
 
void Print(const std::vector<ll>& v){
    for(ll i = 0; i< v.size(); ++i) {
        std::cout << subprimes[v[i]] << " ";
    }
    cout<<endl;
}
 
void sieve(){
    std::vector <ll> os(Mmax+1);
    std::iota(os.begin(), os.end(), 0);
    primes = os;
    for (ll i = 2; i< primes.size(); i++){
        ll number_to_sieve = primes[i];
        if(number_to_sieve == 0)continue;
        //printf("MMax:%d, temp:%llu, len(primes):%d\n",Mmax, number_to_sieve, primes.size() );
        ll temp = number_to_sieve;
        while(temp <= Mmax){  // this loop removes all non prime numbers
            temp +=number_to_sieve;
            if(temp>Mmax)break;
            primes[temp] = 0;
        }
    }  
     
    // 1801 still present
    for (int i = 3; i < primes.size(); i++){
        if(primes[i] == 0)continue;
        ll square = pow(i, 2);
        ll initial_square = square;
        ll incrementor = 1;
        while(square < Mmax){
            if(primes[square+1]> 0){
                primes[square+1] = -primes[square+1]; //removing odd squarefree primes' neighbours
            }
            square = initial_square * incrementor;
            incrementor++;
 
        }
        if(primes[i]>0){
            primes[i] = 0;
        }else{
            primes[i] = -primes[i];
        }
    }
     
    primes.erase(primes.begin(), primes.begin() + Mmin);
    primes.erase(
    std::remove(primes.begin(), primes.end(), 0),
    primes.end());
    primes.shrink_to_fit();
     
 
}
 
void recurse(std::vector<std::vector<ll> > gdc_matrix, std::vector<ll> current_left, int r, int my_idx){
    if(r == 1){
        result++;
        return;
    }
    ll size_to_iterate = current_left.size();
    std::vector<ll> current_alternative;
    for(int i = 0; i < current_left.size(); i++){
        if(gdc_matrix[my_idx][current_left[i]] != 0){
            current_alternative.push_back(current_left[i]);
        }
    }
    if(current_alternative.size()< r-1)return;
    for(int i = 0; i < current_alternative.size(); i++){
        if(gdc_matrix[my_idx][current_alternative[i]] == 2){
            std::vector<ll>tmp = current_alternative;
            tmp.erase(std::remove(tmp.begin(), tmp.end(), current_alternative[i]), tmp.end());
            recurse(gdc_matrix, tmp, r-1, current_alternative[i]);
        }
    }
}
 
void solver(std::vector<ll> tmp, int depth){
    //cout<<"we have depth "<<depth<<endl;
    //Print(tmp);
    if(depth == 1){
        result+= tmp.size();
        //cout<<"matched"<<endl;
        return;
    }
    for(ll n = 0; n < tmp.size(); n++){
        //cout<<"I am number "<<subprimes[tmp[n]]<<" n is: "<<n<<endl;
        std::vector<ll> new_neighbours;
        for(ll s = n+1; s < tmp.size(); s++){
            if(gcds[tmp[n]][tmp[s]] == 2){
                //cout<<"I add for me "<<subprimes[tmp[s]]<<endl;
                new_neighbours.push_back(tmp[s]);
            }
        }
        if(new_neighbours.size()>= depth-1){
            int new_depth = depth-1;
           // cout<<"I am eligible, so I send thru from depth"<<depth<<" to depth: "<< new_depth<<endl;
            solver(new_neighbours, new_depth);
        }else{
            //cout<<"I am not eligible more, so I end\n";
        }
    }
}
 
int main() {
 
    result = 0;
    scanf("%d %d %d\n", &Mmin, &Mmax, &R);
    //cout<<"R is:"<<R<<endl;
    sieve(); // Erath
    for(int i = 0; i < primes.size(); i++){
        subprimes.push_back(primes[i]-1);
    }
     
    std::vector<std::vector<ll> > neighbours(subprimes.size(), std::vector<ll>());
    std::vector<std::vector<ll> > gdc_matrix(subprimes.size(), std::vector<ll>(subprimes.size()));
    for(int i = 0; i < subprimes.size(); i++){
        gcd_partners.push_back(0);
        for (int s = i+1; s < subprimes.size(); s++){
            ll current_gcd = gcd(subprimes[i], subprimes[s]);
            if( current_gcd == 2){
                gcd_partners[i] = gcd_partners[i]+1;
                gdc_matrix[i][s] = current_gcd;
                neighbours[i].push_back(s);
            }
             
        }
        sort(neighbours[i].begin(), neighbours[i].end());
    }
 
    //cout<<"subprimes: "<<endl;
    //for(int i = 0; i < subprimes.size(); i++){
        //cout<<subprimes[i]<<" ";
    //}
    //cout<<endl;
    /*for(int i = 0; i < subprimes.size(); i++){
        for(int x = 0; x < subprimes.size(); x++){
            cout<<gdc_matrix[i][x]<<"    ";
        }
        cout<<endl;
    }*/
    gcds = gdc_matrix;
    //cout<<"......................................................................"<<endl;
    for(int i = 0; i < subprimes.size();i++){
 
        if(gcd_partners[i]< R-1)continue;
        //cout<<"fixing number "<<subprimes[i]<<endl;
        std::vector<ll> tmp = neighbours[i];
        solver(tmp, R-1);
 
        /*
        for(ll s = 0; s < neighbours[i].size()-R+2; s++){
            tmp.erase(tmp.begin()+0);
            if(tmp.size()<R-2)break;
            recurse(gdc_matrix, tmp, R-1, neighbours[i][s]);
             
        }*/
    }
 
     
    cout<<result<<endl;
    return 0;
}