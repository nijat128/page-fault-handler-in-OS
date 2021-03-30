# page-fault-handler-in-OS
Implement two simplified PageFaultHandlers for an OS

Your task is to implement two simplified PageFaultHandlers for an OS. Your program
will react to the sequence of the virtual page references provided on the standard input, and
implement:
- local page replacement algorithm using Clock replacement strategy; and
- global page replacement algorithm using FIFO strategy

A virtual page reference has the following format:
<process id> <VPNumber> where
• <process id> is a nonnegative integer, unique for each process
• <VPNumber> is a nonnegative integer
  
The first line of input contains the word LOCAL or GLOBAL. The second line of input contains
an integer M > 2 (number of frames given to a single process for use in local replacements, or
total number of frames in the system in global replacement).
Here is an example of several first lines of the input (comments are here for explanation
only):
LOCAL
3
1 0 \\ process 1 generates access to Virtual Page 0
1 1 \\ process 1 generates access to VP 1
1 4 \\ process 1 generates access to VP 4
2 0 \\ process 2 generates access to VP 0
1 0 \\ process 1 generates access to VP 0
1 4 \\ ...
2 1
3 0
...
Your PageFaultHandler will change processes page tables, if a particular page reference is a
PageFault. It will also accumulate information on the number of page replacements per process.
When all lines of the input are processed (empty line is the signal of the end of input), your
program will print the following information for each process:
<process id> <Number of Page faults>
and content of each process PageTable (valid entries only!!!) at the end of processing. You can
assume that at the beginning all frames (frame numbers start with 0) are free and in the case
of local replacement there are enough free frames to admit all processes mentioned in the input.
For example, if the input is
1LOCAL
3
1 0
1 1
1 2
2 0
2 1
2 2
your program will print
PID Page Faults
1 3
2 3
Process 1 page table
Page Frame
0 0
1 1
2 2
Process 2 page table
Page Frame
0 3
1 4
2 5
If the same input starts with line GLOBAL your program will print (note that the process 1
page table is empty - process 1 has no valid page-table entries as the result of global replacement
in this case):
PID Page Faults
1 3
2 3
Process 1 page table
Page Frame
Process 2 page table
Page Frame
0 0
1 1
2 2
  
Do not assume any limit on the number of lines in the input!
Do not assume that process IDs appear in the input in any particular order, or that
process IDs form a sequence of consequtive numbers. You can have processes with
IDs 20, 11, 3, and 5 in the input, and only these need to be handled and printed.

