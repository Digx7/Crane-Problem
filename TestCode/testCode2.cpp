#include <iostream>
#include <string>
#include <queue>

void generate_permutations_iterative(int N) {
    std::queue<std::string> permutations;
    permutations.push(""); // Start with an empty string
    
    while (!permutations.empty()) {
        std::string current = permutations.front();
        permutations.pop();
        
        int length = current.size();
        
        // Print the current permutation if its length is greater than 0
        if (length > 0) {
            std::cout << current << std::endl;
        }
        
        // Add 'L' and 'D' to the current permutation and enqueue the new permutations
        if (length < N) {
            permutations.push(current + 'L');
            permutations.push(current + 'D');
        }
    }
}

int main() {
    int N = 3; // Maximum length of the permutations
    generate_permutations_iterative(N);
    
    return 0;
}





