; ModuleID = 'integer_overflow1.o'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-f128:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"a1\00", align 1

define i32 @loop(i32 %a) nounwind {
entry:
  %a_addr = alloca i32, align 4
  %retval = alloca i32
  %0 = alloca i32
  %num1 = alloca i32
  %num2 = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  store i32 %a, i32* %a_addr
  %1 = load i32* %a_addr, align 4, !dbg !65
  store i32 %1, i32* %num1, align 4, !dbg !65
  %2 = load i32* %num1, align 4, !dbg !67
  store i32 %2, i32* %0, align 4, !dbg !67
  %3 = load i32* %0, align 4, !dbg !67
  store i32 %3, i32* %retval, align 4, !dbg !67
  %retval1 = load i32* %retval, !dbg !67
  ret i32 %retval1, !dbg !67
}

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

define i32 @main() nounwind {
entry:
  %retval = alloca i32
  %0 = alloca i32
  %a1 = alloca i32
  %result = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  %a11 = bitcast i32* %a1 to i8*, !dbg !68
  call void @klee_make_symbolic(i8* %a11, i64 4, i8* getelementptr inbounds ([3 x i8]* @.str, i64 0, i64 0)) nounwind, !dbg !68
  %1 = load i32* %a1, align 4, !dbg !70
  %2 = call i32 @loop(i32 %1) nounwind, !dbg !70
  store i32 %2, i32* %result, align 4, !dbg !70
  store i32 0, i32* %0, align 4, !dbg !71
  %3 = load i32* %0, align 4, !dbg !71
  store i32 %3, i32* %retval, align 4, !dbg !71
  %retval2 = load i32* %retval, !dbg !71
  ret i32 %retval2, !dbg !71
}

declare void @klee_make_symbolic(i8*, i64, i8*)

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0, !6, !9, !18, !27}
!llvm.dbg.lv.memcpy = !{!37, !38, !39, !40, !44}
!llvm.dbg.lv.memmove = !{!47, !48, !49, !50, !54}
!llvm.dbg.lv.memset = !{!57, !58, !59, !60}

