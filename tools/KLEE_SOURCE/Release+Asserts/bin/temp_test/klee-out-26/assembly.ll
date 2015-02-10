; ModuleID = 'divide_zero-1.o'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-f128:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"a0\00", align 1
@.str1 = private constant [22 x i8] c"klee_div_zero_check.c\00", align 1
@.str12 = private constant [15 x i8] c"divide by zero\00", align 1
@.str2 = private constant [8 x i8] c"div.err\00", align 1

define i32 @divide_zero(i32 %den) nounwind {
entry:
  %den_addr = alloca i32, align 4
  %retval = alloca i32
  %0 = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  store i32 %den, i32* %den_addr
  %1 = load i32* %den_addr, align 4, !dbg !72
  %int_cast_to_i64 = zext i32 %1 to i64
  call void @klee_div_zero_check(i64 %int_cast_to_i64)
  %2 = sdiv i32 10, %1, !dbg !72
  store i32 %2, i32* %0, align 4, !dbg !72
  %3 = load i32* %0, align 4, !dbg !72
  store i32 %3, i32* %retval, align 4, !dbg !72
  %retval1 = load i32* %retval, !dbg !72
  ret i32 %retval1, !dbg !72
}

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

define i32 @main() nounwind {
entry:
  %retval = alloca i32
  %0 = alloca i32
  %a0 = alloca i32
  %result = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  %a01 = bitcast i32* %a0 to i8*, !dbg !74
  call void @klee_make_symbolic(i8* %a01, i64 4, i8* getelementptr inbounds ([3 x i8]* @.str, i64 0, i64 0)) nounwind, !dbg !74
  %1 = load i32* %a0, align 4, !dbg !76
  %2 = call i32 @divide_zero(i32 %1) nounwind, !dbg !76
  store i32 %2, i32* %result, align 4, !dbg !76
  store i32 0, i32* %0, align 4, !dbg !77
  %3 = load i32* %0, align 4, !dbg !77
  store i32 %3, i32* %retval, align 4, !dbg !77
  %retval2 = load i32* %retval, !dbg !77
  ret i32 %retval2, !dbg !77
}

declare void @klee_make_symbolic(i8*, i64, i8*)

define void @klee_div_zero_check(i64 %z) nounwind {
entry:
  %0 = icmp eq i64 %z, 0, !dbg !78
  br i1 %0, label %bb, label %return, !dbg !78

bb:                                               ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([22 x i8]* @.str1, i64 0, i64 0), i32 14, i8* getelementptr inbounds ([15 x i8]* @.str12, i64 0, i64 0), i8* getelementptr inbounds ([8 x i8]* @.str2, i64 0, i64 0)) noreturn nounwind, !dbg 
  unreachable, !dbg !80

return:                                           ; preds = %entry
  ret void, !dbg !81
}

declare void @klee_report_error(i8*, i32, i8*, i8*) noreturn

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0, !6, !9, !15, !24, !33}
!llvm.dbg.lv.klee_div_zero_check = !{!43}
!llvm.dbg.lv.memcpy = !{!44, !45, !46, !47, !51}
!llvm.dbg.lv.memmove = !{!54, !55, !56, !57, !61}
!llvm.dbg.lv.memset = !{!64, !65, !66, !67}

