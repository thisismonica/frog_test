; ModuleID = 'pointer1.o'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-f128:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"a1\00", align 1

define i32 @loop(i32* %a) nounwind {
entry:
  %a_addr = alloca i32*, align 8
  %retval = alloca i32
  %0 = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  store i32* %a, i32** %a_addr
  %1 = load i32** %a_addr, align 8, !dbg !66
  %2 = getelementptr inbounds i32* %1, i64 1, !dbg !66
  store i32* %2, i32** %a_addr, align 8, !dbg !66
  %3 = load i32** %a_addr, align 8, !dbg !68
  store i32 1, i32* %3, align 4, !dbg !68
  store i32 1, i32* %0, align 4, !dbg !69
  %4 = load i32* %0, align 4, !dbg !69
  store i32 %4, i32* %retval, align 4, !dbg !69
  %retval1 = load i32* %retval, !dbg !69
  ret i32 %retval1, !dbg !69
}

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

define i32 @main() nounwind {
entry:
  %retval = alloca i32
  %0 = alloca i32
  %a1 = alloca [1 x i32]
  %result = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  %a11 = bitcast [1 x i32]* %a1 to i32*, !dbg !70
  %a112 = bitcast i32* %a11 to i8*, !dbg !70
  call void @klee_make_symbolic(i8* %a112, i64 4, i8* getelementptr inbounds ([3 x i8]* @.str, i64 0, i64 0)) nounwind, !dbg !70
  %a13 = bitcast [1 x i32]* %a1 to i32*, !dbg !72
  %1 = call i32 @loop(i32* %a13) nounwind, !dbg !72
  store i32 %1, i32* %result, align 4, !dbg !72
  store i32 0, i32* %0, align 4, !dbg !73
  %2 = load i32* %0, align 4, !dbg !73
  store i32 %2, i32* %retval, align 4, !dbg !73
  %retval4 = load i32* %retval, !dbg !73
  ret i32 %retval4, !dbg !73
}

declare void @klee_make_symbolic(i8*, i64, i8*)

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0, !7, !10, !19, !28}
!llvm.dbg.lv.memcpy = !{!38, !39, !40, !41, !45}
!llvm.dbg.lv.memmove = !{!48, !49, !50, !51, !55}
!llvm.dbg.lv.memset = !{!58, !59, !60, !61}

