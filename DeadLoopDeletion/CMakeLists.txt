add_llvm_library( LLVMDeadLoopDeletionPass MODULE BUILDTREE_ONLY
  DeadLoopDeletion.cpp

  PLUGIN_TOOL
  opt
  )
