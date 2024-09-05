add_llvm_library( LLVMDeadLoopDeletionPass MODULE BUILDTREE_ONLY
  DeadLoopDeletionPass.cpp

  PLUGIN_TOOL
  opt
  )