!0 = metadata !{i32 589870, i32 0, metadata !1, metadata !"divide_zero", metadata !"divide_zero", metadata !"divide_zero", metadata !1, i32 3, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 (i32)* @divide_zero} ; [ DW_TAG_subp
!1 = metadata !{i32 589865, metadata !"divide_zero-1.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/Release+Asserts/bin/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 589841, i32 0, i32 1, metadata !"divide_zero-1.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/Release+Asserts/bin/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 false, metadata !"", i32 0} ; [ DW_
!3 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !5}
!5 = metadata !{i32 589860, metadata !1, metadata !"int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!6 = metadata !{i32 589870, i32 0, metadata !1, metadata !"main", metadata !"main", metadata !"main", metadata !1, i32 11, metadata !7, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 ()* @main} ; [ DW_TAG_subprogram ]
!7 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !8, i32 0, null} ; [ DW_TAG_subroutine_type ]
!8 = metadata !{metadata !5}
!9 = metadata !{i32 589870, i32 0, metadata !10, metadata !"klee_div_zero_check", metadata !"klee_div_zero_check", metadata !"klee_div_zero_check", metadata !10, i32 12, metadata !12, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i64)* @
!10 = metadata !{i32 589865, metadata !"klee_div_zero_check.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !11} ; [ DW_TAG_file_type ]
!11 = metadata !{i32 589841, i32 0, i32 1, metadata !"klee_div_zero_check.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_u
!12 = metadata !{i32 589845, metadata !10, metadata !"", metadata !10, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !13, i32 0, null} ; [ DW_TAG_subroutine_type ]
!13 = metadata !{null, metadata !14}
!14 = metadata !{i32 589860, metadata !10, metadata !"long long int", metadata !10, i32 0, i64 64, i64 64, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!15 = metadata !{i32 589870, i32 0, metadata !16, metadata !"memcpy", metadata !"memcpy", metadata !"memcpy", metadata !16, i32 12, metadata !18, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!16 = metadata !{i32 589865, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !17} ; [ DW_TAG_file_type ]
!17 = metadata !{i32 589841, i32 0, i32 1, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!18 = metadata !{i32 589845, metadata !16, metadata !"", metadata !16, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !19, i32 0, null} ; [ DW_TAG_subroutine_type ]
!19 = metadata !{metadata !20, metadata !20, metadata !20, metadata !21}
!20 = metadata !{i32 589839, metadata !16, metadata !"", metadata !16, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!21 = metadata !{i32 589846, metadata !22, metadata !"size_t", metadata !22, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !23} ; [ DW_TAG_typedef ]
!22 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !17} ; [ DW_TAG_file_type ]
!23 = metadata !{i32 589860, metadata !16, metadata !"long unsigned int", metadata !16, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!24 = metadata !{i32 589870, i32 0, metadata !25, metadata !"memmove", metadata !"memmove", metadata !"memmove", metadata !25, i32 12, metadata !27, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!25 = metadata !{i32 589865, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !26} ; [ DW_TAG_file_type ]
!26 = metadata !{i32 589841, i32 0, i32 1, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!27 = metadata !{i32 589845, metadata !25, metadata !"", metadata !25, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !28, i32 0, null} ; [ DW_TAG_subroutine_type ]
!28 = metadata !{metadata !29, metadata !29, metadata !29, metadata !30}
!29 = metadata !{i32 589839, metadata !25, metadata !"", metadata !25, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!30 = metadata !{i32 589846, metadata !31, metadata !"size_t", metadata !31, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !32} ; [ DW_TAG_typedef ]
!31 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !26} ; [ DW_TAG_file_type ]
!32 = metadata !{i32 589860, metadata !25, metadata !"long unsigned int", metadata !25, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!33 = metadata !{i32 589870, i32 0, metadata !34, metadata !"memset", metadata !"memset", metadata !"memset", metadata !34, i32 12, metadata !36, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!34 = metadata !{i32 589865, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !35} ; [ DW_TAG_file_type ]
!35 = metadata !{i32 589841, i32 0, i32 1, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!36 = metadata !{i32 589845, metadata !34, metadata !"", metadata !34, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !37, i32 0, null} ; [ DW_TAG_subroutine_type ]
!37 = metadata !{metadata !38, metadata !38, metadata !39, metadata !40}
!38 = metadata !{i32 589839, metadata !34, metadata !"", metadata !34, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!39 = metadata !{i32 589860, metadata !34, metadata !"int", metadata !34, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!40 = metadata !{i32 589846, metadata !41, metadata !"size_t", metadata !41, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !42} ; [ DW_TAG_typedef ]
!41 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !35} ; [ DW_TAG_file_type ]
!42 = metadata !{i32 589860, metadata !34, metadata !"long unsigned int", metadata !34, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!43 = metadata !{i32 590081, metadata !9, metadata !"z", metadata !10, i32 12, metadata !14, i32 0} ; [ DW_TAG_arg_variable ]
!44 = metadata !{i32 590081, metadata !15, metadata !"destaddr", metadata !16, i32 12, metadata !20, i32 0} ; [ DW_TAG_arg_variable ]
!45 = metadata !{i32 590081, metadata !15, metadata !"srcaddr", metadata !16, i32 12, metadata !20, i32 0} ; [ DW_TAG_arg_variable ]
!46 = metadata !{i32 590081, metadata !15, metadata !"len", metadata !16, i32 12, metadata !21, i32 0} ; [ DW_TAG_arg_variable ]
!47 = metadata !{i32 590080, metadata !48, metadata !"dest", metadata !16, i32 13, metadata !49, i32 0} ; [ DW_TAG_auto_variable ]
!48 = metadata !{i32 589835, metadata !15, i32 12, i32 0, metadata !16, i32 0} ; [ DW_TAG_lexical_block ]
!49 = metadata !{i32 589839, metadata !16, metadata !"", metadata !16, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !50} ; [ DW_TAG_pointer_type ]
!50 = metadata !{i32 589860, metadata !16, metadata !"char", metadata !16, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!51 = metadata !{i32 590080, metadata !48, metadata !"src", metadata !16, i32 14, metadata !52, i32 0} ; [ DW_TAG_auto_variable ]
!52 = metadata !{i32 589839, metadata !16, metadata !"", metadata !16, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !53} ; [ DW_TAG_pointer_type ]
!53 = metadata !{i32 589862, metadata !16, metadata !"", metadata !16, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !50} ; [ DW_TAG_const_type ]
!54 = metadata !{i32 590081, metadata !24, metadata !"dst", metadata !25, i32 12, metadata !29, i32 0} ; [ DW_TAG_arg_variable ]
!55 = metadata !{i32 590081, metadata !24, metadata !"src", metadata !25, i32 12, metadata !29, i32 0} ; [ DW_TAG_arg_variable ]
!56 = metadata !{i32 590081, metadata !24, metadata !"count", metadata !25, i32 12, metadata !30, i32 0} ; [ DW_TAG_arg_variable ]
!57 = metadata !{i32 590080, metadata !58, metadata !"a", metadata !25, i32 13, metadata !59, i32 0} ; [ DW_TAG_auto_variable ]
!58 = metadata !{i32 589835, metadata !24, i32 12, i32 0, metadata !25, i32 0} ; [ DW_TAG_lexical_block ]
!59 = metadata !{i32 589839, metadata !25, metadata !"", metadata !25, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !60} ; [ DW_TAG_pointer_type ]
!60 = metadata !{i32 589860, metadata !25, metadata !"char", metadata !25, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!61 = metadata !{i32 590080, metadata !58, metadata !"b", metadata !25, i32 14, metadata !62, i32 0} ; [ DW_TAG_auto_variable ]
!62 = metadata !{i32 589839, metadata !25, metadata !"", metadata !25, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !63} ; [ DW_TAG_pointer_type ]
!63 = metadata !{i32 589862, metadata !25, metadata !"", metadata !25, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !60} ; [ DW_TAG_const_type ]
!64 = metadata !{i32 590081, metadata !33, metadata !"dst", metadata !34, i32 12, metadata !38, i32 0} ; [ DW_TAG_arg_variable ]
!65 = metadata !{i32 590081, metadata !33, metadata !"s", metadata !34, i32 12, metadata !39, i32 0} ; [ DW_TAG_arg_variable ]
!66 = metadata !{i32 590081, metadata !33, metadata !"count", metadata !34, i32 12, metadata !40, i32 0} ; [ DW_TAG_arg_variable ]
!67 = metadata !{i32 590080, metadata !68, metadata !"a", metadata !34, i32 13, metadata !69, i32 0} ; [ DW_TAG_auto_variable ]
!68 = metadata !{i32 589835, metadata !33, i32 12, i32 0, metadata !34, i32 0} ; [ DW_TAG_lexical_block ]
!69 = metadata !{i32 589839, metadata !34, metadata !"", metadata !34, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !70} ; [ DW_TAG_pointer_type ]
!70 = metadata !{i32 589877, metadata !34, metadata !"", metadata !34, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !71} ; [ DW_TAG_volatile_type ]
!71 = metadata !{i32 589860, metadata !34, metadata !"char", metadata !34, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!72 = metadata !{i32 5, i32 0, metadata !73, null}
!73 = metadata !{i32 589835, metadata !0, i32 3, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!74 = metadata !{i32 13, i32 0, metadata !75, null}
!75 = metadata !{i32 589835, metadata !6, i32 11, i32 0, metadata !1, i32 1} ; [ DW_TAG_lexical_block ]
!76 = metadata !{i32 16, i32 0, metadata !75, null}
!77 = metadata !{i32 18, i32 0, metadata !75, null}
!78 = metadata !{i32 13, i32 0, metadata !79, null}
!79 = metadata !{i32 589835, metadata !9, i32 12, i32 0, metadata !10, i32 0} ; [ DW_TAG_lexical_block ]
!80 = metadata !{i32 14, i32 0, metadata !79, null}
!81 = metadata !{i32 15, i32 0, metadata !79, null}
