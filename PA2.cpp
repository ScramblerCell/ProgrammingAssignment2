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

/**
 * @brief Extracts two integers from a line in the format "N_#,N_#"
 *
 * @param line A string in the format "N_#,N_#"
 * @return vector<int> A vector containing the two neighboring node values as integers.
 */
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
/**
 * @brief Prints the adjacency list representation of the graph.
 *
 * Used for debugging to display which nodes are connected to each other.
 *
 * @param adjacencyList The adjacency list for all NUM_NODES nodes.
 */
void TEST_printConnections(vector<int> adjacencyList[NUM_NODES]) {
    cout << endl << endl << endl;
    for (int i = 0; i < NUM_NODES; i++) {
        cout << i << ": ";
        for (int node : adjacencyList[i]) {
            cout << node << " ";
        }
        cout << endl;
    }
}


/**
 * @brief Performs a DFS traversal to track how many nodes are visited before the target is found.
 *
 * Traverses deeply through the graph from the start node. Increments currDist for each node visited.
 * Exits early if the target is found.
 *
 * @param start        The starting node.
 * @param target       The node to search for.
 * @param adjacencyList The graph's adjacency list.
 * @param isVisited    A reference to the visited status vector.
 * @param currDist     A reference to the distance counter (number of nodes visited).
 * @param found        A flag to indicate whether the target was found.
 */
void updateDistOfDFS(int start, int target, vector<int> adjacencyList[], vector<bool>& isVisited, int& currDist, bool& found) {
    if (found) return;

    isVisited.at(start) = true;
    if (start == target) {
        found = true;
        return;
    }

    currDist++;

    for (auto neighb : adjacencyList[start]) {
        if (!isVisited.at(neighb)) {
            updateDistOfDFS(neighb, target, adjacencyList, isVisited, currDist, found);
        }
    }
}


/**
 * @brief Performs a BFS traversal to track how many nodes are visited before the target is found.
 *
 * Explores the graph level-by-level using a queue. Increments currDist for each node visited.
 * Exits early if the target is found.
 *
 * @param start        The starting node.
 * @param target       The node to search for.
 * @param adjacencyList The graph's adjacency list.
 * @param isVisited    A reference to the visited status vector.
 * @param currDist     A reference to the distance counter (number of nodes visited).
 */
void updateDistOfBFS(int start, int target, vector<int> adjacencyList[], vector<bool>& isVisited, int& currDist) {
    queue<int> bfsQueue;
    bfsQueue.push(start);
    isVisited[start] = true;
    currDist = 1;

    while (!bfsQueue.empty()) {
        int currNode = bfsQueue.front();
        bfsQueue.pop();

        if (currNode == target) return;

        for (int neighb : adjacencyList[currNode]) {
            if (!isVisited[neighb]) {
                isVisited[neighb] = true;
                bfsQueue.push(neighb);
                if (neighb == target) return;
                currDist++;
            }
        }
    }
}


/**
 * @brief Runs either BFS or DFS to determine traversal time and node count to reach the target.
 *
 * This function acts as a wrapper for BFS and DFS distance tracking functions.
 * It measures execution time and returns both the number of visited nodes and the elapsed time.
 *
 * @param isDFS         If true, uses DFS; otherwise, uses BFS.
 * @param start         The starting node.
 * @param target        The target node.
 * @param adjacencyList The graph's adjacency list.
 * @return pair<int, double> A pair of the number of visited nodes and the time taken in nanoseconds.
 */
pair<int, double> trackSearchAlgo(bool isDFS, int start, int target, vector<int> adjacencyList[]) {
    int currDist = 0;
    double currTime_ms = 0;
    vector<bool> isVisited(NUM_NODES, false);

    auto start_time = std::chrono::high_resolution_clock::now();

    bool found = false;
    if (isDFS) updateDistOfDFS(start, target, adjacencyList, isVisited, currDist, found);
    else updateDistOfBFS(start, target, adjacencyList, isVisited, currDist);

    auto current_time = std::chrono::high_resolution_clock::now();
    currTime_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time).count();

    return make_pair(currDist, currTime_ms);
}


/**
 * @brief Prints a horizontal line separator for table formatting.
 */
void printLine() {
    cout << setfill('-') << setw(78) << "" << setfill(' ') << "" << endl;
}


/**
 * @brief Prints the header row for the BFS vs DFS comparison table.
 */
void printHeader() {
    printLine();
    cout << right << setw(8) << "Node 1"
         << setw(8) << "Node 2"
         << setw(15) << "BFS_Distance"
         << setw(15) << "BFS_Time(ns)"
         << setw(15) << "DFS_Distance"
         << setw(15) << "DFS_time(ns)" << endl;
    printLine();
}


/**
 * @brief Prints a formatted row of data showing DFS and BFS results for a given target node.
 *
 * @param row        The numeric value of the target node (e.g., 1 for N_1).
 * @param DFS_dist   Number of nodes visited by DFS before reaching the target.
 * @param DFS_time   Time taken by DFS in nanoseconds.
 * @param BFS_dist   Number of nodes visited by BFS before reaching the target.
 * @param BFS_time   Time taken by BFS in nanoseconds.
 */
void printRun(int row, int DFS_dist, double DFS_time, int BFS_dist, double BFS_time) {
    cout << right << setw(8) << "N_0"
         << setw(8) << "N_" + to_string(row)
         << setw(15) << BFS_dist
         << setw(15) << BFS_time
         << setw(15) << DFS_dist
         << setw(15) << DFS_time << endl;
}


int main() {
    vector<int> adjacencyList[NUM_NODES];
    string testLine;
    vector<int> strippedInts;
    ifstream csvFile(FILE_TO_READ); 
    //read and write csvFile into graph
    while (getline(csvFile, testLine)) {
        strippedInts =  stripNonNums(testLine);
        adjacencyList[strippedInts[0]].push_back(strippedInts[1]);
    }
   
    printHeader();
    //do search from 0 to all nodes in adjacency list
    for (int i = 1; i < NUM_NODES-1; i++) {
        pair<int, double> DFS_data = trackSearchAlgo(true, 0, i, adjacencyList);
        pair<int, double> BFS_data = trackSearchAlgo(false, 0, i, adjacencyList);
        printRun(i, DFS_data.first, DFS_data.second, BFS_data.first, BFS_data.second);
    }
    printLine();

    //TEST_printConnections(adjacencyList);
}