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

    for (int i = 0; i < 10; ++i) {
        cout << movieRatingPairs[i].second << endl;
    }
}

void parseMovieCSV(const string & filename, vector<float> & user_data) {
  ifstream file(filename);

  vector<pair<float, string>> movieRatingPairs;

  if (file.is_open()) {
    string line;
    int count = 0;
    while (getline(file, line)) {
      float result = 0.0f;
      string movie_title;
      istringstream ss(line);
      int attribute_index = 0;

      // skip first line
      if (count == 1) {

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
              if (csvElement.length() != 2) {
                movie_title = csvElement;
              }
            }

          }
        }
        movieRatingPairs.push_back(make_pair(result, movie_title));
        
      } else {
        count++;
      }
    }
  } else {
    cout << "Failed to open the file." << endl;
  }
  sortRatingsAndMovies(movieRatingPairs);
}

// https://stackoverflow.com/a/5207600
void getUserDataFromCSV(const string& filename, unsigned int user_id) {
    vector<float> lineData;
    fstream file(filename);

    file.seekg(ios::beg);
    for(int i = 0; i <= user_id - 1; ++i) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string line;
    if (getline(file, line)) {
        stringstream ss(line);
        string token;
        for (int i = 0; i < 2; ++i) {
            if (!getline(ss, token, ',')) {
                break;
            }
        }
        
        while (getline(ss, token, ',')) {
            lineData.push_back(stof(token));
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
    if (argc != 2) {
        return 1;
    }

    // Extract the user_id from the command-line arguments and convert to int
    int user_id = atoi(argv[1]);

    // Capture the start time
    auto start = chrono::steady_clock::now();
    getUserDataFromCSV("user_features.csv", user_id);

    // Capture the end time
    auto end = chrono::steady_clock::now();

    // Calculate the duration
    std::chrono::duration<float> duration = end - start;

    std::cout << "Runtime: " << duration.count() << " seconds" << std::endl;



    return 0; // Return success
}