#include "header.h"
#include <limits.h>
#include <stdlib.h>

// ANY STATIC FUNCTIONS ARE UP HERE

static void dfs(int n, const connection_t connections[n][n], int start, int visited[n]){
    visited[start] = 1;
    for (int i = 0; i < n; i++){
        if ((connections[start][i].distance != INT_MAX) && (!visited[i])){
                        dfs(n,connections,i,visited);
        }
    }
}

static void getTranspose(int n, const connection_t connections[n][n], int start, int visited[n]){
    connection_t temp[n][n];
    for (int i = 0; i < n; i++){
        for (int j = 0; j< n; j++){
            temp[j][i] = connections[i][j];
        }
    }
    dfs(n, temp, 0, visited);
}


// YOUR SOLUTIONS BELOW

int q1(int n, const connection_t connections[n][n]){
    int visited[n];
    for (int i = 0; i < n; i++){
        visited[i]=0;
    }
    dfs(n, connections, 0, visited);
    for (int i = 0; i < n; i++){
        if (visited[i]==0){
            return 0;
        }
    }
    for (int i = 0; i < n; i++){
        visited[i]=0;
    }
    getTranspose(n, connections, 0, visited);   
    for (int i = 0; i < n; i++){
        if (visited[i]==0){
            return 0;
        }
    }
    return 1;
}

int q2(const airport_t *src, const airport_t *dest, int n, int k,
       const connection_t connections[n][n])
{
    return 0;
}

int q3(const airport_t *src, int n, const connection_t connections[n][n])
{
    return 0;
}

void q4(int n, int (*predicate_func)(const airport_t *, const airport_t *),
        airport_t airport_list[n])
{

}

pair_t q5(int n, airport_t airports[n])
{
    pair_t ans = {-1, -1};
    return ans;
}

int q6(int n, int amount, const int entry_fee[n])
{
    return 0;
}

void q7(int n, const char *pat, int contains[n], const airport_t airports[n])
{

}

int q8(int n, int trip_order[n - 1], const connection_t connections[n][n])
{
    return 0;
}

int q9(int n, pair_t edges[n - 1], const connection_t connections[n][n])
{
    return 0;
}

void q10(int n, int k, const airport_t *src,
         const connection_t connections[n][n], const int destinations[k],
         int costs[k])
{

}

// END
