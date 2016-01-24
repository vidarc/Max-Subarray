#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <limits.h>

// Enumeration
int algorithm1(std::vector<int>, std::ofstream&, int&, int&);
// Better enumeration
int algorithm2(std::vector<int>, std::ofstream&, int&, int&);
// Divide and Conquer
struct maxSub {
  int sum;
  int start;
  int end;
};
int algorithm3(std::vector<int>, std::ofstream&, int&, int&);
maxSub findMaxSub(std::vector<int>, int, int);
maxSub maxCrossingSub(std::vector<int>, int, int, int);
// Linear Time
int algorithm4(std::vector<int>, std::ofstream&, int&, int&);
void printSubArray(std::vector<int>, std::ofstream&, int, int, int&, int&);
// functions to do the runtime analysis
void runtimeAnalysis(std::ofstream&);

int main() {
    std::ifstream input;
    input.open("MSS_TestProblems-1.txt");
    std::ofstream output;
    output.open("MSS_Results.txt", std::ofstream::out | std::ofstream::trunc);
    
    std::vector<std::vector<int> > numbers;
    std::vector<int> tempNumbers;
    std::vector<std::string> temp;
    std::string line;
    char *token;
    char *duplicate;
    int start = 0,
        end = 0;
    // random seed for later
    srand(unsigned(time(NULL)));
    
    // Check to make sure the input file is there
    if (!input.is_open()) {
        input.close();
        std::cout << "MSS_Problems.txt not found. Please try again" << std::endl;
        return 1;
    }
    
    // read the file and put each line into a string vector
    while (getline(input, line)) {
      temp.push_back(line);
    }
    
    // get rid of the commas [] and spaces
    for (int i = 0; i < temp.size(); i++) {
      // use strdup so that I can convert the string into a usable char array
      // for strtok
      duplicate = strdup(temp[i].c_str());
      token = strtok(duplicate, "[, ]");
      // use atoi to convert each token to an int and push onto first vector
      while (token != NULL) {
        int tempNumber = atoi(token);
        tempNumbers.push_back(tempNumber);
        token = strtok(NULL, "[, ]");
      }
      // random added 0s fix
      if (i != temp.size() - 1) {
        tempNumbers.pop_back();
      }
      // push the array to the number array
      numbers.push_back(tempNumbers);
      // delete stuff
      tempNumbers.clear();
      free(duplicate);
    }
    
    // Pass the vectors for the 1st alogorithm
    output << "-----Results for the 1st algorithm-----" << std::endl;
    for (int i = 0; i < numbers.size(); i++) {
      for (int j = 0; j < numbers[i].size(); j++) {
        tempNumbers.push_back(numbers[i][j]);
      }
      int maxSum = algorithm1(tempNumbers, output, start, end);
      printSubArray(tempNumbers, output, i, maxSum, start, end);
      // delete stuff
      tempNumbers.clear();
    }
    
    // On to the 2nd algorithm
    output << "-----Results for the 2nd algorithm-----" << std::endl;
    for (int i = 0; i < numbers.size(); i++) {
      for (int j = 0; j < numbers[i].size(); j++) {
        tempNumbers.push_back(numbers[i][j]);
      }
      int maxSum = algorithm2(tempNumbers, output, start, end);
      printSubArray(tempNumbers, output, i, maxSum, start, end);
      //delete stuff
      tempNumbers.clear();
    }
    
    // On to the 3rd algorithm
    output << "-----Results for the 3rd algorithm-----" << std::endl;
    for (int i = 0; i < numbers.size(); i++) {
      for (int j = 0; j < numbers[i].size(); j++) {
        tempNumbers.push_back(numbers[i][j]);
      }
      int maxSum = algorithm3(tempNumbers, output, start, end);
      printSubArray(tempNumbers, output, i, maxSum, start, end);
      //delete stuff
      tempNumbers.clear();
    }
    
    // On to the 4th algorithm
    output << "-----Results for the 4th algorithm-----" << std::endl;
    for (int i = 0; i < numbers.size(); i++) {
      for (int j = 0; j < numbers[i].size(); j++) {
        tempNumbers.push_back(numbers[i][j]);
      }
      int maxSum = algorithm4(tempNumbers, output, start, end);
      printSubArray(tempNumbers, output, i, maxSum, start, end);
      //delete stuff
      tempNumbers.clear();
    }
    
    // Runtime Analysis
    int choice;
    std::cout << "Enter 1 for Runtime Analysis (anything else to end): ";
    std::cin >> choice;
    if (choice == 1) {
      output << "-----Runtime Analysis-----" << std::endl;
      runtimeAnalysis(output);
    }
    
    input.close();
    output.close();
    
    return 0;
}

int algorithm1(std::vector<int> vec, std::ofstream &output, int &start, int &end) {
  int max = 0,
      newSum = 0;
  start = 0;
  end = 0;
      
  for (int i = 0; i < vec.size(); i++) {
    for (int j = i; j < vec.size(); j++) {
      newSum = 0;
      for (int k = i; k <= j; k++) {
        newSum += vec[k];
      }
      
      if (newSum > max) {
        max = newSum;
        start = i;
        end = j;
      }
    }
  }
  return max;
}

int algorithm2(std::vector<int> vec, std::ofstream &output, int &start, int &end) {
  int max = 0,
      newSum = 0;
  start = 0;
  end = 0;
      
  for (int i = 0; i < vec.size(); i++) {
    newSum = 0;
    for (int j = i; j < vec.size(); j++) {
      newSum += vec[j];
      if (newSum > max) {
        max = newSum;
        start = i;
        end = j;
      }
    }
  }
  return max;
}

