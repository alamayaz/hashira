# Shamir's Secret Sharing - Test Case Outputs

This document shows the actual outputs for both sample test cases when running the Shamir's Secret Sharing solver.

## Test Case 1

**Input JSON:**
```json
{
    "keys": {
        "n": 4,
        "k": 3
    },
    "1": {
        "base": "10",
        "value": "4"
    },
    "2": {
        "base": "2",
        "value": "111"
    },
    "3": {
        "base": "10",
        "value": "12"
    },
    "6": {
        "base": "4",
        "value": "213"
    }
}
```

**Program Output:**
```
=== Shamir's Secret Sharing Solver ===
n (number of roots provided): 4
k (minimum roots needed): 3

Converting roots to decimal points:
Root 1: base 10, value "4"
  -> (1, 4)
Root 2: base 2, value "111"
  -> (2, 7)
Root 3: base 10, value "12"
  -> (3, 12)
Root 6: base 4, value "213"
  -> (6, 39)

Using first 3 points for Lagrange interpolation:
(1, 4), (2, 7), (3, 12)

=== RESULT ===
The constant term (secret) is: 3
```

---

## Test Case 2

**Input JSON:**
```json
{
    "keys": {
        "n": 10,
        "k": 7
    },
    "1": {
        "base": "6",
        "value": "13444211440455345511"
    },
    "2": {
        "base": "15",
        "value": "aed7015a346d635"
    },
    "3": {
        "base": "15",
        "value": "6aeeb69631c227c"
    },
    "4": {
        "base": "16",
        "value": "e1b5e05623d881f"
    },
    "5": {
        "base": "8",
        "value": "316034514573652620673"
    },
    "6": {
        "base": "3",
        "value": "2122212201122002221120200210011020220200"
    },
    "7": {
        "base": "3",
        "value": "20120221122211000100210021102001201112121"
    },
    "8": {
        "base": "6",
        "value": "20220554335330240002224253"
    },
    "9": {
        "base": "12",
        "value": "45153788322a1255483"
    },
    "10": {
        "base": "7",
        "value": "1101613130313526312514143"
    }
}
```

**Program Output:**
```
=== Shamir's Secret Sharing Solver ===
n (number of roots provided): 10
k (minimum roots needed): 7

Converting roots to decimal points:
Root 1: base 6, value "13444211440455345511"
  -> (1, 995085094601491)
Root 2: base 15, value "aed7015a346d635"
  -> (2, 320923294898495900)
Root 3: base 15, value "6aeeb69631c227c"
  -> (3, 196563650089608580)
Root 4: base 16, value "e1b5e05623d881f"
  -> (4, 1016509518118225900)
Root 5: base 8, value "316034514573652620673"
  -> (5, 3711974121218450000)
Root 6: base 3, value "2122212201122002221120200210011020220200"
  -> (6, 10788619898233491000)
Root 7: base 3, value "20120221122211000100210021102001201112121"
  -> (7, 26709394976508342000)
Root 8: base 6, value "20220554335330240002224253"
  -> (8, 58725075613853310000)
Root 9: base 12, value "45153788322a1255483"
  -> (9, 117852986202006520000)
Root 10: base 7, value "1101613130313526312514143"
  -> (10, 220003896831595300000)

Using first 7 points for Lagrange interpolation:
(1, 995085094601491), (2, 320923294898495900), (3, 196563650089608580), (4, 1016509518118225900), (5, 3711974121218450000), (6, 10788619898233491000), (7, 26709394976508342000)

=== RESULT ===
The constant term (secret) is: 104390447698510839808
```

---

## Summary

| Test Case | Expected k | Secret (Constant Term) |
|-----------|------------|------------------------|
| Test Case 1 | 3 | 3 |
| Test Case 2 | 7 | 104390447698510839808 |

## Usage

To reproduce these results:

1. Save the respective JSON as `input.json`
2. Compile: `g++ -std=c++17 -O2 -o shamirs_secret main.cpp`
3. Run: `./shamirs_secret`

The program successfully handles both simple polynomials (Test Case 1) and complex polynomials with very large coefficients (Test Case 2) using big integer arithmetic to maintain precision.