!0 = metadata !{i32 589870, i32 0, metadata !1, metadata !"loop", metadata !"loop", metadata !"loop", metadata !1, i32 2, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 (i32)* @loop} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 589865, metadata !"integer_overflow1.c", metadata !"/home/qirong/Desktop/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 589841, i32 0, i32 1, metadata !"integer_overflow1.c", metadata !"/home/qirong/Desktop/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 false, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !5}
!5 = metadata !{i32 589860, metadata !1, metadata !"int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!6 = metadata !{i32 589870, i32 0, metadata !1, metadata !"main", metadata !"main", metadata !"main", metadata !1, i32 13, metadata !7, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 ()* @main} ; [ DW_TAG_subprogram ]
!7 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !8, i32 0, null} ; [ DW_TAG_subroutine_type ]
!8 = metadata !{metadata !5}
!9 = metadata !{i32 589870, i32 0, metadata !10, metadata !"memcpy", metadata !"memcpy", metadata !"memcpy", metadata !10, i32 12, metadata !12, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!10 = metadata !{i32 589865, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !11} ; [ DW_TAG_file_type ]
!11 = metadata !{i32 589841, i32 0, i32 1, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!12 = metadata !{i32 589845, metadata !10, metadata !"", metadata !10, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !13, i32 0, null} ; [ DW_TAG_subroutine_type ]
!13 = metadata !{metadata !14, metadata !14, metadata !14, metadata !15}
!14 = metadata !{i32 589839, metadata !10, metadata !"", metadata !10, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!15 = metadata !{i32 589846, metadata !16, metadata !"size_t", metadata !16, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !17} ; [ DW_TAG_typedef ]
!16 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !11} ; [ DW_TAG_file_type ]
!17 = metadata !{i32 589860, metadata !10, metadata !"long unsigned int", metadata !10, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!18 = metadata !{i32 589870, i32 0, metadata !19, metadata !"memmove", metadata !"memmove", metadata !"memmove", metadata !19, i32 12, metadata !21, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!19 = metadata !{i32 589865, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !20} ; [ DW_TAG_file_type ]
!20 = metadata !{i32 589841, i32 0, i32 1, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!21 = metadata !{i32 589845, metadata !19, metadata !"", metadata !19, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !22, i32 0, null} ; [ DW_TAG_subroutine_type ]
!22 = metadata !{metadata !23, metadata !23, metadata !23, metadata !24}
!23 = metadata !{i32 589839, metadata !19, metadata !"", metadata !19, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!24 = metadata !{i32 589846, metadata !25, metadata !"size_t", metadata !25, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !26} ; [ DW_TAG_typedef ]
!25 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !20} ; [ DW_TAG_file_type ]
!26 = metadata !{i32 589860, metadata !19, metadata !"long unsigned int", metadata !19, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!27 = metadata !{i32 589870, i32 0, metadata !28, metadata !"memset", metadata !"memset", metadata !"memset", metadata !28, i32 12, metadata !30, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!28 = metadata !{i32 589865, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !29} ; [ DW_TAG_file_type ]
!29 = metadata !{i32 589841, i32 0, i32 1, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!30 = metadata !{i32 589845, metadata !28, metadata !"", metadata !28, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !31, i32 0, null} ; [ DW_TAG_subroutine_type ]
!31 = metadata !{metadata !32, metadata !32, metadata !33, metadata !34}
!32 = metadata !{i32 589839, metadata !28, metadata !"", metadata !28, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!33 = metadata !{i32 589860, metadata !28, metadata !"int", metadata !28, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!34 = metadata !{i32 589846, metadata !35, metadata !"size_t", metadata !35, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !36} ; [ DW_TAG_typedef ]
!35 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !29} ; [ DW_TAG_file_type ]
!36 = metadata !{i32 589860, metadata !28, metadata !"long unsigned int", metadata !28, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!37 = metadata !{i32 590081, metadata !9, metadata !"destaddr", metadata !10, i32 12, metadata !14, i32 0} ; [ DW_TAG_arg_variable ]
!38 = metadata !{i32 590081, metadata !9, metadata !"srcaddr", metadata !10, i32 12, metadata !14, i32 0} ; [ DW_TAG_arg_variable ]
!39 = metadata !{i32 590081, metadata !9, metadata !"len", metadata !10, i32 12, metadata !15, i32 0} ; [ DW_TAG_arg_variable ]
!40 = metadata !{i32 590080, metadata !41, metadata !"dest", metadata !10, i32 13, metadata !42, i32 0} ; [ DW_TAG_auto_variable ]
!41 = metadata !{i32 589835, metadata !9, i32 12, i32 0, metadata !10, i32 0} ; [ DW_TAG_lexical_block ]
!42 = metadata !{i32 589839, metadata !10, metadata !"", metadata !10, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !43} ; [ DW_TAG_pointer_type ]
!43 = metadata !{i32 589860, metadata !10, metadata !"char", metadata !10, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!44 = metadata !{i32 590080, metadata !41, metadata !"src", metadata !10, i32 14, metadata !45, i32 0} ; [ DW_TAG_auto_variable ]
!45 = metadata !{i32 589839, metadata !10, metadata !"", metadata !10, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !46} ; [ DW_TAG_pointer_type ]
!46 = metadata !{i32 589862, metadata !10, metadata !"", metadata !10, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !43} ; [ DW_TAG_const_type ]
!47 = metadata !{i32 590081, metadata !18, metadata !"dst", metadata !19, i32 12, metadata !23, i32 0} ; [ DW_TAG_arg_variable ]
!48 = metadata !{i32 590081, metadata !18, metadata !"src", metadata !19, i32 12, metadata !23, i32 0} ; [ DW_TAG_arg_variable ]
!49 = metadata !{i32 590081, metadata !18, metadata !"count", metadata !19, i32 12, metadata !24, i32 0} ; [ DW_TAG_arg_variable ]
!50 = metadata !{i32 590080, metadata !51, metadata !"a", metadata !19, i32 13, metadata !52, i32 0} ; [ DW_TAG_auto_variable ]
!51 = metadata !{i32 589835, metadata !18, i32 12, i32 0, metadata !19, i32 0} ; [ DW_TAG_lexical_block ]
!52 = metadata !{i32 589839, metadata !19, metadata !"", metadata !19, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !53} ; [ DW_TAG_pointer_type ]
!53 = metadata !{i32 589860, metadata !19, metadata !"char", metadata !19, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!54 = metadata !{i32 590080, metadata !51, metadata !"b", metadata !19, i32 14, metadata !55, i32 0} ; [ DW_TAG_auto_variable ]
!55 = metadata !{i32 589839, metadata !19, metadata !"", metadata !19, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !56} ; [ DW_TAG_pointer_type ]
!56 = metadata !{i32 589862, metadata !19, metadata !"", metadata !19, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !53} ; [ DW_TAG_const_type ]
!57 = metadata !{i32 590081, metadata !27, metadata !"dst", metadata !28, i32 12, metadata !32, i32 0} ; [ DW_TAG_arg_variable ]
!58 = metadata !{i32 590081, metadata !27, metadata !"s", metadata !28, i32 12, metadata !33, i32 0} ; [ DW_TAG_arg_variable ]
!59 = metadata !{i32 590081, metadata !27, metadata !"count", metadata !28, i32 12, metadata !34, i32 0} ; [ DW_TAG_arg_variable ]
!60 = metadata !{i32 590080, metadata !61, metadata !"a", metadata !28, i32 13, metadata !62, i32 0} ; [ DW_TAG_auto_variable ]
!61 = metadata !{i32 589835, metadata !27, i32 12, i32 0, metadata !28, i32 0} ; [ DW_TAG_lexical_block ]
!62 = metadata !{i32 589839, metadata !28, metadata !"", metadata !28, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !63} ; [ DW_TAG_pointer_type ]
!63 = metadata !{i32 589877, metadata !28, metadata !"", metadata !28, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !64} ; [ DW_TAG_volatile_type ]
!64 = metadata !{i32 589860, metadata !28, metadata !"char", metadata !28, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!65 = metadata !{i32 5, i32 0, metadata !66, null}
!66 = metadata !{i32 589835, metadata !0, i32 2, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!67 = metadata !{i32 7, i32 0, metadata !66, null}
!68 = metadata !{i32 15, i32 0, metadata !69, null}
!69 = metadata !{i32 589835, metadata !6, i32 13, i32 0, metadata !1, i32 1} ; [ DW_TAG_lexical_block ]
!70 = metadata !{i32 17, i32 0, metadata !69, null}
!71 = metadata !{i32 18, i32 0, metadata !69, null}
