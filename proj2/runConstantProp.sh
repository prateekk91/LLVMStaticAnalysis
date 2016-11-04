if (( $# != 1)); then
echo "<ERROR> Usage: ./runConstantProp.sh BenchmarkName"
exit
fi

if [ ! -f Benchmarks/$1'.cpp' ]; then
    echo "Benchmark does not exist!"
    exit
fi

clang -O0 -emit-llvm -c Benchmarks/$1'.cpp' -o Benchmarks/$1'.bc'
opt -load $LLVMLIB/CSE231.so -mem2reg < Benchmarks/$1'.bc' > Benchmarks/$1'_mem2reg.bc'
opt -load $LLVMLIB/CSE231.so -constantProp < Benchmarks/$1'_mem2reg.bc' > Benchmarks/$1'_constantProp.bc'
llvm-dis Benchmarks/$1'_constantProp.bc' -o Benchmarks/$1'_constantProp.ll'
llvm-dis Benchmarks/$1'_mem2reg.bc' -o Benchmarks/$1'_mem2reg.ll'

echo -e "The above output has lattice information at the beginning and end of every basic block and every instruction\n"
echo -e "We ran mem2reg on the benchmark before executing the Constant Propagation pass. mem2reg output available at Benchmarks/benchmarkName_mem2reg.ll\n"
echo -e "Transformend program available at Benchmarks/benchmarkName_constantProp.ll\n"
