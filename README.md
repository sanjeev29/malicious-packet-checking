# Malicious Packet Checking

Malicious Packet Checking is a C++ project designed to identify and manage malicious IP addresses using advanced algorithms like SHA256 and a Random Bloom Filter (RBF) data structure. The repository provides functionalities to generate and embed malicious IP addresses into a Bloom Filter, ensuring efficient storage and retrieval.

## How It Works

1. **IP Generation**: Generates 10,000 random IP addresses.
2. **SHA256 Hashing**: Uses SHA256 to map data into the Recursive Bloom Filter.
3. **Bloom Filter Operations**:
   - Inserts the generated IP addresses into the filter.
   - Validates if an IP is malicious using the filter.

## Usage

### Example Commands

- Generate and check malicious IPs:
  ```
  ./RBFGen 10000 10000.txt
  ```

- Debugging:
  Modify the `DEBUG` flags in `main.cpp` for verbose output.
