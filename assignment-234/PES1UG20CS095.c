#include "header.h"
#include <limits.h>
#include <stdlib.h>
#define ASCIIq7 256

// ANY STATIC FUNCTIONS ARE UP HERE

static void dfsq1(int n, const connection_t connections[n][n], int start, int visited[n]){
    visited[start] = 1;
    for (int i = 0; i < n; i++){
        if ((connections[start][i].distance != INT_MAX) && (!visited[i])){
            dfsq1(n, connections, i, visited);
        }
    }
}

static void getTransposeq1(int n, const connection_t connections[n][n], int start, int visited[n]){
    connection_t temp[n][n];
    for (int i = 0; i < n; i++){
        for (int j = 0; j< n; j++){
            temp[j][i] = connections[i][j];
        }
    }
    dfsq1(n, temp, 0, visited);
}

static void dfsq2(int start, const airport_t *src, const airport_t *dest, int n, int k, const connection_t connections[n][n],int visited[n], int *flag){
    if (*flag == 0 ){   // prevent unecessary executions by stopping further searches once flag is set to 1
        visited[start] = 1;

        // found within given number of steps success
        if ((start == dest->num_id) && (k >= 0)){
            *flag = 1;
            return;
        }


        // trying to see alternative routes since destination not found
        for (int i = 0; i < n; i++){
            if ((connections[start][i].distance != INT_MAX) && (!visited[i])){
                dfsq2(i, src, dest, n, k-1, connections, visited, flag);
            }
        }
        visited[start] = 0;
    }
}

static void dfsq3(int start, const airport_t *src, int n, const connection_t connections[n][n], int visited[n], int *flag){
    if (*flag == 0){    // prevent unecessary executions by stopping further searches once flag is set to 1
        visited[start] = 1;

        // checking if source airport is reachable from current airport while not being a self loop
        if ((start != src->num_id) && (connections[start][src->num_id].distance != INT_MAX)){
            // since possible set flag to 1
            *flag = 1;
        }
        else{
            // else go to another non visited airport and check if route available
            for (int i = 0; i < n; i++){
                if ((connections[start][i].distance != INT_MAX) && (!visited[i]))
                    dfsq3(i, src, n, connections, visited, flag);
            }
        }
    }
}

// getLeftChild and getRightChild are useful as they will be required for question 4 and 5 in which I implement heap sort
static int getLeftChild(int i){
    return ((2 * i) + 1);
}

static int getRightChild(int i){
    return ((2 * i) +2);
}

static void swapq4(airport_t *x, airport_t *y){
    airport_t temp = *x;
    *x = *y;
    *y = temp;
}

static void heapifyq4(int i, int n, int (*predicate_func)(const airport_t *, const airport_t *), airport_t airport_list[n]){
    // find largest among the root and it's left and right child
    int rightIndex = getRightChild(i);
    int leftIndex = getLeftChild(i);
    int max = i;

    if ((leftIndex < n) && !predicate_func(&airport_list[leftIndex], &airport_list[max]))
        max = leftIndex;

    if ((rightIndex < n) && !predicate_func(&airport_list[rightIndex], &airport_list[max]))
        max = rightIndex;

    // swap if required and continue heapify
    if (max != i){
        swapq4(&airport_list[i],&airport_list[max]);
        heapifyq4(max, n, predicate_func, airport_list);
    }
}

static void buildHeapq4(int n, int (*predicate_func)(const airport_t *, const airport_t *), airport_t airport_list[n]){
    // function to build maxheap

    for (int i = n / 2 - 1; i >= 0; i--){
        heapifyq4(i, n, predicate_func, airport_list);
    }
}

// custom implementation of strcmp for replicating the effects of predicate functions given in client file in the client file
static int strcmpq5(const char *a, const char *b){
    const unsigned char *x = (const unsigned char *) a;
    const unsigned char *y = (const unsigned char *) b;

    while ((*x == *y) && (*x != '\0')){
        x++;
        y++;
    }

    return (*x > *y) - (*x < *y);
}

