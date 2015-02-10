; ModuleID = 'klee_div_zero_check.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-f128:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [22 x i8] c"klee_div_zero_check.c\00", align 1
@.str1 = private unnamed_addr constant [17 x i8] c"integer overflow\00", align 1
@.str2 = private unnamed_addr constant [8 x i8] c"div.err\00", align 1
@.str3 = private unnamed_addr constant [15 x i8] c"divide by zero\00", align 1

define void @klee_div_zero_check(i64 %z) nounwind {
entry:
  tail call void @llvm.dbg.value(metadata !{i64 %z}, i64 0, metadata !8), !dbg !9
  %0 = icmp eq i64 %z, 0, !dbg !10
  br i1 %0, label %bb, label %return, !dbg !10

bb:                                               ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([22 x i8]* @.str, i64 0, i64 0), i32 14, i8* getelementptr inbounds ([15 x i8]* @.str3, i64 0, i64 0), i8* getelementptr inbounds ([8 x i8]* @.str2, i64 0, i64 0)) noreturn nounwind, !dbg !12
  unreachable, !dbg !12

return:                                           ; preds = %entry
  ret void, !dbg !13
}

define void @klee_int_overflow_check(i64 %z) nounwind {
entry:
  tail call void @llvm.dbg.value(metadata !{i64 %z}, i64 0, metadata !7), !dbg !14
  %0 = icmp sgt i64 %z, 16, !dbg !15
  br i1 %0, label %bb, label %return, !dbg !15

bb:                                               ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([22 x i8]* @.str, i64 0, i64 0), i32 30, i8* getelementptr inbounds ([17 x i8]* @.str1, i64 0, i64 0), i8* getelementptr inbounds ([8 x i8]* @.str2, i64 0, i64 0)) noreturn nounwind, !dbg !17
  unreachable, !dbg !17

return:                                           ; preds = %entry
  ret void, !dbg !18
}

declare void @klee_report_error(i8*, i32, i8*, i8*) noreturn

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0, !6}
!llvm.dbg.lv.klee_int_overflow_check = !{!7}
!llvm.dbg.lv.klee_div_zero_check = !{!8}

!0 = metadata !{i32 589870, i32 0, metadata !1, metadata !"klee_int_overflow_check", metadata !"klee_int_overflow_check", metadata !"klee_int_overflow_check", metadata !1, i32 28, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i64)* @klee_int_overflow_check} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 589865, metadata !"klee_div_zero_check.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/runtime/Intrinsic/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 589841, i32 0, i32 1, metadata !"klee_div_zero_check.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{null, metadata !5}
!5 = metadata !{i32 589860, metadata !1, metadata !"long long int", metadata !1, i32 0, i64 64, i64 64, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!6 = metadata !{i32 589870, i32 0, metadata !1, metadata !"klee_div_zero_check", metadata !"klee_div_zero_check", metadata !"klee_div_zero_check", metadata !1, i32 12, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i64)* @klee_div_zero_check} ; [ DW_TAG_subprogram ]
!7 = metadata !{i32 590081, metadata !0, metadata !"z", metadata !1, i32 28, metadata !5, i32 0} ; [ DW_TAG_arg_variable ]
!8 = metadata !{i32 590081, metadata !6, metadata !"z", metadata !1, i32 12, metadata !5, i32 0} ; [ DW_TAG_arg_variable ]
!9 = metadata !{i32 12, i32 0, metadata !6, null}
!10 = metadata !{i32 13, i32 0, metadata !11, null}
!11 = metadata !{i32 589835, metadata !6, i32 12, i32 0, metadata !1, i32 1} ; [ DW_TAG_lexical_block ]
!12 = metadata !{i32 14, i32 0, metadata !11, null}
!13 = metadata !{i32 15, i32 0, metadata !11, null}
!14 = metadata !{i32 28, i32 0, metadata !0, null}
!15 = metadata !{i32 29, i32 0, metadata !16, null}
!16 = metadata !{i32 589835, metadata !0, i32 28, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!17 = metadata !{i32 30, i32 0, metadata !16, null}
!18 = metadata !{i32 31, i32 0, metadata !16, null}