int algorithm3(std::vector<int> vec, std::ofstream &output, int &start, int &end) {
  start = 0;
  end = vec.size() - 1;
  maxSub ms;
  ms = findMaxSub(vec, start, end);
  start = ms.start;
  end = ms.end;
  return ms.sum;
}

maxSub findMaxSub(std::vector<int> vec, int start, int end) {
  maxSub temp;
  
  if (start == end) {
    temp.start = start;
    temp.end = end;
    temp.sum = vec[start];
    return temp;
  }
  
  int mid = (start + end) / 2;
  
  maxSub leftSub;
  leftSub = findMaxSub(vec, start, mid);
  maxSub rightSub;
  rightSub = findMaxSub(vec, mid + 1, end);
  maxSub crossSub;
  crossSub = maxCrossingSub(vec, start, end, mid);
  
  if (leftSub.sum >= rightSub.sum && leftSub.sum >= crossSub.sum) {
    return leftSub;
  }
  else if (rightSub.sum >= leftSub.sum && rightSub.sum >= crossSub.sum) {
    return rightSub;
  }
  else {
    return crossSub;
  }
}

maxSub maxCrossingSub(std::vector<int> vec, int crossStart, int crossEnd, int mid) {
  maxSub temp;
  int leftMax = INT_MIN;
  int leftStart, rightEnd;
  int sum = 0;
  
  for (int i = mid; i >= crossStart; i--) {
    sum += vec[i];
    if (sum > leftMax) {
      leftMax = sum;
      leftStart = i;
    }
  }
  
  int rightMax = INT_MIN;
  sum = 0;
  for (int j = mid + 1; j <= crossEnd; j++) {
    sum += vec[j];
    if (sum > rightMax) {
      rightMax = sum;
      rightEnd = j;
    }
  }
  
  temp.sum = leftMax + rightMax;
  temp.start = leftStart;
  temp.end = rightEnd;
  
  return temp;
}

int algorithm4(std::vector<int> vec, std::ofstream &output, int &start, int &end) {
  int max = 0,
      newSum = 0,
      tempStart = 0,
      tempEnd = 0;
  start = 0;
  end = 0;
  
  for (int i = 0; i < vec.size(); i++) {
    tempEnd = i;
    if (newSum > 0) {
      newSum = newSum + vec[i];
    }
    else {
      tempStart = i;
      newSum = vec[i];
    }
    if (newSum > max) {
      max = newSum;
      start = tempStart;
      end = tempEnd;
    }
  }
  
  return max;
}

void printSubArray(std::vector<int> vec, std::ofstream &output, int count, int max, int &start, int &end) {
  // print out the results to file
  output << "Array #" << count + 1 << std::endl;
  output << "Max Sum: " << max << std::endl;
  output << "Array: [";
  for (int i = start; i < end; i++) {
    output << vec[i] << ", ";
  }
  output << vec[end] << "]\n" << std::endl;
}

void runtimeAnalysis(std::ofstream &output) {
  std::vector<int> testSubject;
  int size;
  int a = 0;
  int b = 0;
  clock_t begin,
          end;
  double elapsed_secs;
  
  // Run the 1st algorithm
  std::cout << "Running enumeration analysis" << std::endl;
  output << "Enumeration Run Times" << std::endl;
  size = 0;
  for (int i = 0; i < 10; i++) {
    size = size + 125;
    std::cout << "Running Group #" << i + 1 << std::endl;
    for (int j = 0; j < 10; j++) {
      // fill up the testSubject
      for (int x = 0; x < size; x++) {
        testSubject.push_back(rand() % 100);
      }
      begin = clock();
      algorithm1(testSubject, output, a, b);
      end = clock();
      elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
      output << "n = " << size << ": " << elapsed_secs << "s" << std::endl;
      testSubject.clear();
    }
  }

  // Run the 2nd algorithm
  std::cout << "Running better enumeration analysis" << std::endl;
  output << "Better Enumeration Run Times" << std::endl;
  size = 50;
  for (int i = 0; i < 10; i++) {
    size = size * 2;
    std::cout << "Running Group #" << i + 1 << std::endl;
    for (int j = 0; j < 10; j++) {
      // fill up the testSubject
      for (int x = 0; x < size; x++) {
        testSubject.push_back(rand() % 100);
      }
      begin = clock();
      algorithm2(testSubject, output, a, b);
      end = clock();
      elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
      output << "n = " << size << ": " << elapsed_secs << "s" << std::endl;
      testSubject.clear();
    }
  }
  
  // Run the 3rd algorithm
  std::cout << "Running Didive and Conquer analysis" << std::endl;
  output << "Divide and Conquer Run Times" << std::endl;
  size = 200;
  for (int i = 0; i < 10; i++) {
    size = size * 2;
    std::cout << "Running Group #" << i + 1 << std::endl;
    for (int j = 0; j < 10; j++) {
      // fill up the testSubject
      for (int x = 0; x < size; x++) {
        testSubject.push_back(rand() % 100);
      }
      begin = clock();
      algorithm3(testSubject, output, a, b);
      end = clock();
      elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
      output << "n = " << size << ": " << elapsed_secs << "s" << std::endl;
      testSubject.clear();
    }
  }
  
  // Run the 4th algorithm
  std::cout << "Running Linear Time analysis" << std::endl;
  output << "Linear Time Run Times" << std::endl;
  size = 100000;
  for (int i = 0; i < 10; i++) {
    size = size * 2;
    std::cout << "Running Group #" << i + 1 << std::endl;
    for (int j = 0; j < 10; j++) {
      // fill up the testSubject
      for (int x = 0; x < size; x++) {
        testSubject.push_back(rand() % 100);
      }
      begin = clock();
      algorithm4(testSubject, output, a, b);
      end = clock();
      elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
      output << "n = " << size << ": " << elapsed_secs << "s" << std::endl;
      testSubject.clear();
    }
  }
}
