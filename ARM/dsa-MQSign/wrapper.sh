make clean

export BUILT_OPT="-D_MQS256_72_46"
make
./bench
make clean

export BUILT_OPT="-D_MQS256_112_72"
make
./bench
make clean

export BUILT_OPT="-D_MQS256_148_96"
make
./bench
make clean