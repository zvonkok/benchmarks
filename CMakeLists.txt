cmake_minimum_required (VERSION 2.6)

project(benchmarks)

# echo "" | gcc -march=native -v -E - 2>&1 | grep cc1
set(CMAKE_C_CFLAGS "-march=native -O3 -pipe")
set(CMAKE_CXX_CFLAGS "-march=native -O3 -pipe")

include_directories(. hint)


foreach(w 100 200 300)
  foreach(t int16_t int32_t int64_t)
	add_executable(bresenham_${w}_${t} bresenham.c)
	set_target_properties(bresenham_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={float,double,long double}
	message(bresenham_${w}_${t})
  endforeach()
endforeach()

foreach(w 100000 1000000 10000000)
  foreach(t float double)
	add_executable(cmset_${w}_${t} cmset.c)
	set_target_properties(cmset_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={float,double,long double}
	message(cmset_${w}_${t})
  endforeach()
endforeach()

foreach(w 100000 1000000 10000000)
  foreach(t float double)
	add_executable(cumset_${w}_${t} cumset.c)
	set_target_properties(cumset_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={float,double,long double}
	message(cumset_${w}_${t})
  endforeach()
endforeach()

foreach(w 100000 1000000 10000000)
  foreach(t float double)
	add_executable(mset_${w}_${t} mset.c)
	set_target_properties(mset_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={float,double,long double}
	message(mset_${w}_${t})
  endforeach()
endforeach()

foreach(w 100000 1000000 10000000)
  foreach(t float double)
	add_executable(umset_${w}_${t} umset.c)
	set_target_properties(umset_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={float,double,long double}
 	message(umset_${w}_${t})
  endforeach()
endforeach()

foreach(w 1000000 10000000 100000000)
  foreach(t int16_t int32_t int64_t)
	add_executable(dataconv_${w}_${t} dataconv.c)
	set_target_properties(dataconv_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "FROM=${t};TO=float;WORKLOAD=${w}") #FROM={signed char,short,long,long long,float,double}
	#TO={float,double,long}
	message(dataconv_${w}_${t})
  endforeach()
endforeach()

foreach(w 100000 1000000 10000000)
  foreach(t float double)
	add_executable(fpconmix_${w}_${t} fpconmix.c)
	set_target_properties(fpconmix_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={float,double,long double}
	message(fpconmix_${w}_${t})
  endforeach()
endforeach()

foreach(w 100000 1000000 10000000)
  foreach(t int16_t int32_t int64_t)
	add_executable(fpintmix_${w}_${t} fpintmix.c)
	set_target_properties(fpintmix_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}")  #TYPE={short,int,long,long long}
	
	message(fpintmix_${w}_${t})
  endforeach()
endforeach()


foreach(w 4 8 12)
  foreach(t int16_t int32_t int64_t)
	add_executable(hanoi_${w}_${t} hanoi.c)
	set_target_properties(hanoi_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={signed char,short,int,long,long long,unsigned char},
	#TYPE={unsigned short, unsigned int, unsigned long, unsigned long long}
	
	message(hanoi_${w}_${t})
  endforeach()
endforeach()


foreach(w 10000 100000 1000000)
  add_executable(inlining_${w} inlining.c)
  set_target_properties(inlining_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}")
  message(inlining_${w})
endforeach()




foreach(w 1 2 3)
  add_executable(pal_${w} pal.c)
  set_target_properties(pal_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}")
  message(pal_${w})
endforeach()


foreach(w 1024 2048 4096)
  foreach(t int16_t int32_t int64_t float double)
	add_executable(qsort_${w}_${t} qsort.c)
	set_target_properties(qsort_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={signed char,short,int,long,long long,unsigned char},
	#TYPE={unsigned short, unsigned int, unsigned long, unsigned long long},TYPE={float,double,long double}
	
	message(qsort_${w}_${t})
  endforeach()
endforeach()


foreach(w 100000 1000000 10000000)
  foreach(t int16_t int32_t int64_t)
	add_executable(reg_assign_${w}_${t} reg_assign.c)
	set_target_properties(reg_assign_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={char,short,int,long,long long},TYPE={float,double}
	message(reg_assign_${w}_${t})
  endforeach()
endforeach()

foreach(w 1000 10000 100000)
  add_executable(xaos_${w} xaos.c)
  set_target_properties(xaos_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}")
  message(xaos_${w})
endforeach()


foreach(w 10000 100000 1000000)
  foreach(t int16_t int32_t int64_t)
	add_executable(arith_${w}_${t} arith.c)
	set_target_properties(arith_${w}_${t} PROPERTIES COMPILE_DEFINITIONS 
	  "ITYPE=${t};WORKLOAD=${w}") #ITYPE={signed char,short,int,long,long long,unsigned char},ITYPE={unsigned short, unsigned int, unsigned long, unsigned long long}
	message(arith_${w}_${t})
  endforeach()  
endforeach()

foreach(w 100000 1000000 10000000)
  foreach(t int32_t int64_t)
	add_executable(sieve_${w}_${t} sieve.c)
	set_target_properties(sieve_${w}_${t} PROPERTIES COMPILE_DEFINITIONS 
	  "ITYPE=${t};WORKLOAD=${w}") #ITYPE={short,int,long,long long},
    #ITYPE={unsigned short, unsigned int, unsigned long}
	message(sieve_${w}_${t})
  endforeach()
endforeach()

foreach(w 24 25 26)
  foreach(t int16_t int32_t int64_t float double)
	add_executable(fibo_${w}_${t} fibo.c)	
	set_target_properties(fibo_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "TYPE=${t};WORKLOAD=${w}") #TTYPE={signed char,short,int,long,long long},
	#TYPE={float,double,long double}  
	message(fibo_${w}_${t})
  endforeach()
endforeach()

foreach(w 65536 131072 262144)
  foreach(t float double)
	add_executable(fft_${w}_${t} fft.c)
	set_target_properties(fft_${w}_${t} PROPERTIES COMPILE_DEFINITIONS 
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={float,double,long double}
	message(fft_${w}_${t})
  endforeach()
endforeach()

foreach(w 1000 10000 100000)
  foreach(t float double)
	add_executable(whetstone_${w}_${t} whetstone.c)
	set_target_properties(whetstone_${w}_${t} PROPERTIES COMPILE_DEFINITIONS 
	  "TYPE=${t};WORKLOAD=${w}") #TYPE={float,double,long double}
	target_link_libraries(whetstone_${w}_${t} m)
	message(whetstone_${w}_${t})
	
  endforeach()
endforeach()

foreach(w 50 100 150)
  foreach(t SP DP)
	add_executable(linpack_${w}_${t} linpack.c)
	set_target_properties(linpack_${w}_${t} PROPERTIES COMPILE_DEFINITIONS 
	  "${t};WORKLOAD=${w}") #TYPE={SP, DP}
	message(linpack_${w}_${t})
  endforeach()
endforeach()


foreach(w 50000 150000 200000)
  add_executable(dhrystone_${w} dhry/dry.c)
  add_executable(dhrystone_${w}_register dhry/dry.c)
  set_target_properties(dhrystone_${w}_register PROPERTIES COMPILE_DEFINITIONS 
	"REG=register;WORKLOAD=${w}") #TYPE={register,""}
  set_target_properties(dhrystone_${w} PROPERTIES COMPILE_DEFINITIONS 
	"WORKLOAD=${w}") 
  message(dhrystone_${w})
  message(dhrystone_${w}_register)
endforeach()

foreach(w 100 200 300)
  add_executable(slalom_${w} slalom.c)
  set_target_properties(slalom_${w} PROPERTIES COMPILE_DEFINITIONS 
	"WORKLOAD=${w}") 
  target_link_libraries(slalom_${w} m)
  message(slalom_${w})
endforeach()


foreach(w 6 7 8)
  foreach(t int16_t int32_t)
	add_executable(b1_${w}_${t} bell-labs/b1.c)
	set_target_properties(b1_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "ITYPE=${t};WORKLOAD=${w}") #ITYPE={short,int,long,long long},
    #ITYPE={unsigned short, unsigned int, unsigned long}
	message(b1_${w}_${t})						 
  endforeach()
endforeach()

foreach(w 600 700 800)
  add_executable(b2_${w} bell-labs/b2.c)
  set_target_properties(b2_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}") 
  message(b2_${w})		
endforeach()

foreach(w 30000 40000 50000)
  add_executable(b3_${w} bell-labs/b3.c)
  set_target_properties(b3_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}") 
  message(b3_${w})		
endforeach()

foreach(w 30000 40000 50000)
  add_executable(b4_${w} bell-labs/b4.c)
  set_target_properties(b4_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}") 
  message(b4_${w})		
endforeach()

foreach(w 30000 40000 50000)
  add_executable(b5_${w} bell-labs/b5.c)
  set_target_properties(b5_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}") 
  message(b5_${w})		
endforeach()

foreach(w 100000 1000000 10000000)
  add_executable(b6_${w} bell-labs/b6.c)
  set_target_properties(b6_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}") 
  message(b6_${w})		
endforeach()

foreach(w 30000 40000 50000)
  foreach(t int16_t int32_t int64_t)
	add_executable(b7_${w}_${t} bell-labs/b7.c)
	set_target_properties(b7_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "ITYPE=${t};WORKLOAD=${w}") 
	message(b7_${w}_${t})		
  endforeach()
endforeach()

foreach(w 10 20 30)
  add_executable(stream_${w} stream.c)
  set_target_properties(stream_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}") 
  message(stream_${w})	
endforeach()

foreach(w 1 2 3)
  foreach(t INT FLOAT DOUBLE)
	add_executable(hint_${w}_${t} hint/hint.c hint/hkernel.c)
	set_target_properties(hint_${w}_${t} PROPERTIES COMPILE_DEFINITIONS
	  "DEBUG;UNIX;IINT;${t};WORKLOAD=${w}") #INT,FLOAT,DOUBLE
	message(hint_${w}_${t})
  endforeach()
endforeach()

foreach(w 2000 3000 4000)
  add_executable(stepanov_${w} stepanov.cpp)
  set_target_properties(stepanov_${w} PROPERTIES COMPILE_DEFINITIONS
	"WORKLOAD=${w}")
  message(stepanov_${w})
endforeach()


add_executable(puzzle puzzle.c)
message(puzzle)
add_executable(oopack oopack.cpp)
message(oopack)

add_executable(lldouble livermore_loops/block.c
  livermore_loops/ccernel.c
  livermore_loops/cflopio.c
  livermore_loops/cflops.c)
message(lldouble)

add_executable(llfloat  livermore_loops/block.c
  livermore_loops/ccernel.c
  livermore_loops/cflopio.c
  livermore_loops/cflops.c)
message(llfloat)

set_target_properties(lldouble PROPERTIES COMPILE_DEFINITIONS "REAL=double;ANSI_TIMER=1")
set_target_properties(llfloat PROPERTIES COMPILE_DEFINITIONS "REAL=float;ANSI_TIMER=1")

target_link_libraries(lldouble m)
target_link_libraries(llfloat m)














