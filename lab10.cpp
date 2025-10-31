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

    if (((num[0] == '+') || (num[0] == '-')) && (!(isdigit(num[1])))) { //checks if there is a +/- and if it is followed by a digit
        return false;
    }

    if ((num[0] != '+') && (num[0] != '-')) { //if the first element is not +/-
        if (!(isdigit(num[0]))) { //check if the first element is not a digit
            return false;
        }
    }

    for (int i=0; i<num.size(); i++) { //loop through the new parsed num
        if (num[i] == '.') { //if a '.' is found
            if (i == 0 || i == num.size() - 1) { // Check bounds before accessing neighbors
                return false; // decimal at start or end
            } 
            if (isdigit(num[i - 1]) && isdigit(num[i + 1])) {
                return true;  // valid decimal (e.g. 12.34)
            } else {
                return false; // invalid decimal (e.g. .123 or 123.)
            }
        }
    }
    return true;
}

string parse(string num) { //this removes the +/- and leading zeros
    if (num[0] == '+' || num[0] == '-') { //remove leading sign
        num = num.substr(1); //return the rest of the string starting at index 1
    }

    int pos = 0;
    while (pos < (int)num.size() && num[pos] == '0') pos++; //remove leading zeros

    num = num.substr(pos);
    if (num.empty()) num = "0"; //if the num is empty set it to zero
    return num;
}

string add_ints(string num1, string num2) { //-- got this from ChatGPT to manually add two integers
    int i = num1.size() - 1;  // index for a --start at the end of the string
    int j = num2.size() - 1;  // index for b --start at the end of the string
    int carry = 0;
    string result = "";    // final result

    // continue while there are digits left or carry remains
    while (i >= 0 || j >= 0 || carry > 0) { //while there are still un-added digits in either number OR there is still a 1 to carry over
        int digitA = (i >= 0) ? num1[i] - '0' : 0; //if there are still digits left in num1, digitA = current digit (converted to an int by subtracting '0'), otherwise digitA = 0
        int digitB = (j >= 0) ? num2[j] - '0' : 0; //if there are still digits left in num2, digitB = current digit (converted to an int by subtracting '0'), otherwise digitB = 0

        int sum = digitA + digitB + carry; //get the sum of the digits + carry
        int digit = sum % 10; //get the remainder of that sum (second number if there is a carry)
        carry = sum / 10; //get the carried 1 if the sum is 10+

        result = char(digit + '0') + result; //add the final digit to the front of the result string

        i--; //go to the next-left digit
        j--; //go to the next-left digit
    }

    return result;
}

