ulimit -c unlimited
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./bin/gen 
