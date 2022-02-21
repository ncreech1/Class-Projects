all: tip-fw-bits-packed-SIMD \
    ap-flow-simd \


clean:
	rm -f \
    tip-fw-bits-packed-SIMD  \
    ap-flow-simd \

ap-flow-fw: AP-Flow-Main.cpp AP-Flow.h MOA.c MOA.h AP-Flow-FW.cpp
	g++ -O2 -o ap-flow-fw AP-Flow-Main.cpp MOA.c AP-Flow-FW.cpp

ap-flow-simd: AP-Flow-Main.cpp AP-Flow.h MOA.c MOA.h AP-Flow-SIMD.cpp
	g++ -O2 -o ap-flow-simd AP-Flow-Main.cpp MOA.c AP-Flow-SIMD.cpp -msse2

ap-flow-d: AP-Flow-Main.cpp AP-Flow.h MOA.c MOA.h AP-Flow-Dijkstra.cpp
	g++ -O2 -o ap-flow-d AP-Flow-Main.cpp MOA.c AP-Flow-Dijkstra.cpp

tip-fw-if: The-Tips-Floyd-If.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-fw-if The-Tips-Floyd-If.cpp The-Tips-Main.cpp

tip-fw-bits-packed-SIMD: The-Tips-Floyd-Bits-Packed-SIMD.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -msse2 -o tip-fw-bits-packed-SIMD The-Tips-Floyd-Bits-Packed-SIMD.cpp The-Tips-Main.cpp

tip-fw-bits-packed-64: The-Tips-Floyd-Bits-Packed-64.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-fw-bits-packed-64 The-Tips-Floyd-Bits-Packed-64.cpp The-Tips-Main.cpp

tip-fw-bits-packed-32: The-Tips-Floyd-Bits-Packed-32.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-fw-bits-packed-32 The-Tips-Floyd-Bits-Packed-32.cpp The-Tips-Main.cpp

tip-fw-bits-packed-16: The-Tips-Floyd-Bits-Packed-16.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-fw-bits-packed-16 The-Tips-Floyd-Bits-Packed-16.cpp The-Tips-Main.cpp

tip-fw-bits-packed-08: The-Tips-Floyd-Bits-Packed-08.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-fw-bits-packed-08 The-Tips-Floyd-Bits-Packed-08.cpp The-Tips-Main.cpp

tip-fw-bits-2: The-Tips-Floyd-Bits-2.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-fw-bits-2 The-Tips-Floyd-Bits-2.cpp The-Tips-Main.cpp

tip-fw-bits: The-Tips-Floyd-Bits.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-fw-bits The-Tips-Floyd-Bits.cpp The-Tips-Main.cpp

tip-dfs-stack-1: The-Tips-DFS-Stack-1.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-dfs-stack-1 The-Tips-DFS-Stack-1.cpp The-Tips-Main.cpp

tip-dfs-stack-2: The-Tips-DFS-Stack-2.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-dfs-stack-2 The-Tips-DFS-Stack-2.cpp The-Tips-Main.cpp

tip-dfs-r: The-Tips-DFS-R.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-dfs-r The-Tips-DFS-R.cpp The-Tips-Main.cpp

tip-dfs-r2: The-Tips-DFS-R2.cpp The-Tips-Main.cpp The-Tips.h
	g++ -O2 -o tip-dfs-r2 The-Tips-DFS-R2.cpp The-Tips-Main.cpp