string subtract_ints(string a, string b) { //assumes a >= b -- got this from ChatGPT to manually subtract two integers 
    int i = a.size() - 1, j = b.size() - 1, borrow = 0;
    string result = "";
    while (i >= 0) {
        int digitA = a[i] - '0' - borrow;
        int digitB = (j >= 0) ? b[j] - '0' : 0;
        if (digitA < digitB) {
            digitA += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result = char((digitA - digitB) + '0') + result;
        i--; j--;
    }
    return result;
}


string sum(string num1, string num2) {
    // Split integer and fractional parts
    int pos1 = num1.find('.'); //find the index of the decimal
    int pos2 = num2.find('.'); //find the index of the decimal

    string int1 = (pos1 == string::npos) ? num1 : num1.substr(0, pos1); //if the decimal's position is invalid, int1 = num1, otherwise int1 = the first part of the string before the decimal
    string frac1 = (pos1 == string::npos) ? "" : num1.substr(pos1 + 1); //if the decimal's position is invalid, int1 = empty string, otherwise int1 = the second part of the string after the decimal

    string int2 = (pos2 == string::npos) ? num2 : num2.substr(0, pos2); // ^same thing with the second number string 
    string frac2 = (pos2 == string::npos) ? "" : num2.substr(pos2 + 1);

    // Pad fractional parts to equal length
    while (frac1.size() < frac2.size()) frac1 += '0'; //add trailing zeros after fract1 if fract2 is bigger
    while (frac2.size() < frac1.size()) frac2 += '0'; //add trailing zeros after fract2 if fract1 is bigger

    // Pad integer parts to equal length
    while (int1.size() < int2.size()) int1 = '0' + int1; //add leading zeros before int1 if int2 is bigger
    while (int2.size() < int1.size()) int2 = '0' + int2; //add leading zeros before int 2 if int1 is bigger

    // Add fractional parts (if any)
    string fracSum = "";
    int carry = 0;

    if (!frac1.empty() || !frac2.empty()) { //similar code to add_ints but saves the carry so it can be added to the integers
        int i = frac1.size() - 1;
        while (i >= 0) {
            int digitA = frac1[i] - '0';
            int digitB = frac2[i] - '0';
            int sum = digitA + digitB + carry;
            int digit = sum % 10;
            carry = sum / 10;
            fracSum = char(digit + '0') + fracSum;
            i--;
        }
    }

    // Add integer parts, including carry
    string intSum = add_ints(int1, int2);
    if (carry > 0) { //if carry from the fractional part is 1, add that in as well
        intSum = add_ints(intSum, "1");
    }
    // Combine results
    string result = intSum;
    if (!fracSum.empty()) { //if there are decimal points
        result += "." + fracSum; //add them after the intsum
    }
    return result; //return final result
}

// Compare two integer strings (returns -1 if a<b, 0 if equal, 1 if a>b)
int compareStrings(string a, string b) {
    if (a.size() != b.size()) { //if the strings are of different length
        if (a.size() < b.size()) {
            return -1;
        } else {
            return 1;
        }
    } 
    //if the strings are of the same length
    if (a == b) { 
        return 0;
    } else if (a < b) {
        return -1;
    } else {
        return 1;
    }
}

string subtract(string num1, string num2) {
    bool neg1 = false, neg2 = false; //init each numbers neg bool to false -- keeping track of negatives 
    if (num1[0] == '-') { 
        neg1 = true; num1 = num1.substr(1); 
    }
    if (num2[0] == '-') { 
        neg2 = true; num2 = num2.substr(1); 
    }

    if (neg1 && neg2)  return subtract(num2, num1);      // (-a) - (-b) = b - a
    if (!neg1 && neg2) return sum(num1, num2);           // a - (-b) = a + b
    if (neg1 && !neg2) return "-" + sum(num1, num2);     // (-a) - b = -(a + b)

    int pos1 = num1.find('.'); //find the index of the decimal
    int pos2 = num2.find('.'); //find the index of the decimal

    string int1 = (pos1 == string::npos) ? num1 : num1.substr(0, pos1); //if the decimal's position is invalid, int1 = num1, otherwise int1 = the first part of the string before the decimal
    string frac1 = (pos1 == string::npos) ? "" : num1.substr(pos1 + 1); //if the decimal's position is invalid, int1 = empty string, otherwise int1 = the second part of the string after the decimal

    string int2 = (pos2 == string::npos) ? num2 : num2.substr(0, pos2); // ^same thing with the second number string 
    string frac2 = (pos2 == string::npos) ? "" : num2.substr(pos2 + 1);

    while (frac1.size() < frac2.size()) frac1 += '0'; //add trailing zeros after fract1 if fract2 is bigger
    while (frac2.size() < frac1.size()) frac2 += '0'; //add trailing zeros after fract2 if fract1 is bigger

    while (int1.size() < int2.size()) int1 = '0' + int1; //add leading zeros before int1 if int2 is bigger
    while (int2.size() < int1.size()) int2 = '0' + int2; //add leading zeros before int 2 if int1 is bigger 

    bool swap = compareStrings(int1 + frac1, int2 + frac2) < 0; //if num1 is less than num2, swap = true -- meaning that num1 needs to be swapped to be before num2

    string bigInt = swap ? int2 : int1; //swap ints and fracts of each number if swap is true
    string bigFrac = swap ? frac2 : frac1;
    string smallInt = swap ? int1 : int2;
    string smallFrac = swap ? frac1 : frac2;

    string fracResult = "";
    int borrow = 0; //init borrow to 0

    for (int i = bigFrac.size() - 1; i >= 0; i--) { //got this code from ChatGPT on how to manually subtract
        int dA = bigFrac[i] - '0' - borrow;
        int dB = smallFrac[i] - '0';
        if (dA < dB) { dA += 10; borrow = 1; }
        else borrow = 0;
        fracResult = char((dA - dB) + '0') + fracResult;
    }

    string intResult = subtract_ints(bigInt, smallInt);
    if (borrow > 0) {
        intResult = subtract_ints(intResult, "1");
    } 
    string result = intResult;
    if (!fracResult.empty()) {
        result += "." + fracResult;
    }

    while (!result.empty() && result.back() == '0') result.pop_back(); //get rid of trailing zeros

    if (!result.empty() && result.back() == '.') {
        result.pop_back();
    }

    if (swap) { //if the numbers had to be swapped add the necessary '-' sign in front
        result = "-" + result;
    }

    return result.empty() ? "0" : result;
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
        vector<string> result = split(line, ' '); //returns in the format [num1, num2]
        string num1 = result[0]; 
        string num2 = result[1];
        
        if (!isvalid(num1) || !isvalid(num1)) {
            cout << "Invalid double(s)" << endl;
            continue; //skip all functionality and go to next line in the file
        }

        if (isdigit(num1[0])) { //convert the num to positive if there is no sign
            num1 = '+' + num1;
        }
        if (isdigit(num2[0])) { //convert the num to positive if there is no sign
            num2 = '+' + num2;
        }

        if ((num1[0] == '+') && (num2[0] == '+')) {
            cout << "Sum of " << num1 << " and " << num2 << ": " << sum(parse(num1), parse(num2)) << endl;

        } else if ((num1[0] == '+') && (num2[0] == '-')) {
            cout << "Sum of " << num1 << " and " << num2 << ": " << subtract(parse(num1), parse(num2.substr(1))) << endl;

        } else if ((num1[0] == '-') && (num2[0] == '+')) {
            cout << "Sum of " << num1 << " and " << num2 << ": " << subtract(parse(num1.substr(1)), parse(num2)) << endl;

        } else if ((num1[0] == '-') && (num2[0] == '-')) {
            cout << "Sum of " << num1 << " and " << num2 << ": " << subtract(parse(num1.substr(1)), parse(num2.substr(1))) << endl;
        }
    }

    File.close();
    return 0;
}