; ModuleID = '4.c'
source_filename = "4.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 0, ptr %2, align 4
  br label %3

3:                                                ; preds = %6, %0
  %4 = load i32, ptr %2, align 4
  %5 = icmp slt i32 %4, 20
  br i1 %5, label %6, label %9

6:                                                ; preds = %3
  %7 = load i32, ptr %2, align 4
  %8 = add nsw i32 %7, 1
  store i32 %8, ptr %2, align 4
  br label %3, !llvm.loop !6

9:                                                ; preds = %3
  br label %10

10:                                               ; preds = %13, %9
  %11 = load i32, ptr %2, align 4
  %12 = icmp slt i32 %11, 30
  br i1 %12, label %13, label %14

13:                                               ; preds = %10
  br label %10, !llvm.loop !8

14:                                               ; preds = %10
  br label %15

15:                                               ; preds = %16, %14
  br label %16

16:                                               ; preds = %15
  %17 = load i32, ptr %2, align 4
  %18 = icmp slt i32 %17, 30
  br i1 %18, label %15, label %19, !llvm.loop !9

19:                                               ; preds = %16
  %20 = load i32, ptr %1, align 4
  ret i32 %20
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
!9 = distinct !{!9, !7}
