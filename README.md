# Key-Value Storage on Higher Dimensional Data

An efficient storage and query system for higher-dimensional data using Generalized Two-dimensional Array (G2A) and key-value storage techniques. This project was developed as part of an undergraduate thesis research work.

>  For detailed theoretical background, experimental results, and comprehensive analysis, please refer to [Thesis_Paper.pdf](Thesis_Paper.pdf) included in this repository.

## 📋 Table of Contents

- [Overview](#overview)
- [Problem Statement](#problem-statement)
- [Solution Approach](#solution-approach)
- [G2A (Generalized Two-dimensional Array)](#g2a-generalized-two-dimensional-array)
- [Key-Value Storage Scheme](#key-value-storage-scheme)
- [Installation](#installation)
- [Usage](#usage)
- [File Structure](#file-structure)
- [Algorithm Details](#algorithm-details)
- [Query Operations](#query-operations)
- [Performance](#performance)

## Overview

This project addresses the **curse of dimensionality** problem in high-dimensional data storage and retrieval. Traditional multidimensional arrays suffer from poor performance when the number of dimensions is very high. This system uses:

- **G2A (Generalized Two-dimensional Array)**: Converts n-dimensional data into a 2D representation
- **Key-Value Storage**: Generates unique 64-bit keys for efficient storage and retrieval
- **Bin-based File System**: Stores keys in fixed-size bins for optimized query performance

## Problem Statement

Higher-dimensional data faces several challenges:
- **Data sparsity** increases with dimensions
- **Index computation** becomes expensive
- **Memory allocation** is inefficient
- **Query operations** are slow on linearized data

## Solution Approach

Our solution uses a three-step process to efficiently store and query high-dimensional data:

1. **G2A Transformation**: Convert n-dimensional data to 2D coordinates
2. **Key Generation**: Create unique 64-bit keys from 2D coordinates
3. **Bin Storage**: Organize keys into fixed-size bins for fast retrieval

---

## G2A (Generalized Two-dimensional Array)

### What is G2A?

G2A is a technique that converts an n-dimensional array into a 2-dimensional array. Instead of working with all n dimensions directly, we split them into two groups and create a 2D representation.

### How It Works

**Step 1: Split Dimensions**
- **Group 1 (x0)**: Even-indexed columns (0, 2, 4, 6, ...)
- **Group 2 (x1)**: Odd-indexed columns (1, 3, 5, 7, ...)

**Step 2: Calculate 2D Coordinates**

For a dataset with 7 columns (indices 0-6), we calculate:

**x0 (Row coordinate)** - Uses columns 0, 2, 4, 6:
```
x0 = (index[0] × size[2] × size[4] × size[6]) + 
     (index[2] × size[4] × size[6]) + 
     (index[4] × size[6]) + 
     index[6]
```

**x1 (Column coordinate)** - Uses columns 1, 3, 5:
```
x1 = (index[1] × size[3] × size[5]) + 
     (index[3] × size[5]) + 
     index[5]
```

**Example:**
```
If we have a record with indices: [2, 1, 0, 3, 1, 2, 0]
And column sizes: [10, 5, 8, 4, 6, 3, 2]

x0 = (2 × 8 × 6 × 2) + (0 × 6 × 2) + (1 × 2) + 0
   = 192 + 0 + 2 + 0 = 194

x1 = (1 × 4 × 3) + (3 × 3) + 2
   = 12 + 9 + 2 = 23

Result: 2D coordinate (194, 23)
```

**Step 3: Create 2D Matrix**

The result is a 2D matrix of size `a × b`:
- `a` = size[0] × size[2] × size[4] × size[6] (product of even-indexed columns)
- `b` = size[1] × size[3] × size[5] (product of odd-indexed columns)

---

## Key Generation

### Overview

Each record gets a **unique 64-bit key** that represents its position in the 2D G2A matrix. This key is used for all storage and query operations.

### Key Structure

A 64-bit key consists of two 32-bit parts:

```
┌─────────────────────────────────────────────────────────────┐
│                    64-bit Key                               │
├──────────────────────────┬──────────────────────────────────┤
│   x0 (32 bits)           │   x1 (32 bits)                   │
│   High-order bits        │   Low-order bits                 │
│   (Row coordinate)       │   (Column coordinate)            │
└──────────────────────────┴──────────────────────────────────┘
```

### Key Generation Formula

```
Key = (x0 << 32) | x1
```

**Breaking it down:**
1. **x0 << 32**: Shift x0 left by 32 bits (moves it to upper half)
2. **| x1**: Combine with x1 using bitwise OR (places x1 in lower half)

**Example:**
```
x0 = 194 (binary: 0000...11000010)
x1 = 23  (binary: 0000...00010111)

After x0 << 32:
x0 = 11000010...00000000 (32 zeros on right)

After | x1:
Key = 11000010...00000000 | 00000000...00010111
    = 11000010...00010111

Final 64-bit key: 833,658,368,023 (decimal)
```

### Why This Works

- **Uniqueness**: Each (x0, x1) pair is unique → each key is unique
- **Efficiency**: 64-bit keys fit in standard integer types
- **Fast Operations**: Bitwise operations are extremely fast
- **Reversibility**: Can extract x0 and x1 from key if needed

---

## Key-Value Storage Scheme

### Complete Process Flow

```
┌─────────────┐
│ dataset.csv │  (Original high-dimensional data)
└──────┬──────┘
       │
       ▼
┌─────────────────────────┐
│ Extract Unique Values   │  For each column, find all unique values
│ per Column              │  Example: Column 0 → [106, 107, 108, ...]
└──────┬──────────────────┘
       │
       ▼
┌─────────────────────────┐
│ Map Values to Indices   │  Each value gets an index in its column
│                         │  Example: 106 → index 0, 107 → index 1
└──────┬──────────────────┘
       │
       ▼
┌─────────────────────────┐
│ Apply G2A Transformation│  Convert nD indices → 2D (x0, x1)
│                         │  Example: [2,1,0,3,1,2,0] → (194, 23)
└──────┬──────────────────┘
       │
       ▼
┌─────────────────────────┐
│ Generate 64-bit Key     │  Combine x0 and x1 into single key
│                         │  Key = (x0 << 32) | x1
└──────┬──────────────────┘
       │
       ▼
┌─────────────────────────┐
│ Store in Bins           │  Group keys into bins of 100
│                         │  Write to v0.bin, v1.bin, ...
└──────┬──────────────────┘
       │
       ▼
┌─────────────────────────┐
│ Store Links             │  Save first key of each bin in links.txt
│                         │  (For fast bin lookup)
└─────────────────────────┘
```

### Detailed Steps

**Step 1: Extract Unique Values**
```
For each column in dataset:
    - Read all values
    - Store unique values in a set
    - Example: Column 0 has values [106, 107, 108, 106, 107]
              Unique set: [106, 107, 108]
```

**Step 2: Map Values to Indices**
```
For each record:
    - Find index of each value in its column's unique set
    - Example: Record has value 107 in column 0
              Index = 1 (second position in unique set)
```

**Step 3: Apply G2A Transformation**
```
Using the indices, calculate:
    x0 = formula using even-indexed columns
    x1 = formula using odd-indexed columns
```

**Step 4: Generate Key**
```
key = (x0 << 32) | x1
This creates a unique 64-bit identifier
```

**Step 5: Store in Bins**
```
- Collect keys in groups of 100
- Write each group to a binary file (v0.bin, v1.bin, ...)
- Store the first key of each bin in links.txt
```

---

## Bin Storage System

### What are Bins?

Bins are **fixed-size containers** that hold 100 keys each. They're stored as binary files on disk (secondary memory) for efficient I/O operations.

### Storage Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Primary Memory (RAM)                     │
│                                                             │
│  links.txt: [key1, key101, key201, key301, ...]             │
│  (First key of each bin - for fast lookup)                  │
└─────────────────────────────────────────────────────────────┘
                           │
                           │ Points to
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                 Secondary Memory (Disk)                     │
│                                                             │
│  v0.bin:  [key1,  key2,  ..., key100 ]  (100 keys)          │
│  v1.bin:  [key101, key102, ..., key200]  (100 keys)         │
│  v2.bin:  [key201, key202, ..., key300]  (100 keys)         │
│  ...                                                        │
└─────────────────────────────────────────────────────────────┘
```

### How It Works

**1. Key Storage:**
- Keys are written to binary files in groups of 100
- Each file is named `v0.bin`, `v1.bin`, `v2.bin`, etc.
- Binary format allows fast reading/writing

**2. Link Storage:**
- The **first key** of each bin is stored in `links.txt`
- This file stays in primary memory (RAM) for fast access
- Used to quickly determine which bin contains a key

**3. Query Process:**
```
To find a key:
1. Check links.txt (in RAM) → Find which bin might contain the key
2. Load that bin file from disk
3. Search within the bin (only 100 keys to check)
```

### Benefits

- **Fast Lookup**: Links in RAM allow quick bin identification
- **Efficient I/O**: Only load the specific bin needed
- **Scalable**: Can handle millions of keys across many bins
- **Memory Efficient**: Only 100 keys loaded at a time per query

### Storage Structure

```
dataset.csv
    ↓
[Extract unique values]
    ↓
[Generate keys using G2A]
    ↓
[Store in bins: v0.bin, v1.bin, ...]
    ↓
[Store links in links.txt]
```

## Installation

### Prerequisites

- C++ compiler (g++ recommended)
- Standard C++ libraries (C++11 or higher)
- CSV dataset file

### Build Instructions

1. **Compile the key generation program:**
   ```bash
   g++ -o create create.cpp
   ```

2. **Compile query programs:**
   ```bash
   g++ -o exact_match "exact match.cpp"
   g++ -o singlekeysearch singlekeysearch.cpp
   g++ -o rangevaluesearch rangevaluesearch.cpp
   ```

## Usage

### Step 1: Generate Keys

First, generate keys from your dataset:

```bash
./create
```

**Configuration:**
- Place your dataset in `dataset.csv`
- Modify line 59 in `create.cpp` to change the number of rows processed:
  ```cpp
  while(fgets(line, sizeof line, fp) != NULL && cnt<10000)
  ```
- The program will:
  - Read the dataset
  - Extract unique values for each column
  - Generate keys using G2A transformation
  - Store keys in bin files (`v0.bin`, `v1.bin`, ...)
  - Create `output.txt` (column mappings)
  - Create `links.txt` (bin links)
  - Create `keyvalues.txt` (all keys)

### Step 2: Run Queries

#### 1. Exact Match Query

Check if a specific record exists:

```bash
./exact_match
```

**Input Format:**
Enter values for each column (excluding primary key):
```
Please Enter the Number You Want to Search : :
1 th number : 106
2 th number : 09:04.1
3 th number : 400
...
```

**Output:**
- Key value generated
- Whether the record exists
- Bin file and index if found

#### 2. Single Key Query

Find all relationships for a specific value in one column:

```bash
./singlekeysearch
```

**Input Format:**
```
Enter the value column number you want to search: 1
Enter Value
106
```

**Output:**
- All related records that contain the specified value
- Shows relationships across other columns

#### 3. Range Key Query

Find relationships for values within a range:

```bash
./rangevaluesearch
```

**Input Format:**
```
Enter the range you want to search: 106 110
```

**Output:**
- All records with values in column 1 between 106 and 110
- Related records across other columns

## File Structure

```
Key-Value-Storage/
├── create.cpp              # Key generation and bin creation
├── exact match.cpp         # Exact match query
├── singlekeysearch.cpp     # Single key query
├── rangevaluesearch.cpp    # Range key query
├── dataset.csv             # Input dataset
├── output.txt              # Column mappings (generated)
├── links.txt               # Bin links (generated)
├── keyvalues.txt           # All keys (generated)
├── v0.bin, v1.bin, ...     # Bin files (generated)
└── README.md               # This file
```

## Algorithm Details

### Key Generation Algorithm

```cpp
// 1. Extract column indices
for each column:
    find index of value in unique values set

// 2. Apply G2A transformation
x0 = calculate from even-indexed columns
x1 = calculate from odd-indexed columns

// 3. Generate 64-bit key
key = (x0 << 32) | x1
```

### Bin Storage Algorithm

```cpp
// Store keys in bins of size 100
for each key:
    add to current bin
    if bin is full (100 keys):
        write bin to file (vN.bin)
        store first key in links.txt
        create new bin
```

### Query Algorithm

```cpp
// 1. Generate key from query input
key = generate_key_from_input()

// 2. Find appropriate bin
bin_number = binary_search(links, key)

// 3. Search in bin
load bin file
search for key in bin
if found: return record
else: return not found
```

## Query Operations

### 1. Exact Match Query

**Purpose:** Verify if a specific record exists in the dataset

**Process:**
1. User inputs all column values
2. System maps values to indices
3. Generates key using G2A formula
4. Searches for key in bins
5. Returns existence status

**Time Complexity:** O(log B + K) where B = number of bins, K = bin size

### 2. Single Key Query

**Purpose:** Find all relationships for a value in one column

**Process:**
1. User specifies column and value
2. System generates all permutations for other columns
3. For each permutation:
   - Generate key
   - Search in bins
   - If found, display relationship
4. Returns all matching records

**Time Complexity:** O(P × (log B + K)) where P = number of permutations

### 3. Range Key Query

**Purpose:** Find relationships for values within a range

**Process:**
1. User specifies range (e.g., 106-110)
2. System finds all indices within range
3. For each index:
   - Generate all permutations
   - Search for keys
   - Display matches
4. Returns all records in range

**Time Complexity:** O(R × P × (log B + K)) where R = range size

## Performance

### Storage Efficiency

- **Key-value storage** uses less memory than traditional indexing
- **Bin-based system** reduces I/O operations
- **Links in primary memory** enable fast bin lookup

### Query Performance

- **Exact match:** Very fast (O(log B + K))
- **Single key:** Fast for small datasets, depends on permutation count
- **Range query:** Efficient for small ranges

### Comparison with Traditional Methods

- **vs MySQL:** Faster for exact match queries
- **vs Bitmap Indexing:** More storage efficient

## Notes

- Primary key columns are automatically excluded from key generation
- Columns with constant values are excluded
- Bin size is fixed at 100 keys (can be modified in code)
- Range query currently supports only the first column


