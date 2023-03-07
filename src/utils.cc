#include <vector>
#include <algorithm>
#include <iostream>
#include <dirent.h>


bool sortAlphabetically(const struct dirent* a, const struct dirent* b) {
    return std::string(a->d_name) < std::string(b->d_name);
}

void print_vector(const std::vector<double>& vec) {
    std::cout << "[";
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}
std::vector<double> get_values_at_target_index(const std::vector<double>& vec1, const std::vector<double>& vec2, double target) {
    std::vector<double> result;
    for (int i = 0; i < vec1.size(); i++) {
        if (vec1[i] == target) {
            result.push_back(vec2[i]);
        }
    }
    return result;
}


std::pair<std::vector<double>, std::vector<double>> get_values_in_range(const std::vector<double>& vec1, const std::vector<double>& vec2, double range_start, double range_end) {
    std::vector<double> result_vec1;
    std::vector<double> result_vec2;
    for (int i = 0; i < vec1.size(); i++) {
        if (vec1[i] >= range_start && vec1[i] <= range_end) {
            result_vec1.push_back(vec1[i]);
            result_vec2.push_back(vec2[i]);
        }
    }
    return {result_vec1, result_vec2};
}

std::vector<double> get_values_until_change(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    std::vector<double> result;
    if (vec1.size() == 0 || vec2.size() == 0) {
        return result; // return empty vector if either input vector is empty
    }
    double first_value = vec1[0];
    for (int i = 0; i < vec1.size(); i++) {
        if (vec1[i] != first_value) {
            break; // break loop when first value changes
        }
        result.push_back(vec2[i]); // add corresponding value from second vector to result
    }
    return result;
}

std::vector<double> returnSubVector(const std::vector<double>& v, int index)
{
    // Create an iterator pointing to the index position
    auto start = v.begin() + index;

    // Create a new vector containing elements from the index position to the end
    std::vector<double> subVector(start, v.end());

    return subVector;
}
