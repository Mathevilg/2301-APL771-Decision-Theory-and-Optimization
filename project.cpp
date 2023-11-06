#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <random>
using namespace std;


// given the state of board, return number of non-attacking pair of queens
int fitness_function(vector<int> board) {
    int eval = 0;
    for (int i=0; i<board.size(); i++) {
        int col = board[i];
        for (int j=i+1; j<board.size(); j++) {
            if ((board[j]==col+j-i) || (board[j]==col-j+i) || board[j]==col) eval++;
        }
    }
    return (board.size()*(board.size()-1))/2 - eval;
}

pair<vector<int>, vector<int> > crossover(const vector<int>& parent1, const vector<int>& parent2) {
    // Assuming parent1 and parent2 have the same size
    int n = parent1.size();

    // Choose a random crossover point
    int crossover_point = rand() % n;

    // Create the first offspring by combining the first part of parent1 and the second part of parent2
    vector<int> offspring1(parent1.begin(), parent1.begin() + crossover_point);
    offspring1.insert(offspring1.end(), parent2.begin() + crossover_point, parent2.end());

    // Create the second offspring by combining the first part of parent2 and the second part of parent1
    vector<int> offspring2(parent2.begin(), parent2.begin() + crossover_point);
    offspring2.insert(offspring2.end(), parent1.begin() + crossover_point, parent1.end());

    return make_pair(offspring1, offspring2);
}

void print_board(vector<int> board) {
    for (int _=0; _<board.size(); _++) cout << "__";
    cout <<"_\n";
    for (int row=0; row<board.size(); row++){
        for (int col=1; col<=board.size(); col++) {
            if (col == board[row]) cout << "|x";
            else cout << "|_";
        }
        cout << "|\n";
    }
    cout << "\n\n";
}


int main() {

    std::random_device rd;  // Create a random device to seed the engine
    std::mt19937 gen(rd()); // Create a Mersenne Twister random number engine
    // Create a uniform distribution for floating-point numbers between 0 and 1
    std::uniform_real_distribution<float> dis(0.0, 1.0);


    int population_size = 8;
    int board_size = 10;

    // initializing initial population
    vector<vector<int> > initial_population;
    for (int _=0; _<population_size; _++){
        vector<int> board_sample;
        for (int _=0; _<board_size; _++){
            board_sample.push_back((rand()%board_size) + 1);
        }
        initial_population.push_back(board_sample);
    }

    vector<int> population_fitness(population_size, 0);
    vector<vector<int> > selected_population = initial_population;
    vector<vector<int> > offsprings = initial_population;
    vector<vector<int> > mutated_offsprings = initial_population;


    vector<int> solution;
    int iter = 0;
    while (iter < 5000000) {
        // for (auto b : initial_population) print_board(b);
        // evaluating population fitness
        bool found_solution = false;
        int cumulative_fitness=0;
        for (int i=0; i<population_size; i++){
            int fitness = fitness_function(initial_population[i]);
            // cout << fitness << "\n";
            // print_board(initial_population[i]);
            // cout << "\n";
            if (fitness==(board_size*(board_size-1))/2) {
                found_solution=true;
                solution = initial_population[i];
                break;
            }
            cumulative_fitness += fitness;
            population_fitness[i] = cumulative_fitness;
        }

        if (found_solution) break;

        // selection
        for (int i=0; i<population_size; i++){
            int rand_sample = rand()%cumulative_fitness;
            for (int j=0; j<population_size; j++){
                if (rand_sample<population_fitness[j]){
                    selected_population[i]=initial_population[j];
                    break;
                }
            }
        }

        // cross-over
        for (int i=0; i<population_size; i+=2){
            pair<vector<int>, vector<int> > children = crossover(selected_population[i],selected_population[i+1]);
            offsprings[i]=children.first;
            offsprings[i+1]=children.second;
        }

        // mutation 
        for (int i=0; i<population_size; i++) {
            if (dis(gen)>0.5){
                int random_row = rand()%board_size;
                int new_col = rand()%board_size + 1;
                mutated_offsprings[i][random_row] = new_col;
            }
        }

        // for next iteration, update initial_population
        initial_population=mutated_offsprings;

        iter++;
    }

    print_board(solution);
    cout << iter << "\n";

    return 0;
}
