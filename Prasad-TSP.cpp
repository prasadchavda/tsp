#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

struct City {
    double x_coordinate;
    double y_coordinate;
};

double calculateDistance(const City& a, const City& b)
{
    return sqrt(pow(a.x_coordinate - b.x_coordinate, 2) + pow(a.y_coordinate - b.y_coordinate, 2));
}

double calculateTotalDistance(const vector<City>& cities, const vector<int>& tour_order) {
    double total_distance = 0;
    for (size_t i = 0; i < tour_order.size() - 1; ++i) {
        total_distance += calculateDistance(cities[tour_order[i]], cities[tour_order[i + 1]]);
    }
    total_distance += calculateDistance(cities[tour_order.back()], cities[tour_order.front()]); // Return to start
    return total_distance;
}

vector<int> createInitialTour(int num_cities) {
    vector<int> tour_order(num_cities);
    for (int i = 0; i < num_cities; ++i) tour_order[i] = i;
    random_shuffle(tour_order.begin(), tour_order.end());
    return tour_order;
}

vector<int> generateNeighboringTour(const vector<int>& current_tour) {
    vector<int> new_tour = current_tour;
    int i = rand() % new_tour.size();
    int j = rand() % new_tour.size();
    swap(new_tour[i], new_tour[j]);
    return new_tour;
}

bool shouldAcceptSolution(double current_distance, double new_distance, double temperature) {
    if (new_distance < current_distance) return true;
    double acceptance_probability = exp((current_distance - new_distance) / temperature);
    return (rand() / (double)RAND_MAX) < acceptance_probability;
}

void simulatedAnnealing(vector<City>& cities, int num_iterations, double initial_temperature, double cooling_factor) {
    int num_cities = cities.size();
    vector<int> current_tour = createInitialTour(num_cities);
    double current_distance = calculateTotalDistance(cities, current_tour);

    vector<int> best_tour = current_tour;
    double best_distance = current_distance;

    double temperature = initial_temperature;

    for (int iteration = 0; iteration < num_iterations; ++iteration) {
        vector<int> new_tour = generateNeighboringTour(current_tour);
        double new_distance = calculateTotalDistance(cities, new_tour);

        if (shouldAcceptSolution(current_distance, new_distance, temperature)) {
            current_tour = new_tour;
            current_distance = new_distance;

            if (current_distance < best_distance) {
                best_tour = current_tour;
                best_distance = current_distance;
            }
        }

        temperature *= cooling_factor;
    }

    cout << "Best tour distance: " << best_distance << endl;
    cout << "Best tour: ";
    for (int city_index : best_tour) {
        cout << city_index << " ";
    }
    cout << endl;
}

int main() {
    srand(time(0));

    // Example: Define cities
    vector<City> cities = {
        {0, 0}, {1, 3}, {4, 3}, {6, 1}, {3, 0}
    };

    int num_iterations = 10000;
    double initial_temperature = 1000.0;
    double cooling_factor = 0.995;

    simulatedAnnealing(cities, num_iterations, initial_temperature, cooling_factor);

    return 0;
}