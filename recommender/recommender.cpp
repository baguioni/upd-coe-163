#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <sstream> 
#include <iomanip>
#include <algorithm>
#include <chrono>

using namespace std;

void sortRatingsAndMovies(vector<pair<float, string>>& movieRatingPairs) {
    sort(movieRatingPairs.begin(), movieRatingPairs.end(), [](const pair<float, string>& a, const pair<float, string>& b) {
        return (a.first != b.first) ? a.first > b.first : a.second < b.second;
    });

    // display top 10
    for (int i = 0; i < 10; ++i) {
        cout << movieRatingPairs[i].second << endl;
    }
}

void parseMovieCSV(const string & filename, vector<float> & user_data) {
  ifstream file(filename);
  vector<pair<float, string>> movieRatingPairs;

  string line;
  string movie_title;

  getline(file, line);
    // skip first line
  while (getline(file, line)) {
    float result = 0.0f;
    
    istringstream ss(line);
    int attribute_index = 0;

    while (ss >> ws) {
      string csvElement;
      if (ss.peek() == '"') {
        ss >> std::quoted(csvElement);
        movie_title = csvElement;
        string discard;
        getline(ss, discard, ',');
      } else {
        getline(ss, csvElement, ',');

        // some strings has trailing newline
        if (!csvElement.empty() && int(csvElement[csvElement.length()-1]) == 13) {
            csvElement.erase(csvElement.length()-1);
        }

        if (csvElement == "1") {
          result += user_data[attribute_index];
          attribute_index++;
        } else if (csvElement == "0") {
          attribute_index++;
                        
        } else {
          if (csvElement.length() != 2 && csvElement != "") {
            movie_title = csvElement;
          }
        }
      }
    }
    movieRatingPairs.push_back(make_pair(result, movie_title));
  }
  sortRatingsAndMovies(movieRatingPairs);
}

// https://stackoverflow.com/a/5207600
void getUserDataFromCSV(const string& filename, int user_id) {
    vector<float> lineData;
    ifstream file(filename);
    file.seekg(ios::beg);
    for(int i = 0; i <= user_id - 1; ++i) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
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
int main(int argc, char* argv[]) {
  ios::sync_with_stdio(false);
  string test_input;

  cin >> test_input;
  test_input = test_input.substr(1);
  int user_id = atoi(test_input.c_str());

  getUserDataFromCSV("user_features.csv", user_id);
  return 0;
}