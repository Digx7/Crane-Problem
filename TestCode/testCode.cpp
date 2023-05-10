#include <iostream>
#include <string>

void generate_permutations(int variable_length, int N, std::string prefix = "") 
{
    if (variable_length > N) 
    {
        return;
    }
    
    if (variable_length > 0) 
    {
        std::cout << prefix << std::endl;
    }
    
    for (char element : {'L', 'D'}) 
    {
        generate_permutations(variable_length + 1, N, prefix + element);
    }
}

int main() 
{
    int iteration = 0;
    int N = 3; // Maximum length of the permutations
    generate_permutations(0, N);

    return 0;
}