if (( $# != 1)); then
echo "<ERROR> Usage: ./runRangeAnalysis.sh BenchmarkName"
exit
fi

if [ ! -f Benchmarks/$1'.cpp' ]; then
    echo "Benchmark does not exist!"
    exit
fi

clang -O0 -emit-llvm -c Benchmarks/$1'.cpp' -o Benchmarks/$1'.bc'
opt -load $LLVMLIB/CSE231.so -mem2reg < Benchmarks/$1'.bc' > Benchmarks/$1'_mem2reg.bc'
opt -load $LLVMLIB/CSE231.so -rangeAnalysis < Benchmarks/$1'_mem2reg.bc' > Benchmarks/$1'_rangeAnalysis.bc'
llvm-dis Benchmarks/$1'_rangeAnalysis.bc' -o Benchmarks/$1'_mem2reg.ll'

echo -e "The above output has lattice information at the beginning and end of every basic block and every instruction\n"
echo -e "We ran mem2reg on the benchmark before executing the Range Analysis pass. mem2reg output available at Benchmarks/benchmarkName_mem2reg.ll\n"
