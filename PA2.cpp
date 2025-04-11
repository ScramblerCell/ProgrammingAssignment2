#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <chrono>
#include <thread>
#include <iomanip>

using namespace std;

const char DELIMITER = ',';
const int NUM_NODES = 26;


vector<int> stripNonNums(const string& line) {
    size_t delimPos = line.find(DELIMITER);

    // Strip prefix "N_" and convert both parts
    string left = line.substr(2, delimPos - 2);
    string right = line.substr(delimPos + 3);  
    //convert to ints
    int leftInt = stoi(left);
    int rightInt = stoi(right);
    
    vector<int> result;
    //add to result vector
    result.push_back(leftInt);
    result.push_back(rightInt);
    return result;
}
void TEST_printConnections(vector<int> adj[26]) {
    cout << endl<< endl<< endl;
    for (int i = 0; i < NUM_NODES; i++) {
        cout << i << ": ";
        for (int node : adj[i]) {
            cout << node << " ";
        }
        cout << endl;
    }
}
void DFS(int start, int target, vector<int> adj[], vector<bool>& isVisited, int& currDist) {
    //init start node as visited
    isVisited.at(start) = true;
    if (start == target) return;
    currDist++;


    for (auto neighb : adj[start]) {//for each neighbor of start
        if (!isVisited.at(neighb)) {//if neighbor hasn't been visited
            DFS(neighb, target, adj, isVisited, currDist);//do dfs on it
        }
    }
}
pair<int, double> trackDFS(int start, int target, vector<int> adj[]) {
    int currDist = 0;
    double currTime_ms = 0;
    //init visited vector
    vector<bool> isVisited(NUM_NODES, false);

    //start timer
    auto start_time = std::chrono::high_resolution_clock::now();
    
    //run dfs to get distance
    DFS(start, target, adj, isVisited, currDist);
    
    //end timeer
    auto current_time = std::chrono::high_resolution_clock::now();
    currTime_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time).count();

    pair<int, double> trackedData;
    trackedData.first = currDist;
    trackedData.second = currTime_ms;
    //cout << "distance: " << trackedData.first << endl << "time: " << trackedData.second << endl;

    return trackedData;
}
//print functions
void printLine() {
    cout << setfill('-') << setw(78) << "" << setfill(' ') << "" << endl;
}
void printHeader() {
    cout << endl << endl << endl << endl << endl;
    printLine();
    cout << right << setw(8) << "Node 1" << setw(8) << "Node 2" << setw(15) << "BFS_Distance" << setw(15) << "BFS_Time(ns)" << setw(15) << "DFS_Distance" <<  setw(15) <<"DFS_time(ns)" << endl;
    printLine();
}
void printRun(int row, int DFS_dist, double DFS_time, int BFS_dist, double BFS_time) {
    cout << right << setw(8) << "N_0" << setw(8) << "N_"+to_string(row) << setw(15) << BFS_dist << setw(15) << BFS_time << setw(15) << DFS_dist <<  setw(15) << DFS_time << endl;
}
int main() {
    string testLine;
    vector<int> strippedInts;
    ifstream csvFile("Test_Case_Assignment2.txt");
    vector<int> adj[NUM_NODES]; 

    //read and write csvFile into graph
    while (getline(csvFile, testLine)) {
        strippedInts =  stripNonNums(testLine);
        adj[strippedInts[0]].push_back(strippedInts[1]);
    }
    printHeader();
    //do dfs from 0 to 9 for testing
    for (int i = 1; i < NUM_NODES-1; i++) {
        pair<int, double> DFS_data = trackDFS(0, i, adj);

        printRun(i, DFS_data.first, DFS_data.second, 0, 0);
    }
    printLine();
    
    //cout << "took " << DFS_data.second << "ns long, traversing " << DFS_data.first << " nodes" << endl;
    //perform BFS starting at N0 to N#



    //TEST_printConnections(adj);

    return 1;
}