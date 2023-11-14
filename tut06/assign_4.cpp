#include <fstream>
#include <iostream>
#include <map>
#include <pthread.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// Define a struct to represent a link between nodes with a cost.
struct Link {
    string node1;
    string node2;
    int cost;
};

int N; // Number of routers

// Declare data structures
vector<pthread_t> threads;
vector<vector<pair<int, int>> > graph;
vector<vector<int>> matrix;
vector<vector<int>> matrix2;
vector<pthread_mutex_t> mutex;
vector<int> complete;
pthread_mutex_t print = PTHREAD_MUTEX_INITIALIZER;
vector<string> routerNames;

// Function to check if the graph is connected using BFS
bool isConnected(vector<vector<pair<int, int>>>& graph, int N) {
    vector<bool> visited(N, false);
    queue<int> q;

    // Start from the first node.
    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (auto neighbor : graph[node]) {
            int nextNode = neighbor.first;
            if (!visited[nextNode]) {
                visited[nextNode] = true;
                q.push(nextNode);
            }
        }
    }

    // Check if all nodes were visited.
    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            return false; // Graph is disconnected.
        }
    }
    return true; // Graph is connected.
}

// Function to update the distance matrix
void update(int threadId) {
    for (auto i : graph[threadId]) {
        int node = i.first;
        pthread_mutex_lock(&mutex[node]);
        for (int j = 0; j < N; j++) {
            matrix[node][j] = min(matrix[node][j], matrix[node][threadId] + matrix[threadId][j]);
        }
        pthread_mutex_unlock(&mutex[node]);
    }
    complete[threadId]++;
}

// Function to print the distance matrix

// void printMatrix(int iteration) {
//     pthread_mutex_lock(&print);
//     cout << "Iteration " << iteration << ":\n";
//     // cout << "\t";
//     for (int i = 0; i < N; i++) {
//         cout << "\t " << routerNames[i];
//     }
//     cout << "\n";
//     cout<<"---------------------------------------\n";
//     for (int i = 0; i < N; i++) {
//         cout<< routerNames[i] <<":"<< "\t";
//         for (int j = 0; j < N; j++) {
//             cout << matrix[i][j] << "\t";
//         }
//         cout << "\n";
//     }
//     cout<<"---------------------------------------\n";
//     pthread_mutex_unlock(&print);
// }

void printMatrix(int iteration) {
    pthread_mutex_lock(&print);
    cout << "Iteration " << iteration << ":\n";
    // cout << "\t";
    for (int i = 0; i < N; i++) {
        cout << "\t " << routerNames[i];
    }
    cout << "\n";
    cout<<"---------------------------------------\n";
    for (int i = 0; i < N; i++) {
        cout<< routerNames[i] <<":"<< "\t";
        for (int j = 0; j < N; j++) {

            if(matrix[i][j]!=1e8)
            {

            if(matrix[i][j]>=matrix2[i][j])
                cout << matrix[i][j] << "\t";
            else cout << "* "<<matrix[i][j] << "\t";
            }
            else wprintf (L"%lc\t", 8734);
        }
        cout << "\n";
    }
    cout<<"---------------------------------------\n";
    pthread_mutex_unlock(&print);
}

// Function to initialize each thread
void* init(void* arg) {
    int threadId = *((int*)arg);
    pthread_mutex_lock(&mutex[threadId]);

    // Initialize the distance matrix based on neighbor information
    for (int i = 0; i < graph[threadId].size(); i++) {
        int node = graph[threadId][i].first;
        int dist = graph[threadId][i].second;
        matrix[threadId][node] = dist;
    }

    complete[threadId]++;
    
    // Wait for all threads to reach this point
    for (int j = 0; j < N; j++) {
        if (complete[j] < 1)
            j = 0;
    }


        if(threadId==0)
        printMatrix(0);
        pthread_mutex_unlock(&mutex[threadId]);

        // Iteratively update the distance matrix

        for(int k1=0;k1<N;k1++){
                for(int k2=0;k2<N;k2++){
                    matrix2[k1][k2] = matrix[k1][k2];
                }
            }

        for (int i = 0; i < N; i++) {

            for(int k1=0;k1<N;k1++){
                for(int k2=0;k2<N;k2++){
                    matrix2[k1][k2] = matrix[k1][k2];
                }
            }

            update(threadId);
            for (int j = 0; j < N; j++) {
                if (complete[j] < i + 2)
                    j = 0;
            }
            if(threadId==0)
                printMatrix(i + 1);
            sleep(1);
        }
}

int main() {

     setlocale( LC_ALL, "en_US.UTF-8" );
    
    // Read the input from the "topology.txt" file
    ifstream inputFile("topology.txt");
    if (!inputFile.is_open()) {
        cout << "Failed to is_open the file." << endl;
        return 1;
    }

    // Read the number of routers from the file.
    inputFile >> N;

    // Initialize data structures
    threads.resize(N);
    graph.resize(N);
    matrix.resize(N, vector<int>(N, 1e8));
    matrix2.resize(N, vector<int>(N, 1e8));
    mutex.resize(N, PTHREAD_MUTEX_INITIALIZER);
    complete.resize(N, 0);
    routerNames.resize(N);

    for(int i=0;i<N;i++){   
        matrix[i][i] = 0;
    }

    // Read router names
    
    for (int i = 0; i < N; ++i) {
        inputFile >> routerNames[i];
    }

    // Read links and build the graph
    vector<Link> links;
    string node1, node2;
    int cost;
    while (inputFile >> node1 >> node2 >> cost) {
        int node1Index = find(routerNames.begin(), routerNames.end(), node1) - routerNames.begin();
        int node2Index = find(routerNames.begin(), routerNames.end(), node2) - routerNames.begin();
        graph[node1Index].push_back({ node2Index, cost });
        graph[node2Index].push_back({ node1Index, cost });
    }

    inputFile.close();

    // Check if the network is connected
    if (!isConnected(graph, N)) {
        cout << "The input topology is disconnected." << endl;
        return 1;
    }

    // Create threads and start the network routing process
    int arr[N];
    for (int i = 0; i < N; i++) {
        arr[i] = i;
        pthread_create(&threads[i], NULL, init, &arr[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // Clean up mutexes
    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(&mutex[i]);
    }

    return 0;
}
