; ModuleID = '1.c'
source_filename = "1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 0, ptr %2, align 4
  store i32 3, ptr %3, align 4
  br label %4

4:                                                ; preds = %7, %0
  %5 = load i32, ptr %3, align 4
  %6 = add nsw i32 %5, 3
  store i32 %6, ptr %3, align 4
  br label %7

7:                                                ; preds = %4
  %8 = load i32, ptr %2, align 4
  %9 = icmp slt i32 %8, 10
  br i1 %9, label %4, label %10, !llvm.loop !6

10:                                               ; preds = %7
  br label %11

11:                                               ; preds = %12, %10
  br label %12

12:                                               ; preds = %11
  %13 = load i32, ptr %2, align 4
  %14 = icmp slt i32 %13, 10
  br i1 %14, label %11, label %15, !llvm.loop !8

15:                                               ; preds = %12
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 17.0.0"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
