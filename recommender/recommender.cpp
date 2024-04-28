#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <sstream> 
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;

// https://en.cppreference.com/w/cpp/algorithm/partial_sort
void sortRatingsAndMovies(vector < pair < float, string >> & movieRatingPairs) {
  partial_sort(movieRatingPairs.begin(), movieRatingPairs.begin() + min(10, static_cast < int > (movieRatingPairs.size())), movieRatingPairs.end(),
    [](const pair < float, string > & a,
      const pair < float, string > & b) {
      return (a.first != b.first) ? a.first > b.first : a.second < b.second;
    });

  string result;
  for (int i = 0; i < 10; ++i) {
    result += movieRatingPairs[i].second;
    result += '\n';
  }
  cout << result;
}

void parseMovieCSV(const string & filename, vector < float > & user_data) {
  ifstream file(filename);
  vector < pair < float, string >> movieRatingPairs;
  movieRatingPairs.reserve(398);

  string line;

  getline(file, line);
  // skip first line

  while (getline(file, line)) {
    float result = 0.0f;
    string movie_title;
    // Movie title with quotes
    if (line[0] == '"') {
      line.erase(line.begin());
      size_t end = line.find('"');

      string values(line.begin() + end + 1, line.end());
      movie_title.assign(line.begin(), line.begin() + end);

      string token;
      stringstream ss(values);

      // Skip the first comma
      getline(ss, token, ',');

      int attribute_index = 0;
      while (getline(ss, token, ',')) {
        float value = stof(token);
        result += value * user_data[attribute_index];
        attribute_index++;
      }

    } else {
      stringstream ss(line);
      string token;
      int attribute_index = 0;
      while (getline(ss, token, ',')) {
        if (attribute_index == 0) {
          movie_title = token;
        } else {
          float value = stof(token);
          result += value * user_data[attribute_index - 1];
        }
        attribute_index++;
      }
    }
    movieRatingPairs.push_back(make_pair(round(result * 10000) / 10000, movie_title));
  }

  sortRatingsAndMovies(movieRatingPairs);
}

void getUserDataFromCSV(const string & filename, int user_id) {
  vector < float > lineData;
  lineData.reserve(20);
  ifstream file(filename);
  file.seekg(ios::beg);
  for (int i = 0; i <= user_id - 1; ++i) {
    file.ignore(numeric_limits < streamsize > ::max(), '\n');
  }

  string line;
  if (getline(file, line)) {
    stringstream ss(line);
    string token;

    // Skip first 2 columsn
    for (int i = 0; i < 2; ++i) {
      if (!getline(ss, token, ',')) {
        break;
      }
    }

    while (getline(ss, token, ',')) {
      lineData.emplace_back(stof(token));
    }
  }

  parseMovieCSV("movie_features.csv", lineData);
}

/*
  1. Parse user_features.csv but only get the specified user_id
  2. Parse movie_features.csv and calculate the ratings while parsing
  3. Sort the ratings and movie titles
*/
int main(int argc, char * argv[]) {
  ios::sync_with_stdio(false);
  string test_input;

  cin >> test_input;
  test_input = test_input.substr(1);
  int user_id = atoi(test_input.c_str());
  auto start = chrono::steady_clock::now();
  getUserDataFromCSV("user_features.csv", user_id);

  // Capture the end time
  auto end = chrono::steady_clock::now();

  // Calculate the duration
  std::chrono::duration < float > duration = end - start;

  std::cout << "Runtime: " << duration.count() << " seconds" << std::endl;
  return 0;
}