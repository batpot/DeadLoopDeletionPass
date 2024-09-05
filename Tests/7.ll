; ModuleID = '7.c'
source_filename = "7.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [18 x i8] c"nece obrisati ovu\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.2 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"ll\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 0, ptr %2, align 4
  br label %7

7:                                                ; preds = %17, %0
  %8 = load i32, ptr %2, align 4
  %9 = icmp slt i32 %8, 10
  br i1 %9, label %10, label %20

10:                                               ; preds = %7
  %11 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %12

12:                                               ; preds = %15, %10
  %13 = load i32, ptr %2, align 4
  %14 = icmp slt i32 %13, 5
  br i1 %14, label %15, label %16

15:                                               ; preds = %12
  br label %12, !llvm.loop !6

16:                                               ; preds = %12
  br label %17

17:                                               ; preds = %16
  %18 = load i32, ptr %2, align 4
  %19 = add nsw i32 %18, 1
  store i32 %19, ptr %2, align 4
  br label %7, !llvm.loop !8

20:                                               ; preds = %7
  store i32 0, ptr %3, align 4
  br label %21

21:                                               ; preds = %35, %20
  %22 = load i32, ptr %3, align 4
  %23 = icmp slt i32 %22, 10
  br i1 %23, label %24, label %38

24:                                               ; preds = %21
  %25 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %26

26:                                               ; preds = %29, %24
  %27 = load i32, ptr %3, align 4
  %28 = icmp slt i32 %27, 5
  br i1 %28, label %29, label %34

29:                                               ; preds = %26
  %30 = load i32, ptr %3, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %4, align 4
  %32 = load i32, ptr %4, align 4
  %33 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %32)
  br label %26, !llvm.loop !9

34:                                               ; preds = %26
  br label %35

35:                                               ; preds = %34
  %36 = load i32, ptr %3, align 4
  %37 = add nsw i32 %36, 1
  store i32 %37, ptr %3, align 4
  br label %21, !llvm.loop !10

38:                                               ; preds = %21
  store i32 0, ptr %5, align 4
  br label %39

39:                                               ; preds = %55, %38
  %40 = load i32, ptr %5, align 4
  %41 = icmp slt i32 %40, 0
  br i1 %41, label %42, label %58

42:                                               ; preds = %39
  %43 = call i32 (ptr, ...) @printf(ptr noundef @.str.2)
  %44 = load i32, ptr %5, align 4
  %45 = add nsw i32 %44, 1
  store i32 %45, ptr %6, align 4
  br label %46

46:                                               ; preds = %51, %42
  %47 = load i32, ptr %6, align 4
  %48 = icmp slt i32 %47, 10
  br i1 %48, label %49, label %54

49:                                               ; preds = %46
  %50 = call i32 (ptr, ...) @printf(ptr noundef @.str.3)
  br label %51

51:                                               ; preds = %49
  %52 = load i32, ptr %6, align 4
  %53 = add nsw i32 %52, 1
  store i32 %53, ptr %6, align 4
  br label %46, !llvm.loop !11

54:                                               ; preds = %46
  br label %55

55:                                               ; preds = %54
  %56 = load i32, ptr %5, align 4
  %57 = add nsw i32 %56, 1
  store i32 %57, ptr %5, align 4
  br label %39, !llvm.loop !12

58:                                               ; preds = %39
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

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
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
