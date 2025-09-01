#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <algorithm>

struct Point {
    long long x, y;
    Point(long long x, long long y) : x(x), y(y) {}
};

struct RootData {
    int base;
    std::string value;
};

// Convert number from given base to decimal
long long baseToDecimal(const std::string& value, int base) {
    long long result = 0;
    long long power = 1;
    
    for (int i = value.length() - 1; i >= 0; i--) {
        int digit;
        if (value[i] >= '0' && value[i] <= '9') {
            digit = value[i] - '0';
        } else if (value[i] >= 'A' && value[i] <= 'F') {
            digit = value[i] - 'A' + 10;
        } else if (value[i] >= 'a' && value[i] <= 'f') {
            digit = value[i] - 'a' + 10;
        } else {
            throw std::invalid_argument("Invalid character in number");
        }
        
        if (digit >= base) {
            throw std::invalid_argument("Digit exceeds base");
        }
        
        result += digit * power;
        power *= base;
    }
    
    return result;
}

// Simple JSON parser for the specific format
class JSONParser {
private:
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }
    
    static std::string removeQuotes(const std::string& str) {
        std::string result = str;
        if (result.front() == '"') result.erase(0, 1);
        if (result.back() == '"') result.pop_back();
        return result;
    }

public:
    static void parseFile(const std::string& filename, int& n, int& k, std::map<int, RootData>& roots) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
        
        // Parse keys section
        size_t keysStart = content.find("\"keys\"");
        if (keysStart != std::string::npos) {
            size_t keysEnd = content.find("}", keysStart);
            std::string keysSection = content.substr(keysStart, keysEnd - keysStart);
            
            // Extract n
            size_t nPos = keysSection.find("\"n\"");
            if (nPos != std::string::npos) {
                size_t colonPos = keysSection.find(":", nPos);
                size_t commaPos = keysSection.find(",", colonPos);
                if (commaPos == std::string::npos) commaPos = keysSection.find("}", colonPos);
                std::string nValue = keysSection.substr(colonPos + 1, commaPos - colonPos - 1);
                n = std::stoi(trim(nValue));
            }
            
            // Extract k
            size_t kPos = keysSection.find("\"k\"");
            if (kPos != std::string::npos) {
                size_t colonPos = keysSection.find(":", kPos);
                size_t endPos = keysSection.find_first_of(",}", colonPos);
                std::string kValue = keysSection.substr(colonPos + 1, endPos - colonPos - 1);
                k = std::stoi(trim(kValue));
            }
        }
        
        // Parse root sections
        size_t pos = 0;
        while ((pos = content.find("\"", pos)) != std::string::npos) {
            pos++; // Move past the quote
            size_t endQuote = content.find("\"", pos);
            if (endQuote == std::string::npos) break;
            
            std::string key = content.substr(pos, endQuote - pos);
            pos = endQuote + 1;
            
            // Skip keys section
            if (key == "keys" || key == "n" || key == "k" || key == "base" || key == "value") {
                continue;
            }
            
            // Try to parse as integer (root number)
            try {
                int rootNum = std::stoi(key);
                
                // Find the corresponding object
                size_t objStart = content.find("{", pos);
                if (objStart == std::string::npos) continue;
                
                size_t objEnd = content.find("}", objStart);
                if (objEnd == std::string::npos) continue;
                
                std::string objContent = content.substr(objStart + 1, objEnd - objStart - 1);
                
                RootData rootData;
                
                // Extract base
                size_t basePos = objContent.find("\"base\"");
                if (basePos != std::string::npos) {
                    size_t colonPos = objContent.find(":", basePos);
                    size_t commaPos = objContent.find(",", colonPos);
                    if (commaPos == std::string::npos) commaPos = objContent.length();
                    std::string baseValue = objContent.substr(colonPos + 1, commaPos - colonPos - 1);
                    rootData.base = std::stoi(removeQuotes(trim(baseValue)));
                }
                
                // Extract value
                size_t valuePos = objContent.find("\"value\"");
                if (valuePos != std::string::npos) {
                    size_t colonPos = objContent.find(":", valuePos);
                    size_t endPos = objContent.length();
                    std::string valueStr = objContent.substr(colonPos + 1, endPos - colonPos - 1);
                    rootData.value = removeQuotes(trim(valueStr));
                }
                
                roots[rootNum] = rootData;
                pos = objEnd;
                
            } catch (...) {
                // Not a number, skip
            }
        }
    }
};

