/**
 * @file PA2.cpp
 * @author Jonah Downing
 * @brief reads csv file FILE_TO_READ and creates statistic table using 
 * @date 2025-04-11
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <chrono>
#include <iomanip>
#include <queue>

using namespace std;

const char DELIMITER = ',';
const int NUM_NODES = 26;
const string FILE_TO_READ = "Test_Case_Assignment2.txt";


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
void updateDistOfDFS(int start, int target, vector<int> adj[], vector<bool>& isVisited, int& currDist, bool& found) {
    if (found) return;
    
    //init start node as visited
    isVisited.at(start) = true;
    if (start == target) {
        found = true;
        return;
    }   
    currDist++;
    //depth first search 
    for (auto neighb : adj[start]) {//for each neighbor of start
        if (!isVisited.at(neighb)) {//if neighbor hasn't been visited
            updateDistOfDFS(neighb, target, adj, isVisited, currDist, found);//do dfs on it
        }
    }
}
void updateDistOfBFS(int start, int target, vector<int> adj[], vector<bool>& isVisited, int& currDist) {
    queue<pair<int, int> > bfsQueue; // pair<node, distance>
    bfsQueue.push(make_pair(start, 0));
    isVisited[start] = true;

    while (!bfsQueue.empty()) {
        int currNode = bfsQueue.front().first;
        int currLevel = bfsQueue.front().second;
        bfsQueue.pop();
        //quit if target found
        if (currNode == target) {
            currDist = currLevel;
            return;
        }

        for (int neighb : adj[currNode]) {
            if (!isVisited[neighb]) {
                isVisited[neighb] = true;
                bfsQueue.push(make_pair(neighb, currLevel + 1));
            }
        }
    }

    currDist = -1; // if unreachable
}
pair<int, double> trackSearchAlgo(bool isDFS,int start, int target, vector<int> adj[]) {
    int currDist = 0;
    double currTime_ms = 0;
    //init visited vector
    vector<bool> isVisited(NUM_NODES, false);

    //start timer
    auto start_time = std::chrono::high_resolution_clock::now();
    
    //run search algo to get distance
    bool found = false;
    if (isDFS) updateDistOfDFS(start, target, adj, isVisited, currDist, found);
    else updateDistOfBFS(start, target, adj, isVisited, currDist);
    
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
    ifstream csvFile(FILE_TO_READ);
    vector<int> adj[NUM_NODES]; 

    //read and write csvFile into graph
    while (getline(csvFile, testLine)) {
        strippedInts =  stripNonNums(testLine);
        adj[strippedInts[0]].push_back(strippedInts[1]);
    }
    printHeader();
    //do dfs from 0 to 9 for testing
    for (int i = 1; i < NUM_NODES-1; i++) {
        pair<int, double> DFS_data = trackSearchAlgo(true, 0, i, adj);
        pair<int, double> BFS_data = trackSearchAlgo(false, 0, i, adj);
        printRun(i, DFS_data.first, DFS_data.second, BFS_data.first, BFS_data.second);
    }
    printLine();

    //TEST_printConnections(adj);

    return 1;
}