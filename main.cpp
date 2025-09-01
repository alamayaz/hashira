#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Use a large prime for modular arithmetic (similar to Java approach)
const long long MOD = 1000000007LL; // Large prime number

// Modular arithmetic functions
long long modAdd(long long a, long long b, long long mod = MOD) {
    return ((a % mod) + (b % mod)) % mod;
}

long long modMul(long long a, long long b, long long mod = MOD) {
    return ((a % mod) * (b % mod)) % mod;
}

long long modPow(long long base, long long exp, long long mod = MOD) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = modMul(result, base, mod);
        base = modMul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// Extended Euclidean Algorithm for modular inverse
long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    long long x1, y1;
    long long gcd = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

long long modInverse(long long a, long long mod = MOD) {
    long long x, y;
    long long gcd = extgcd(a, mod, x, y);
    if (gcd != 1) return -1; // No inverse exists
    return (x % mod + mod) % mod;
}

struct Point {
    long long x, y;
    Point(long long x, long long y) : x(x), y(y) {}
};

struct RootData {
    int base;
    std::string value;
};

// Convert from base to decimal (with overflow protection)
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
        
        // Use modular arithmetic to prevent overflow
        result = modAdd(result, modMul(digit, power));
        power = modMul(power, base);
    }
    
    return result;
}

// Lagrange interpolation using modular arithmetic (like Java code)
long long lagrangeInterpolationModular(const std::vector<Point>& points, int k) {
    long long result = 0;
    
    std::cout << "\n=== Modular Lagrange Interpolation ===" << std::endl;
    std::cout << "Using prime modulus: " << MOD << std::endl;
    
    for (int i = 0; i < k; i++) {
        long long numerator = 1;
        long long denominator = 1;
        
        std::cout << "\nTerm " << (i+1) << ": y" << (i+1) << " = " << points[i].y << std::endl;
        
        for (int j = 0; j < k; j++) {
            if (i != j) {
                // numerator *= (0 - points[j].x)
                numerator = modMul(numerator, (MOD - points[j].x) % MOD);
                
                // denominator *= (points[i].x - points[j].x)
                long long diff = (points[i].x - points[j].x + MOD) % MOD;
                denominator = modMul(denominator, diff);
                
                std::cout << "  * (0 - " << points[j].x << ") / (" 
                          << points[i].x << " - " << points[j].x << ")" << std::endl;
            }
        }
        
        // Calculate: points[i].y * numerator * modInverse(denominator)
        long long inv_denominator = modInverse(denominator);
        if (inv_denominator == -1) {
            throw std::runtime_error("Cannot find modular inverse");
        }
        
        long long term = modMul(points[i].y, modMul(numerator, inv_denominator));
        result = modAdd(result, term);
        
        std::cout << "  Numerator: " << numerator << std::endl;
        std::cout << "  Denominator: " << denominator << std::endl;
        std::cout << "  Inverse: " << inv_denominator << std::endl;
        std::cout << "  Term: " << term << std::endl;
        std::cout << "  Running total: " << result << std::endl;
    }
    
    return result;
}

