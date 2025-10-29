#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;


vector<string> split(string str, char del){
    vector<string> split_str;
    string temp = ""; //create temp string

    for (int i=0; i<(int)str.size(); i++) { //iterate through the string
        if (str[i] != del) { //if the char is not the delimeter add it to the substring
            temp += str[i];
        } else { //if the char is the delimeter
            if (!temp.empty()) { //as long as temp is not empty
                split_str.push_back(temp); //append the substring to the array
                temp = ""; //reset temp
            }
        }
    }
    //handles adding the second element, since the end of the line is not a delimeter
    if (!temp.empty()) { //as long as temp is not empty
        split_str.push_back(temp); //append the substring to the array
        temp = ""; //reset temp
    }

    return split_str; //return the array
}

bool isvalid (string num) {
    if (num.empty()) {
        return false;
    }

    if (((num[0] == '+') || (num[0] == '-')) && ((num[1] != '+') && (num[1] != '-'))) {
        return true;
    }

    for (int i=0; i<(int)num.size(); i++) {
        if ((num[i] == '+') || (num[i] == '-')) {
            for (int j = i; j < (int)num.size() - 1; j++) {
                num[j] = num[j + 1]; //start at the index of the +/- and shift all other following characters to the left (removing it)
            }
            num.pop_back(); //remove last extra element
            i--; //decrement the index as the next character has moved backwards by 1
        }
    }

    for (int i=0; i<num.size(); i++) {
        if ((num[i] == '.') && (isdigit(num[i+1]))) { //check if there is a decimal point and if there is a digit after the decimal point
            return true;
        }
    }

    return true;
}

int main() {
    string filename;
    cout << "Enter a file name: " <<endl;
    cin >> filename; 

    ifstream File(filename);

    if (!File) { // always check that the file opened successfully
        cerr << "Error: could not open file.\n";
        return 0;
    }

    string line;
    while(getline(File, line)) {
        vector<string> result = split(line, ' ');
        cout << result[0] << endl;
        cout << isvalid(result[0]) << endl;
        cout << result[1] << endl;
        cout << isvalid(result[1]) << endl;
    }

    File.close();
    return 0;
}