static int compareq5(const airport_t *x, const airport_t *y){
    // note this is a SELF DEFINED STRCMP
    return strcmpq5(x->airport_name, y->airport_name) < 0;
}

static void heapifyq5(int i, int n, airport_t airports[n]){
    // find largest among the root and it's left and right child
    int rightIndex = getRightChild(i);
    int leftIndex = getLeftChild(i);
    int max = i;

    if ((leftIndex < n) && !compareq5(&airports[leftIndex], &airports[max]))
        max = leftIndex;

    if ((rightIndex < n) && !compareq5(&airports[rightIndex], &airports[max]))
        max = rightIndex;

    // swap if required and continue heapify
    if (max != i){
        swapq4(&airports[i],&airports[max]);
        heapifyq5(max, n, airports);
    }
}

static void heapSortq5(int n, airport_t airports[n]){
    for (int i = n / 2 - 1; i >= 0; i--){
        heapifyq5(i, n, airports);
    }

    for (int i = n - 1; i >= 0; i--){
        swapq4(&airports[0],&airports[i]);

        // heapify again
        heapifyq5(0, i, airports);
    }
}
// perform actual string matching on an array sorted according to the airport name
static pair_t findLargestMatchq5(int n, airport_t airports[n]){
    int count;
    int max=0;
    const unsigned char *x, *y;
    pair_t ret = {-1,-1};
    for (int i = 0; i < n-1; i++){
        count = 0;
        x = (const unsigned char *) airports[i].airport_name;
        y = (const unsigned char* ) airports[i+1].airport_name;

        while ((*x==*y) && (*x != '\0')){
            x++;
            y++;
            count++;
        }
        if (count > max){
            max = count;
            ret.first = airports[i].num_id;
            ret.second = airports[i+1].num_id;
        }
    }
    return ret;
}

// sequential search would have been simpler but binary search will be useful when the array size is extremely large
// and luckily sorted array is provided as input
static void binSearchq6(int low, int high, int *flag,int n, int amount, const int entry_fee[n]){
    if (high >= low){
        int mid = (low + high)/2;
        *flag = mid + 1;
        if (amount > entry_fee[mid]){
            binSearchq6(mid+1, high, flag, n, amount, entry_fee);
        }
        else if (amount < entry_fee[mid]){
            binSearchq6(low, mid-1, flag, n, amount, entry_fee);
        }
    }
}

static int getPatterLenq7(const char *pat){
    const unsigned char *temp = (const unsigned char *) pat;
    int count = 0;
    while (*temp != '\0'){
        temp++;
        count++;
    }
    return count;
}

static void buildShiftTableq7(int patternLen, const char *pat, int shiftTable[ASCIIq7]){
    for (int i = 0; i < ASCIIq7; i++){
        shiftTable[i] = patternLen;
    }

    for (int i = 0; i < patternLen-1; i++){
        shiftTable[(int)pat[i]] = patternLen - 1 - i;
    }
}

static void generateNodesq8(int n,int excluded, int graph[n-1]){
    int i = 0;
    int j = 0;
    while (i < n){
        if (i != excluded){
            graph[j] = i;
            j++;
        }
        i++;
    }
}