!0 = metadata !{i32 589870, i32 0, metadata !1, metadata !"loop", metadata !"loop", metadata !"loop", metadata !1, i32 2, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 (i32*)* @loop} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 589865, metadata !"pointer1.c", metadata !"/home/qirong/Desktop/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 589841, i32 0, i32 1, metadata !"pointer1.c", metadata !"/home/qirong/Desktop/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 false, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !6}
!5 = metadata !{i32 589860, metadata !1, metadata !"int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!6 = metadata !{i32 589839, metadata !1, metadata !"", metadata !1, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !5} ; [ DW_TAG_pointer_type ]
!7 = metadata !{i32 589870, i32 0, metadata !1, metadata !"main", metadata !"main", metadata !"main", metadata !1, i32 13, metadata !8, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 ()* @main} ; [ DW_TAG_subprogram ]
!8 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !9, i32 0, null} ; [ DW_TAG_subroutine_type ]
!9 = metadata !{metadata !5}
!10 = metadata !{i32 589870, i32 0, metadata !11, metadata !"memcpy", metadata !"memcpy", metadata !"memcpy", metadata !11, i32 12, metadata !13, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!11 = metadata !{i32 589865, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !12} ; [ DW_TAG_file_type ]
!12 = metadata !{i32 589841, i32 0, i32 1, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!13 = metadata !{i32 589845, metadata !11, metadata !"", metadata !11, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !14, i32 0, null} ; [ DW_TAG_subroutine_type ]
!14 = metadata !{metadata !15, metadata !15, metadata !15, metadata !16}
!15 = metadata !{i32 589839, metadata !11, metadata !"", metadata !11, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!16 = metadata !{i32 589846, metadata !17, metadata !"size_t", metadata !17, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !18} ; [ DW_TAG_typedef ]
!17 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !12} ; [ DW_TAG_file_type ]
!18 = metadata !{i32 589860, metadata !11, metadata !"long unsigned int", metadata !11, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!19 = metadata !{i32 589870, i32 0, metadata !20, metadata !"memmove", metadata !"memmove", metadata !"memmove", metadata !20, i32 12, metadata !22, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!20 = metadata !{i32 589865, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !21} ; [ DW_TAG_file_type ]
!21 = metadata !{i32 589841, i32 0, i32 1, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!22 = metadata !{i32 589845, metadata !20, metadata !"", metadata !20, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !23, i32 0, null} ; [ DW_TAG_subroutine_type ]
!23 = metadata !{metadata !24, metadata !24, metadata !24, metadata !25}
!24 = metadata !{i32 589839, metadata !20, metadata !"", metadata !20, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!25 = metadata !{i32 589846, metadata !26, metadata !"size_t", metadata !26, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !27} ; [ DW_TAG_typedef ]
!26 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !21} ; [ DW_TAG_file_type ]
!27 = metadata !{i32 589860, metadata !20, metadata !"long unsigned int", metadata !20, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!28 = metadata !{i32 589870, i32 0, metadata !29, metadata !"memset", metadata !"memset", metadata !"memset", metadata !29, i32 12, metadata !31, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!29 = metadata !{i32 589865, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !30} ; [ DW_TAG_file_type ]
!30 = metadata !{i32 589841, i32 0, i32 1, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!31 = metadata !{i32 589845, metadata !29, metadata !"", metadata !29, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !32, i32 0, null} ; [ DW_TAG_subroutine_type ]
!32 = metadata !{metadata !33, metadata !33, metadata !34, metadata !35}
!33 = metadata !{i32 589839, metadata !29, metadata !"", metadata !29, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!34 = metadata !{i32 589860, metadata !29, metadata !"int", metadata !29, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!35 = metadata !{i32 589846, metadata !36, metadata !"size_t", metadata !36, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !37} ; [ DW_TAG_typedef ]
!36 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !30} ; [ DW_TAG_file_type ]
!37 = metadata !{i32 589860, metadata !29, metadata !"long unsigned int", metadata !29, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!38 = metadata !{i32 590081, metadata !10, metadata !"destaddr", metadata !11, i32 12, metadata !15, i32 0} ; [ DW_TAG_arg_variable ]
!39 = metadata !{i32 590081, metadata !10, metadata !"srcaddr", metadata !11, i32 12, metadata !15, i32 0} ; [ DW_TAG_arg_variable ]
!40 = metadata !{i32 590081, metadata !10, metadata !"len", metadata !11, i32 12, metadata !16, i32 0} ; [ DW_TAG_arg_variable ]
!41 = metadata !{i32 590080, metadata !42, metadata !"dest", metadata !11, i32 13, metadata !43, i32 0} ; [ DW_TAG_auto_variable ]
!42 = metadata !{i32 589835, metadata !10, i32 12, i32 0, metadata !11, i32 0} ; [ DW_TAG_lexical_block ]
!43 = metadata !{i32 589839, metadata !11, metadata !"", metadata !11, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !44} ; [ DW_TAG_pointer_type ]
!44 = metadata !{i32 589860, metadata !11, metadata !"char", metadata !11, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!45 = metadata !{i32 590080, metadata !42, metadata !"src", metadata !11, i32 14, metadata !46, i32 0} ; [ DW_TAG_auto_variable ]
!46 = metadata !{i32 589839, metadata !11, metadata !"", metadata !11, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !47} ; [ DW_TAG_pointer_type ]
!47 = metadata !{i32 589862, metadata !11, metadata !"", metadata !11, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !44} ; [ DW_TAG_const_type ]
!48 = metadata !{i32 590081, metadata !19, metadata !"dst", metadata !20, i32 12, metadata !24, i32 0} ; [ DW_TAG_arg_variable ]
!49 = metadata !{i32 590081, metadata !19, metadata !"src", metadata !20, i32 12, metadata !24, i32 0} ; [ DW_TAG_arg_variable ]
!50 = metadata !{i32 590081, metadata !19, metadata !"count", metadata !20, i32 12, metadata !25, i32 0} ; [ DW_TAG_arg_variable ]
!51 = metadata !{i32 590080, metadata !52, metadata !"a", metadata !20, i32 13, metadata !53, i32 0} ; [ DW_TAG_auto_variable ]
!52 = metadata !{i32 589835, metadata !19, i32 12, i32 0, metadata !20, i32 0} ; [ DW_TAG_lexical_block ]
!53 = metadata !{i32 589839, metadata !20, metadata !"", metadata !20, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !54} ; [ DW_TAG_pointer_type ]
!54 = metadata !{i32 589860, metadata !20, metadata !"char", metadata !20, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!55 = metadata !{i32 590080, metadata !52, metadata !"b", metadata !20, i32 14, metadata !56, i32 0} ; [ DW_TAG_auto_variable ]
!56 = metadata !{i32 589839, metadata !20, metadata !"", metadata !20, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !57} ; [ DW_TAG_pointer_type ]
!57 = metadata !{i32 589862, metadata !20, metadata !"", metadata !20, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !54} ; [ DW_TAG_const_type ]
!58 = metadata !{i32 590081, metadata !28, metadata !"dst", metadata !29, i32 12, metadata !33, i32 0} ; [ DW_TAG_arg_variable ]
!59 = metadata !{i32 590081, metadata !28, metadata !"s", metadata !29, i32 12, metadata !34, i32 0} ; [ DW_TAG_arg_variable ]
!60 = metadata !{i32 590081, metadata !28, metadata !"count", metadata !29, i32 12, metadata !35, i32 0} ; [ DW_TAG_arg_variable ]
!61 = metadata !{i32 590080, metadata !62, metadata !"a", metadata !29, i32 13, metadata !63, i32 0} ; [ DW_TAG_auto_variable ]
!62 = metadata !{i32 589835, metadata !28, i32 12, i32 0, metadata !29, i32 0} ; [ DW_TAG_lexical_block ]
!63 = metadata !{i32 589839, metadata !29, metadata !"", metadata !29, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !64} ; [ DW_TAG_pointer_type ]
!64 = metadata !{i32 589877, metadata !29, metadata !"", metadata !29, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !65} ; [ DW_TAG_volatile_type ]
!65 = metadata !{i32 589860, metadata !29, metadata !"char", metadata !29, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!66 = metadata !{i32 4, i32 0, metadata !67, null}
!67 = metadata !{i32 589835, metadata !0, i32 2, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!68 = metadata !{i32 5, i32 0, metadata !67, null}
!69 = metadata !{i32 7, i32 0, metadata !67, null}
!70 = metadata !{i32 15, i32 0, metadata !71, null}
!71 = metadata !{i32 589835, metadata !7, i32 13, i32 0, metadata !1, i32 1} ; [ DW_TAG_lexical_block ]
!72 = metadata !{i32 18, i32 0, metadata !71, null}
!73 = metadata !{i32 19, i32 0, metadata !71, null}