// JSON Parser (same as before)
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
        if (!result.empty() && result.front() == '"') result.erase(0, 1);
        if (!result.empty() && result.back() == '"') result.pop_back();
        if (!result.empty() && result.back() == ',') result.pop_back();
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
            content += line + " ";
        }
        file.close();
        
        // Parse keys section
        size_t keysStart = content.find("\"keys\"");
        if (keysStart != std::string::npos) {
            size_t keysEnd = content.find("}", keysStart);
            std::string keysSection = content.substr(keysStart, keysEnd - keysStart);
            
            // Extract n and k
            size_t nPos = keysSection.find("\"n\"");
            if (nPos != std::string::npos) {
                size_t colonPos = keysSection.find(":", nPos);
                size_t commaPos = keysSection.find(",", colonPos);
                if (commaPos == std::string::npos) commaPos = keysSection.find("}", colonPos);
                std::string nValue = keysSection.substr(colonPos + 1, commaPos - colonPos - 1);
                n = std::stoi(trim(nValue));
            }
            
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
            pos++;
            size_t endQuote = content.find("\"", pos);
            if (endQuote == std::string::npos) break;
            
            std::string key = content.substr(pos, endQuote - pos);
            pos = endQuote + 1;
            
            if (key == "keys" || key == "n" || key == "k" || key == "base" || key == "value") {
                continue;
            }
            
            try {
                int rootNum = std::stoi(key);
                
                size_t braceStart = content.find("{", pos);
                if (braceStart == std::string::npos) continue;
                
                size_t braceEnd = content.find("}", braceStart);
                if (braceEnd == std::string::npos) continue;
                
                std::string rootContent = content.substr(braceStart + 1, braceEnd - braceStart - 1);
                
                RootData rootData;
                
                // Extract base
                size_t basePos = rootContent.find("\"base\"");
                if (basePos != std::string::npos) {
                    size_t colonPos = rootContent.find(":", basePos);
                    size_t commaPos = rootContent.find(",", colonPos);
                    if (commaPos == std::string::npos) commaPos = rootContent.length();
                    std::string baseValue = rootContent.substr(colonPos + 1, commaPos - colonPos - 1);
                    rootData.base = std::stoi(removeQuotes(trim(baseValue)));
                }
                
                // Extract value
                size_t valuePos = rootContent.find("\"value\"");
                if (valuePos != std::string::npos) {
                    size_t colonPos = rootContent.find(":", valuePos);
                    size_t endPos = rootContent.length();
                    std::string valueStr = rootContent.substr(colonPos + 1, endPos - colonPos - 1);
                    rootData.value = removeQuotes(trim(valueStr));
                }
                
                if (rootData.base > 0 && !rootData.value.empty()) {
                    roots[rootNum] = rootData;
                }
                
                pos = braceEnd;
                
            } catch (...) {
                continue;
            }
        }
    }
};

int main() {
    try {
        int n = 0, k = 0;
        std::map<int, RootData> roots;
        
        std::cout << "=== Shamir's Secret Sharing with Modular Arithmetic ===" << std::endl;
        
        JSONParser::parseFile("input.json", n, k, roots);
        
        std::cout << "n (number of roots provided): " << n << std::endl;
        std::cout << "k (minimum roots needed): " << k << std::endl;
        std::cout << "Using modular arithmetic with prime: " << MOD << std::endl;
        std::cout << std::endl;
        
        if (roots.empty()) {
            std::cout << "ERROR: No roots were parsed from the JSON file!" << std::endl;
            return 1;
        }
        
        // Convert roots to points using modular arithmetic
        std::vector<Point> points;
        
        std::cout << "Converting roots to decimal points (mod " << MOD << "):" << std::endl;
        for (const auto& root : roots) {
            int x = root.first;
            int base = root.second.base;
            std::string value = root.second.value;
            
            try {
                long long y = baseToDecimal(value, base);
                points.push_back(Point(x, y));
                
                std::cout << "Root " << x << ": base " << base 
                          << ", value \"" << value << "\"" << std::endl;
                std::cout << "  -> (" << x << ", " << y << ")" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Error converting Root " << x << ": " << e.what() << std::endl;
            }
        }
        std::cout << std::endl;
        
        if ((int)points.size() < k) {
            std::cout << "ERROR: Not enough valid points!" << std::endl;
            return 1;
        }
        
        // Sort points and use first k
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
        });
        
        std::vector<Point> selectedPoints(points.begin(), points.begin() + k);
        
        std::cout << "Using first " << k << " points:" << std::endl;
        for (int i = 0; i < k; i++) {
            std::cout << "(" << selectedPoints[i].x << ", " << selectedPoints[i].y << ")";
            if (i < k - 1) std::cout << ", ";
        }
        std::cout << std::endl;
        
        // Use modular Lagrange interpolation
        long long secret = lagrangeInterpolationModular(selectedPoints, k);
        
        std::cout << "\n=== RESULT ===" << std::endl;
        std::cout << "The constant term (secret) is: " << secret << std::endl;
        std::cout << "Note: Result is computed modulo " << MOD << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}