; ModuleID = 'integer_overflow.o'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-f128:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"a1\00", align 1
@.str1 = private constant [22 x i8] c"klee_div_zero_check.c\00", align 1
@.str12 = private constant [17 x i8] c"integer overflow\00", align 1
@.str2 = private constant [8 x i8] c"div.err\00", align 1
@.str3 = private constant [15 x i8] c"divide by zero\00", align 1

define i32 @loop(i32 %a) nounwind {
entry:
  %a_addr = alloca i32, align 4
  %retval = alloca i32
  %0 = alloca i32
  %num1 = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  store i32 %a, i32* %a_addr
  %1 = load i32* %a_addr, align 4, !dbg !74
  %2 = load i32* %a_addr, align 4, !dbg !74
  %int_cast_to_i64 = zext i32 %1 to i64
  call void @klee_int_overflow_check(i64 %int_cast_to_i64)
  %3 = add nsw i32 %1, %2, !dbg !74
  store i32 %3, i32* %num1, align 4, !dbg !74
  %4 = load i32* %num1, align 4, !dbg !76
  store i32 %4, i32* %0, align 4, !dbg !76
  %5 = load i32* %0, align 4, !dbg !76
  store i32 %5, i32* %retval, align 4, !dbg !76
  %retval1 = load i32* %retval, !dbg !76
  ret i32 %retval1, !dbg !76
}

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

define i32 @main() nounwind {
entry:
  %retval = alloca i32
  %0 = alloca i32
  %a1 = alloca i32
  %result = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  %a11 = bitcast i32* %a1 to i8*, !dbg !77
  call void @klee_make_symbolic(i8* %a11, i64 4, i8* getelementptr inbounds ([3 x i8]* @.str, i64 0, i64 0)) nounwind, !dbg !77
  %1 = load i32* %a1, align 4, !dbg !79
  %2 = call i32 @loop(i32 %1) nounwind, !dbg !79
  store i32 %2, i32* %result, align 4, !dbg !79
  store i32 0, i32* %0, align 4, !dbg !80
  %3 = load i32* %0, align 4, !dbg !80
  store i32 %3, i32* %retval, align 4, !dbg !80
  %retval2 = load i32* %retval, !dbg !80
  ret i32 %retval2, !dbg !80
}

declare void @klee_make_symbolic(i8*, i64, i8*)

define void @klee_div_zero_check(i64 %z) nounwind {
entry:
  %0 = icmp eq i64 %z, 0, !dbg !81
  br i1 %0, label %bb, label %return, !dbg !81

bb:                                               ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([22 x i8]* @.str1, i64 0, i64 0), i32 14, i8* getelementptr inbounds ([15 x i8]* @.str3, i64 0, i64 0), i8* getelementptr inbounds ([8 x i8]* @.str2, i64 0, i64 0)) noreturn nounwind, !dbg !
  unreachable, !dbg !83

return:                                           ; preds = %entry
  ret void, !dbg !84
}

define void @klee_int_overflow_check(i64 %z) nounwind {
entry:
  %0 = icmp sgt i64 %z, 16, !dbg !85
  br i1 %0, label %bb, label %return, !dbg !85

bb:                                               ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([22 x i8]* @.str1, i64 0, i64 0), i32 26, i8* getelementptr inbounds ([17 x i8]* @.str12, i64 0, i64 0), i8* getelementptr inbounds ([8 x i8]* @.str2, i64 0, i64 0)) noreturn nounwind, !dbg 
  unreachable, !dbg !87

return:                                           ; preds = %entry
  ret void, !dbg !88
}

declare void @klee_report_error(i8*, i32, i8*, i8*) noreturn

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0, !6, !9, !15, !16, !25, !34}
!llvm.dbg.lv.klee_int_overflow_check = !{!44}
!llvm.dbg.lv.klee_div_zero_check = !{!45}
!llvm.dbg.lv.memcpy = !{!46, !47, !48, !49, !53}
!llvm.dbg.lv.memmove = !{!56, !57, !58, !59, !63}
!llvm.dbg.lv.memset = !{!66, !67, !68, !69}

