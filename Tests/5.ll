; ModuleID = '5.c'
source_filename = "5.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 0, ptr %2, align 4
  br label %4

4:                                                ; preds = %7, %0
  %5 = load i32, ptr %2, align 4
  %6 = add nsw i32 %5, 1
  store i32 %6, ptr %2, align 4
  br label %7

7:                                                ; preds = %4
  %8 = load i32, ptr %2, align 4
  %9 = icmp slt i32 %8, 10
  br i1 %9, label %4, label %10, !llvm.loop !6

10:                                               ; preds = %7
  store i32 5, ptr %3, align 4
  br label %11

11:                                               ; preds = %14, %10
  %12 = load i32, ptr %3, align 4
  %13 = mul nsw i32 %12, 1
  store i32 %13, ptr %2, align 4
  br label %14

14:                                               ; preds = %11
  %15 = load i32, ptr %2, align 4
  %16 = icmp slt i32 %15, 10
  br i1 %16, label %11, label %17, !llvm.loop !8

17:                                               ; preds = %14
  store i32 5, ptr %3, align 4
  br label %18

18:                                               ; preds = %21, %17
  %19 = load i32, ptr %2, align 4
  %20 = mul nsw i32 %19, 1
  store i32 %20, ptr %2, align 4
  br label %21

21:                                               ; preds = %18
  %22 = load i32, ptr %2, align 4
  %23 = icmp slt i32 %22, 10
  br i1 %23, label %18, label %24, !llvm.loop !9

24:                                               ; preds = %21
  %25 = load i32, ptr %1, align 4
  ret i32 %25
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