static void swapq8(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

static int shuffleq8(int n, int graph[n]){
    int i;
    int left;
    int right;
    int mid,diff,c=-1;

    i = n - 2; 
    while (i >= 0 && graph[i] >= graph[i+1]){
        i--;
    }
    if (i < 0){
        return 0;
    }    
    left = i +1;
    right = n -1;
    while (left <= right){
        diff = right - left;
        mid = left + (diff)/2;

        if (graph[mid] > graph[i]){
            left = mid +1;
            if ((c == -1) && (graph[c] >= graph[mid])){
                c = mid;
            }
        }
        else{
            right = mid - 1;
        }
    }

    swapq8(&graph[i],&graph[c]);

    left = i + 1;
    right = n - 1;
    while (left < right){
        left++;
        right--;
        swapq8(&graph[left],&graph[right]);
    }
    return 1;
}

static void computeDistanceq8(int n, int graph[n], const connection_t connections[n][n], int *distance, int *disconnected){
    *disconnected = 0;
    *distance = 0;

    for (int i = 0; i < n - 1; i++){
        if (connections[graph[i]][graph[(i+1)%(n-1)]].distance != INT_MAX){
            *distance += connections[graph[i]][graph[(i+1)%(n-1)]].distance;
        }
        else{
            *disconnected = 1;
            return;
        }
    }
}

static void writeTripq8(int n, int trip_order[n], int graph[n-1]){
    for (int i = 0; i < n-1; i++){
        trip_order[i]  = graph[i];
    }
}

static int getNextAdditionq9(int curr, int n, const connection_t connections[n][n], int edgeTime[n], int inMST[n]){
    int currentMin = INT_MAX;
    int next;
    for (int i = 0; i < n; i++){
        if ((!inMST[i]) && (connections[curr][i].time < currentMin)){
            currentMin = connections[curr][i].time;
            next = i;            
        }
    }
    // add it to the tree as it's the next min that is to be added
    inMST[next] = 1; 
    return next;
}

static void updateConnectionsq9(int curr, int next,int n, pair_t edges[n-1]){
    for (int i = 0; i < n; i++){
        if ( edges[i].first == -1){
            edges[i].first = curr;
            edges[i].second = next;
            return;
        }
    }
}

static void updateTimeq9(int next, int n, const connection_t connections[n][n], int edgeTime[n], int inMST[n], pair_t edges[n-1]){
    for (int i = 0; i < n; i++){
        // check if path available from "next" to a node and if that node is not in the tree already
        // also check if the time to a given node is lesser than from other nodes checked so far
        if ((connections[next][i].time < edgeTime[i]) && (connections[next][i].time != INT_MAX) && (!inMST[i])){
            edgeTime[i] = connections[next][i].time;
            updateConnectionsq9(next, i, n, edges);
        }

    }
}

static int getNextbestTimeq10(int curr ,int n, const connection_t connections[n][n], int bestTime[n], int included[n]){
    int minTime = INT_MAX;
    int next;

    for (int i = 0; i < n; i++){
        if ((connections[curr][i].time != INT_MAX) && (!included[i]) && (bestTime[i] <= minTime)){
            minTime = bestTime[i];
            next = i;
        }
    }
    included[next] = 1;
    return next;
}

static void updateTimeq10(int next, int n, const connection_t connections[n][n], int bestTime[n], int included[n]){
    for (int i = 0; i < n; i++){
        if ( (connections[next][i].time != INT_MAX) && (!included[i]) && (bestTime[next] != INT_MAX) && (connections[next][i].time + bestTime[next] < bestTime[i]))
            bestTime[i] = connections[next][i].time + bestTime[next];
    }
}

static void writeTimeq10(int n, int k, const int destination[k], int bestTime[n], int costs[k]){
    
    for (int i = 0; i < k; i++){
        costs[i] = bestTime[destination[i]];
    }
    
}

// YOUR SOLUTIONS BELOW

int q1(int n, const connection_t connections[n][n]){
    int visited[n];
    for (int i = 0; i < n; i++){
        visited[i]=0;
    }
    dfsq1(n, connections, 0, visited);
    for (int i = 0; i < n; i++){
        if (visited[i]==0){
            return 0;
        }
    }
    for (int i = 0; i < n; i++){
        visited[i]=0;
    }
    getTransposeq1(n, connections, 0, visited);   
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
    int flag = 0;
    int visited[n];
    for (int i = 0; i < n; i++){
        visited[i] = 0;
    }
    dfsq2(src->num_id, src, dest, n, k, connections, visited, &flag);
    return flag;
}

int q3(const airport_t *src, int n, const connection_t connections[n][n])
{
    int flag = 0;
    int visited[n];
    for (int i = 0; i < n; i++){
        visited[i] = 0;    
    }
    dfsq3(src->num_id, src, n, connections, visited, &flag);
    return flag;
}

void q4(int n, int (*predicate_func)(const airport_t *, const airport_t *),
        airport_t airport_list[n])
{
    buildHeapq4(n, predicate_func, airport_list);

    // heap sort
    for (int i = n - 1; i >= 0; i--){
        swapq4(&airport_list[0],&airport_list[i]);

        // heapifyq4 again
        heapifyq4(0, i, predicate_func, airport_list);
    }
}

pair_t q5(int n, airport_t airports[n])
{
    pair_t ans = {-1, -1};
    heapSortq5(n, airports);

    ans = findLargestMatchq5(n, airports);

    return ans;
}

int q6(int n, int amount, const int entry_fee[n])
{
    int flag = 0;
    binSearchq6(0, n-1, &flag, n, amount, entry_fee);
    return flag;
}

void q7(int n, const char *pat, int contains[n], const airport_t airports[n])
{
    int patternLen = getPatterLenq7(pat);
    int shiftTable[ASCIIq7];
    buildShiftTableq7(patternLen, pat, shiftTable);

    int j;
    int k;
    int strl; // length of parent string
    int flag;

    for (int i = 0; i < n; i++){
        flag = 0;
        strl = getPatterLenq7(airports[i].airport_name);
        j = patternLen - 1;
        while (j < strl && flag==0){
            k = 0;

            while ((k < patternLen) && (pat[patternLen-1-k] == airports[i].airport_name[j-k]))
                k++;

            if (k == patternLen){
                contains[airports[i].num_id] = 1;
                flag = 1;
            }
            else
                j += shiftTable[(int) airports[i].airport_name[j]];
        }
    }
}

int q8(int n, int trip_order[n - 1], const connection_t connections[n][n])
{
    int shortest = INT_MAX;
    int graph[n-1];
    int flag;
    int disconnected;  // set to 1 if a disconnection detected in graph
    int distance;       // compute and store distances in graph

    for (int excluded = 0; excluded < n; ++excluded){
        generateNodesq8(n, excluded, graph);
        flag = 1;

        while(flag){
            computeDistanceq8(n, graph, connections, &distance, &disconnected);

            if ((distance < shortest) && (!disconnected)){
                shortest = distance;
                writeTripq8(n, trip_order, graph);
            }

            flag = shuffleq8(n - 1, graph);
        }

    }
    
    if (shortest == INT_MAX)
        shortest = -1;


    return shortest;
}

int q9(int n, pair_t edges[n - 1], const connection_t connections[n][n])
{
    // tracks nodes that have been added to the mst
    int inMST[n];  // initially all are zero as none belong in MST, stores 0 or 1
    // stores the minimum weight required to visit the node
    int edgeTime[n];  // initialized to INT_MAX initially
    int next;
    for (int i = 0; i < n; i++){
        edgeTime[i] = INT_MAX;
        inMST[i] = 0;
    }

    // starting with 0th index in connections as root so setting it's edgeTime to 0
    edgeTime[0] = 0;
    // 0th index is in MST
    inMST[0] = 1;

    for (int i = 0; i < n; i++){
        // get the next node to be added
        next = getNextAdditionq9(i, n, connections, edgeTime, inMST);
        updateConnectionsq9(i, next, n, edges);
        // update time for all keys based on the "next" node
        updateTimeq9(next, n, connections, edgeTime, inMST, edges);
    }

    int minTime = 0;

    for (int i = 0; i < n-1; i++){
        minTime += connections[edges[i].first][edges[i].second].time; 
    }


    // solution requires prims minimum spanning tree
    return minTime;

}

void q10(int n, int k, const airport_t *src,
         const connection_t connections[n][n], const int destinations[k],
         int costs[k])
{
    int bestTime[n];     // holds the least time a node has from source node
    int included[n];    // if it is already present in the shortest path, stores 0 or 1

    for (int i = 0; i < n; i++){
        bestTime[i] = INT_MAX;
        included[i] = 0;
    }   

    bestTime[src->num_id] = 0;   // setting the best time of the source vertex to 0
    

    for (int i = 0; i < n - 1; i++){
        int next = getNextbestTimeq10(src->num_id, n, connections, bestTime, included);

        updateTimeq10(next, n, connections, bestTime, included);
    }

    writeTimeq10(n, k, destinations, bestTime, costs);
}   

// END