// Lagrange interpolation to find the constant term
double lagrangeInterpolation(const std::vector<Point>& points, int k) {
    double result = 0.0;
    
    // We only need the constant term, so we evaluate at x = 0
    for (int i = 0; i < k; i++) {
        double term = points[i].y;
        
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0.0 - points[j].x) / (double)(points[i].x - points[j].x);
            }
        }
        
        result += term;
    }
    
    return result;
}

int main() {
    try {
        int n, k;
        std::map<int, RootData> roots;
        
        // Parse JSON file
        JSONParser::parseFile("input.json", n, k, roots);
        
        std::cout << "=== Shamir's Secret Sharing Solver ===" << std::endl;
        std::cout << "n (number of roots provided): " << n << std::endl;
        std::cout << "k (minimum roots needed): " << k << std::endl;
        std::cout << std::endl;
        
        // Convert roots to points
        std::vector<Point> points;
        
        std::cout << "Converting roots to decimal points:" << std::endl;
        for (const auto& root : roots) {
            int x = root.first;
            int base = root.second.base;
            std::string value = root.second.value;
            
            long long y = baseToDecimal(value, base);
            points.push_back(Point(x, y));
            
            std::cout << "Root " << x << ": base " << base 
                      << ", value \"" << value << "\" -> (" << x << ", " << y << ")" << std::endl;
        }
        std::cout << std::endl;
        
        if (points.size() < k) {
            throw std::runtime_error("Not enough points to solve the polynomial");
        }
        
        // Use first k points for interpolation
        std::vector<Point> selectedPoints(points.begin(), points.begin() + k);
        
        std::cout << "Using first " << k << " points for Lagrange interpolation:" << std::endl;
        for (int i = 0; i < k; i++) {
            std::cout << "(" << selectedPoints[i].x << ", " << selectedPoints[i].y << ")";
            if (i < k - 1) std::cout << ", ";
        }
        std::cout << std::endl << std::endl;
        
        // Find the constant term using Lagrange interpolation
        double constant = lagrangeInterpolation(selectedPoints, k);
        
        std::cout << "=== RESULT ===" << std::endl;
        std::cout << "The constant term (secret) is: " << std::fixed << std::setprecision(0) << constant << std::endl;
        
        // Verify with manual calculation for the sample
        if (k == 3 && selectedPoints.size() >= 3) {
            std::cout << std::endl << "=== Manual Verification ===" << std::endl;
            std::cout << "For points (" << selectedPoints[0].x << "," << selectedPoints[0].y << "), "
                      << "(" << selectedPoints[1].x << "," << selectedPoints[1].y << "), "
                      << "(" << selectedPoints[2].x << "," << selectedPoints[2].y << ")" << std::endl;
            
            // Manual calculation
            long long x1 = selectedPoints[0].x, y1 = selectedPoints[0].y;
            long long x2 = selectedPoints[1].x, y2 = selectedPoints[1].y;
            long long x3 = selectedPoints[2].x, y3 = selectedPoints[2].y;
            
            double manual = y1 * (0.0 - x2) * (0.0 - x3) / ((x1 - x2) * (x1 - x3)) +
                           y2 * (0.0 - x1) * (0.0 - x3) / ((x2 - x1) * (x2 - x3)) +
                           y3 * (0.0 - x1) * (0.0 - x2) / ((x3 - x1) * (x3 - x2));
            
            std::cout << "Manual calculation result: " << std::fixed << std::setprecision(0) << manual << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}