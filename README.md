# AutoGenSECYAN
An automatic code generator for SECYAN algorithm
## Requirements
### For Debian Linux
 - build-essential (gcc >= 8)
 - cmake >= 3.12
 - libssl-dev
 - libgmp-dev
 - libboost-all-dev (Boost >= 1.66)

## Build Guide
### Step1: Build sql-parser to generate libsqlparser.so
```bash
cd sql-parser
make
```
### Step2: Build SECYAN and Autogen
```bash
mkdir build
cd build
cmake ..
make
```

## Run Guide
* Run autogen by:
```bash
cd build
./main
```