!0 = metadata !{i32 589870, i32 0, metadata !1, metadata !"loop", metadata !"loop", metadata !"loop", metadata !1, i32 2, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 (i32)* @loop} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 589865, metadata !"integer_overflow.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/Release+Asserts/bin/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 589841, i32 0, i32 1, metadata !"integer_overflow.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/Release+Asserts/bin/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 false, metadata !"", i32 0} ; [ 
!3 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !5}
!5 = metadata !{i32 589860, metadata !1, metadata !"int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!6 = metadata !{i32 589870, i32 0, metadata !1, metadata !"main", metadata !"main", metadata !"main", metadata !1, i32 13, metadata !7, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 ()* @main} ; [ DW_TAG_subprogram ]
!7 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !8, i32 0, null} ; [ DW_TAG_subroutine_type ]
!8 = metadata !{metadata !5}
!9 = metadata !{i32 589870, i32 0, metadata !10, metadata !"klee_int_overflow_check", metadata !"klee_int_overflow_check", metadata !"klee_int_overflow_check", metadata !10, i32 24, metadata !12, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, v
!10 = metadata !{i32 589865, metadata !"klee_div_zero_check.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/runtime/Intrinsic/", metadata !11} ; [ DW_TAG_file_type ]
!11 = metadata !{i32 589841, i32 0, i32 1, metadata !"klee_div_zero_check.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [
!12 = metadata !{i32 589845, metadata !10, metadata !"", metadata !10, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !13, i32 0, null} ; [ DW_TAG_subroutine_type ]
!13 = metadata !{null, metadata !14}
!14 = metadata !{i32 589860, metadata !10, metadata !"long long int", metadata !10, i32 0, i64 64, i64 64, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!15 = metadata !{i32 589870, i32 0, metadata !10, metadata !"klee_div_zero_check", metadata !"klee_div_zero_check", metadata !"klee_div_zero_check", metadata !10, i32 12, metadata !12, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i64)* 
!16 = metadata !{i32 589870, i32 0, metadata !17, metadata !"memcpy", metadata !"memcpy", metadata !"memcpy", metadata !17, i32 12, metadata !19, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!17 = metadata !{i32 589865, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !18} ; [ DW_TAG_file_type ]
!18 = metadata !{i32 589841, i32 0, i32 1, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!19 = metadata !{i32 589845, metadata !17, metadata !"", metadata !17, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !20, i32 0, null} ; [ DW_TAG_subroutine_type ]
!20 = metadata !{metadata !21, metadata !21, metadata !21, metadata !22}
!21 = metadata !{i32 589839, metadata !17, metadata !"", metadata !17, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!22 = metadata !{i32 589846, metadata !23, metadata !"size_t", metadata !23, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !24} ; [ DW_TAG_typedef ]
!23 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !18} ; [ DW_TAG_file_type ]
!24 = metadata !{i32 589860, metadata !17, metadata !"long unsigned int", metadata !17, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!25 = metadata !{i32 589870, i32 0, metadata !26, metadata !"memmove", metadata !"memmove", metadata !"memmove", metadata !26, i32 12, metadata !28, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!26 = metadata !{i32 589865, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !27} ; [ DW_TAG_file_type ]
!27 = metadata !{i32 589841, i32 0, i32 1, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!28 = metadata !{i32 589845, metadata !26, metadata !"", metadata !26, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !29, i32 0, null} ; [ DW_TAG_subroutine_type ]
!29 = metadata !{metadata !30, metadata !30, metadata !30, metadata !31}
!30 = metadata !{i32 589839, metadata !26, metadata !"", metadata !26, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!31 = metadata !{i32 589846, metadata !32, metadata !"size_t", metadata !32, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !33} ; [ DW_TAG_typedef ]
!32 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !27} ; [ DW_TAG_file_type ]
!33 = metadata !{i32 589860, metadata !26, metadata !"long unsigned int", metadata !26, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!34 = metadata !{i32 589870, i32 0, metadata !35, metadata !"memset", metadata !"memset", metadata !"memset", metadata !35, i32 12, metadata !37, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!35 = metadata !{i32 589865, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !36} ; [ DW_TAG_file_type ]
!36 = metadata !{i32 589841, i32 0, i32 1, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!37 = metadata !{i32 589845, metadata !35, metadata !"", metadata !35, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !38, i32 0, null} ; [ DW_TAG_subroutine_type ]
!38 = metadata !{metadata !39, metadata !39, metadata !40, metadata !41}
!39 = metadata !{i32 589839, metadata !35, metadata !"", metadata !35, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!40 = metadata !{i32 589860, metadata !35, metadata !"int", metadata !35, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!41 = metadata !{i32 589846, metadata !42, metadata !"size_t", metadata !42, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !43} ; [ DW_TAG_typedef ]
!42 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !36} ; [ DW_TAG_file_type ]
!43 = metadata !{i32 589860, metadata !35, metadata !"long unsigned int", metadata !35, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!44 = metadata !{i32 590081, metadata !9, metadata !"z", metadata !10, i32 24, metadata !14, i32 0} ; [ DW_TAG_arg_variable ]
!45 = metadata !{i32 590081, metadata !15, metadata !"z", metadata !10, i32 12, metadata !14, i32 0} ; [ DW_TAG_arg_variable ]
!46 = metadata !{i32 590081, metadata !16, metadata !"destaddr", metadata !17, i32 12, metadata !21, i32 0} ; [ DW_TAG_arg_variable ]
!47 = metadata !{i32 590081, metadata !16, metadata !"srcaddr", metadata !17, i32 12, metadata !21, i32 0} ; [ DW_TAG_arg_variable ]
!48 = metadata !{i32 590081, metadata !16, metadata !"len", metadata !17, i32 12, metadata !22, i32 0} ; [ DW_TAG_arg_variable ]
!49 = metadata !{i32 590080, metadata !50, metadata !"dest", metadata !17, i32 13, metadata !51, i32 0} ; [ DW_TAG_auto_variable ]
!50 = metadata !{i32 589835, metadata !16, i32 12, i32 0, metadata !17, i32 0} ; [ DW_TAG_lexical_block ]
!51 = metadata !{i32 589839, metadata !17, metadata !"", metadata !17, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !52} ; [ DW_TAG_pointer_type ]
!52 = metadata !{i32 589860, metadata !17, metadata !"char", metadata !17, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!53 = metadata !{i32 590080, metadata !50, metadata !"src", metadata !17, i32 14, metadata !54, i32 0} ; [ DW_TAG_auto_variable ]
!54 = metadata !{i32 589839, metadata !17, metadata !"", metadata !17, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !55} ; [ DW_TAG_pointer_type ]
!55 = metadata !{i32 589862, metadata !17, metadata !"", metadata !17, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !52} ; [ DW_TAG_const_type ]
!56 = metadata !{i32 590081, metadata !25, metadata !"dst", metadata !26, i32 12, metadata !30, i32 0} ; [ DW_TAG_arg_variable ]
!57 = metadata !{i32 590081, metadata !25, metadata !"src", metadata !26, i32 12, metadata !30, i32 0} ; [ DW_TAG_arg_variable ]
!58 = metadata !{i32 590081, metadata !25, metadata !"count", metadata !26, i32 12, metadata !31, i32 0} ; [ DW_TAG_arg_variable ]
!59 = metadata !{i32 590080, metadata !60, metadata !"a", metadata !26, i32 13, metadata !61, i32 0} ; [ DW_TAG_auto_variable ]
!60 = metadata !{i32 589835, metadata !25, i32 12, i32 0, metadata !26, i32 0} ; [ DW_TAG_lexical_block ]
!61 = metadata !{i32 589839, metadata !26, metadata !"", metadata !26, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !62} ; [ DW_TAG_pointer_type ]
!62 = metadata !{i32 589860, metadata !26, metadata !"char", metadata !26, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!63 = metadata !{i32 590080, metadata !60, metadata !"b", metadata !26, i32 14, metadata !64, i32 0} ; [ DW_TAG_auto_variable ]
!64 = metadata !{i32 589839, metadata !26, metadata !"", metadata !26, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !65} ; [ DW_TAG_pointer_type ]
!65 = metadata !{i32 589862, metadata !26, metadata !"", metadata !26, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !62} ; [ DW_TAG_const_type ]
!66 = metadata !{i32 590081, metadata !34, metadata !"dst", metadata !35, i32 12, metadata !39, i32 0} ; [ DW_TAG_arg_variable ]
!67 = metadata !{i32 590081, metadata !34, metadata !"s", metadata !35, i32 12, metadata !40, i32 0} ; [ DW_TAG_arg_variable ]
!68 = metadata !{i32 590081, metadata !34, metadata !"count", metadata !35, i32 12, metadata !41, i32 0} ; [ DW_TAG_arg_variable ]
!69 = metadata !{i32 590080, metadata !70, metadata !"a", metadata !35, i32 13, metadata !71, i32 0} ; [ DW_TAG_auto_variable ]
!70 = metadata !{i32 589835, metadata !34, i32 12, i32 0, metadata !35, i32 0} ; [ DW_TAG_lexical_block ]
!71 = metadata !{i32 589839, metadata !35, metadata !"", metadata !35, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !72} ; [ DW_TAG_pointer_type ]
!72 = metadata !{i32 589877, metadata !35, metadata !"", metadata !35, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !73} ; [ DW_TAG_volatile_type ]
!73 = metadata !{i32 589860, metadata !35, metadata !"char", metadata !35, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!74 = metadata !{i32 5, i32 0, metadata !75, null}
!75 = metadata !{i32 589835, metadata !0, i32 2, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!76 = metadata !{i32 7, i32 0, metadata !75, null}
!77 = metadata !{i32 15, i32 0, metadata !78, null}
!78 = metadata !{i32 589835, metadata !6, i32 13, i32 0, metadata !1, i32 1} ; [ DW_TAG_lexical_block ]
!79 = metadata !{i32 17, i32 0, metadata !78, null}
!80 = metadata !{i32 18, i32 0, metadata !78, null}
!81 = metadata !{i32 13, i32 0, metadata !82, null}
!82 = metadata !{i32 589835, metadata !15, i32 12, i32 0, metadata !10, i32 1} ; [ DW_TAG_lexical_block ]
!83 = metadata !{i32 14, i32 0, metadata !82, null}
!84 = metadata !{i32 15, i32 0, metadata !82, null}
!85 = metadata !{i32 25, i32 0, metadata !86, null}
!86 = metadata !{i32 589835, metadata !9, i32 24, i32 0, metadata !10, i32 0} ; [ DW_TAG_lexical_block ]
!87 = metadata !{i32 26, i32 0, metadata !86, null}
!88 = metadata !{i32 27, i32 0, metadata !86, null}
