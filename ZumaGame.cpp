//APPROACH
//Use BFS

/*(1) Breadth-First Search (BFS): Use a BFS to explore all possible states (board configurations) level by level, ensuring that the first time an empty board is found, it's with the minimum number of steps.


(2) State Representation: Each state is represented by the current board and the remaining hand, combined into a single string to track visited states.


(3) State Reduction: After each insertion, remove any consecutive balls of three or more using a helper function (e.g., removeConsecutiveTriplets) to reduce the board.


(4)Pruning: Use a set to track visited states to avoid redundant computations and improve efficiency.

(5)Early Termination: If an empty board is found, return the number of steps taken to reach it.


     Time complexity:

    O((B×H)×2H)

     
      Space complexity:

      

      O(B×2H)
*/

class Solution {
public:
    string removeConsecutiveTriplets(string str) {
        bool changed;
        do {
            changed = false;
            int n = str.length();
            string temp;

            for (int i = 0; i < n;) {
                int j = i;
                while (j < n && str[j] == str[i]) j++;
                if (j - i >= 3) {
                    changed = true;
                } else {
                    temp.append(str.substr(i, j - i));
                }
                i = j;
            }

            str = temp;
        } while (changed);

        return str;
    }

    int findMinStep(string board, string hand) {
        sort(hand.begin(), hand.end());

        queue<pair<string, string>> boardQueue;
        queue<int> stepQueue;
        unordered_set<string> visited;

        boardQueue.push({board, hand});
        stepQueue.push(0);
        visited.insert(board + "+" + hand);

        while (!boardQueue.empty()) {
            auto [curBoard, curHand] = boardQueue.front();
            int curStep = stepQueue.front();
            boardQueue.pop();
            stepQueue.pop();

            for (int i = 0; i <= curBoard.size(); ++i) {
                for (int j = 0; j < curHand.size(); ++j) {
                    if (i > 0 && curBoard[i - 1] == curHand[j]) continue;

                    bool shouldTry = (i < curBoard.size() && curBoard[i] == curHand[j]) ||
                                     (i > 0 && curBoard[i - 1] == curBoard[i] && curBoard[i] != curHand[j]);

                    if (shouldTry) {
                        string newBoard = removeConsecutiveTriplets(curBoard.substr(0, i) + curHand[j] + curBoard.substr(i));
                        string newHand = curHand.substr(0, j) + curHand.substr(j + 1);

                        if (newBoard.empty()) return curStep + 1;

                        string stateKey = newBoard + "+" + newHand;
                        if (visited.insert(stateKey).second) {
                            boardQueue.push({newBoard, newHand});
                            stepQueue.push(curStep + 1);
                        }
                    }
                }
            }
        }

        return -1;
    }
};

//2nd code

class Solution {
public:
   int findMinStep(string board, string hand) {
	vector<int> freq(26, 0);
	for(char c: hand)
		freq[c - 'A']++;
	unordered_map<string, int> cache;
	return dfs(board, freq, cache);
}

int dfs(string board, vector<int>& freq, unordered_map<string, int>& cache) {
    string key = board + "#" + serialize(freq);
	if(cache.count(key)) {
        return cache[key];
    }
	int r = INT_MAX;
	if(board.length() == 0) {           
		r = 0;
	} else {
		for(int i = 0; i < board.length(); i++) {    
			for(int j = 0; j < freq.size(); j++) {     
                bool worthTrying = false;
                if(board[i] - 'A' == j)
                    worthTrying = true;
                else if(0 < i && board[i] == board[i - 1] && board[i] - 'A' != j) 
                    worthTrying = true;
                    
				if(freq[j] > 0 && worthTrying) {     
					board.insert(board.begin() + i, j + 'A');  
					freq[j]--;   

					string newStr = updateBoard(board);  
					int steps = dfs(newStr, freq, cache);   
					if(steps != -1) {   
						r = min(r, steps + 1);
					}

					freq[j]++; 
					board.erase(board.begin() + i);   
				}
			}
		}
	}
	if(r == INT_MAX) r = -1; 
	cache[key] = r;
	return r;
}

string updateBoard(string board) {
	 int start = 0;
	 int end = 0; 
	 int boardLen = board.length();
	 while(start < boardLen) {
		 while(end < boardLen && board[start] == board[end]) {
			 end++;  
		 }
		
		 if(end - start >= 3) {  
			 string newBoard = board.substr(0, start) + board.substr(end); 
			 return updateBoard(newBoard);    
		 } else {
			 start = end;
		 }
	 }
	 return board;
}

string serialize(vector<int>& freq) {
    string key = "";
    for(int i = 0; i < freq.size(); i++) {
        if(freq[i] > 0)
        key += to_string((char) i + 'A') + to_string(freq[i]);
    }
    return key;
}  
};
