LLVM Dead Loop Deletion Pass

This pass identifies and removes dead loops. These loops include:
  1. loops that never execute (e.g. counter is 0)
  2. loops that have no meaningful instructions (e.g. loops that have no call instruction or arithmetic opperation)
  3. loops that only contain useless instructions (e.g. addition with 0, multiplication with 1, deletion with zero...)
 
