; ModuleID = '8.c'
source_filename = "8.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 0, ptr %3, align 4
  br label %12

12:                                               ; preds = %16, %0
  %13 = load i32, ptr %3, align 4
  %14 = icmp slt i32 %13, 10
  br i1 %14, label %15, label %19

15:                                               ; preds = %12
  store i32 12, ptr %4, align 4
  store i32 11, ptr %2, align 4
  br label %16

16:                                               ; preds = %15
  %17 = load i32, ptr %3, align 4
  %18 = add nsw i32 %17, 1
  store i32 %18, ptr %3, align 4
  br label %12, !llvm.loop !6

19:                                               ; preds = %12
  store i32 0, ptr %5, align 4
  br label %20

20:                                               ; preds = %28, %19
  %21 = load i32, ptr %5, align 4
  %22 = icmp slt i32 %21, 5
  br i1 %22, label %23, label %31

23:                                               ; preds = %20
  store i32 12, ptr %6, align 4
  %24 = load i32, ptr %2, align 4
  %25 = add nsw i32 %24, 1
  store i32 %25, ptr %2, align 4
  %26 = load i32, ptr %5, align 4
  %27 = add nsw i32 %26, 1
  store i32 %27, ptr %7, align 4
  br label %28

28:                                               ; preds = %23
  %29 = load i32, ptr %5, align 4
  %30 = add nsw i32 %29, 1
  store i32 %30, ptr %5, align 4
  br label %20, !llvm.loop !8

31:                                               ; preds = %20
  store i32 10, ptr %8, align 4
  br label %32

32:                                               ; preds = %35, %31
  %33 = load i32, ptr %2, align 4
  %34 = icmp slt i32 %33, 15
  br i1 %34, label %35, label %40

35:                                               ; preds = %32
  %36 = load i32, ptr %8, align 4
  %37 = mul nsw i32 %36, 12
  store i32 %37, ptr %8, align 4
  %38 = load i32, ptr %2, align 4
  %39 = add nsw i32 %38, 1
  store i32 %39, ptr %2, align 4
  br label %32, !llvm.loop !9

40:                                               ; preds = %32
  store i32 0, ptr %9, align 4
  br label %41

41:                                               ; preds = %42, %40
  store i32 12, ptr %10, align 4
  store i32 1, ptr %11, align 4
  br label %42

42:                                               ; preds = %41
  %43 = load i32, ptr %9, align 4
  %44 = icmp slt i32 %43, 10
  br i1 %44, label %41, label %45, !llvm.loop !10

45:                                               ; preds = %42
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
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
