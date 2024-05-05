#include "search.hpp"

// Base Class

std::pair<int, int> Search::indexInFinal(int num) {
    std::pair<int, int> f = {-1, -1};
    std::vector<std::vector<int>> goalState = problem->getGoalState().getData();
    for(int y = 0; y<goalState.size(); y++) {
        for(int x = 0; x<goalState[0].size(); x++) {
            if (goalState[x][y] == num) {
                f.first = x;
                f.second = y;
                return f;
            }
        }
    } return f;
}

int Search::getTotalNodes() const {
    return totalNodes;
}

int Search::getMaxQueue() const {
    return maxQueue;
}

int Search::getSolutionDepth() const {
    return solutionDepth;
}

std::optional<State> Search::doSearch() {
    State start = problem->getStartState();
    std::priority_queue<State, std::vector<State>, std::greater<State>> pqueue;
    pqueue.push(problem->getStartState());
    while (!pqueue.empty()) {
        int size = pqueue.size();
        maxQueue = std::max(size, maxQueue);
        State currState = pqueue.top();    
        //heuristic = cost - depth
        int h = currState.getCost() - currState.getDepth();                                                
        std::cout << std::endl << "The best state to expand with g(n) = " << currState.getDepth() << " and h(n) = " << h << " is " << std::endl;
        currState.displayState();
        pqueue.pop();
        if (problem->isGoal(currState)) {
            solutionDepth = currState.getDepth();
            return currState;
        }
        std::vector<State> expanded = problem->expand(currState);
        totalNodes += expanded.size();
        for (State nextState : expanded) {
            nextState.setCost(nextState.getDepth() + heuristic(nextState));
            pqueue.push(nextState);
        }
    }
    return std::nullopt;
}

int Search::heuristic(const State& currentState) {
    return 0;
}

// Uniform

int UniformCostSearch::heuristic(const State& currentState) {
    return 0;
}

// Euclidean

int EuclideanDistanceSearch::heuristic(const State& currentState) {
    int totalHeuristic = 0;
    std::vector<std::vector<int>> goalState = problem->getGoalState().getData();
    for(int y = 0; y < goalState.size(); y++){
        for(int x = 0; x< goalState[y].size(); x++){
            int value = currentState.getData()[y][x];
            if(value != 0){
                std::pair coordsFinal = indexInFinal(value);
                std::pair coordsInitial = std::make_pair(y,x);
                totalHeuristic += calculateEuclidean(coordsFinal, coordsInitial);
            }
        }
    }
    return totalHeuristic;
}

int EuclideanDistanceSearch::calculateEuclidean(std::pair<int,int> pos1, std::pair<int,int> pos2){
    return sqrt(pow((pos2.first-pos1.first),2) + pow((pos2.second-pos1.second),2));
    //sqrt((x2-x1)^2+(y2-y1)^2)
}

// Misplaced Tile 

int MisplacedTileInformedSearch::heuristic(const State& c) {
    int totalHeuristic = 0;
    std::vector<std::vector<int>> goalState = problem->getGoalState().getData();
    std::vector<std::vector<int>> currentState = c.getData();
    for(int y = 0; y<currentState.size(); y++) {
        for(int x = 0; x<currentState[0].size(); x++) {
            if(currentState[y][x] != goalState[y][x]) totalHeuristic++;
            
        }
    }
    // std::cout << "      state heuristic of " << totalHeuristic << std::endl;
    return totalHeuristic;
}



// Manhattan 

int ManhattanInformedSearch::heuristic(const State& c) {
    int totalHeuristic = 0;
    std::vector<std::vector<int>> goalState = problem->getGoalState().getData();
    std::vector<std::vector<int>> currentState = c.getData();
    for(int y = 0; y<currentState.size(); y++) {
        for(int x = 0; x<currentState[0].size(); x++) {
            // Run distance test on every tile
            int value = currentState[x][y];
            std::pair coordsFinal = indexInFinal(value);
            // std::cout << "  Calculating tile " << value << std::endl;
            totalHeuristic += calculateManhattan(std::make_pair(x, y), coordsFinal);
            
        }
    }
    // std::cout << "      state heuristic of " << totalHeuristic << std::endl;
    return totalHeuristic;
}



int ManhattanInformedSearch::calculateManhattan(std::pair<int, int> value1, std::pair<int, int> value2) {
    // value1 --> current tile placement
    // value2 --> goal tile placement
    
    // std::cout << "       Finding distance between " << "(" << value1.second <<" , " << value1.first << ") and "  << "(" << value2.second << ", " << value2.first << ")" << std::endl;
    int dx = std::abs(value1.first - value2.first);
    int dy = std::abs(value1.second - value2.second);
    // std::cout << "       dx: " << dx << std::endl;
    // std::cout << "       dy: " << dy << std::endl;
    // std::cout << "       tile heuristic " << x << std::endl;
    return dx+dy;
}







