; ModuleID = 'link-list1.o'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-f128:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

%struct.Node_t = type { i32, %struct.Node_t*, %struct.Node_t* }

@.str = private unnamed_addr constant [3 x i8] c"a0\00", align 1
@.str1 = private unnamed_addr constant [3 x i8] c"a1\00", align 1

define void @remove_from_list(%struct.Node_t** %head, %struct.Node_t* %node) nounwind {
entry:
  %head_addr = alloca %struct.Node_t**, align 8
  %node_addr = alloca %struct.Node_t*, align 8
  %"alloca point" = bitcast i32 0 to i32
  store %struct.Node_t** %head, %struct.Node_t*** %head_addr
  store %struct.Node_t* %node, %struct.Node_t** %node_addr
  %0 = load %struct.Node_t** %node_addr, align 8, !dbg !74
  %1 = getelementptr inbounds %struct.Node_t* %0, i32 0, i32 2, !dbg !74
  %2 = load %struct.Node_t** %1, align 8, !dbg !74
  %3 = load %struct.Node_t** %node_addr, align 8, !dbg !74
  %4 = icmp eq %struct.Node_t* %2, %3, !dbg !74
  br i1 %4, label %bb, label %bb1, !dbg !74

bb:                                               ; preds = %entry
  %5 = load %struct.Node_t*** %head_addr, align 8, !dbg !76
  store %struct.Node_t* null, %struct.Node_t** %5, align 8, !dbg !76
  br label %return, !dbg !76

bb1:                                              ; preds = %entry
  %6 = load %struct.Node_t** %node_addr, align 8, !dbg !77
  %7 = getelementptr inbounds %struct.Node_t* %6, i32 0, i32 2, !dbg !77
  %8 = load %struct.Node_t** %7, align 8, !dbg !77
  %9 = load %struct.Node_t** %node_addr, align 8, !dbg !77
  %10 = getelementptr inbounds %struct.Node_t* %9, i32 0, i32 1, !dbg !77
  %11 = load %struct.Node_t** %10, align 8, !dbg !77
  %12 = getelementptr inbounds %struct.Node_t* %8, i32 0, i32 1, !dbg !77
  store %struct.Node_t* %11, %struct.Node_t** %12, align 8, !dbg !77
  %13 = load %struct.Node_t** %node_addr, align 8, !dbg !78
  %14 = getelementptr inbounds %struct.Node_t* %13, i32 0, i32 1, !dbg !78
  %15 = load %struct.Node_t** %14, align 8, !dbg !78
  %16 = load %struct.Node_t** %node_addr, align 8, !dbg !78
  %17 = getelementptr inbounds %struct.Node_t* %16, i32 0, i32 2, !dbg !78
  %18 = load %struct.Node_t** %17, align 8, !dbg !78
  %19 = getelementptr inbounds %struct.Node_t* %15, i32 0, i32 2, !dbg !78
  store %struct.Node_t* %18, %struct.Node_t** %19, align 8, !dbg !78
  %20 = load %struct.Node_t*** %head_addr, align 8, !dbg !79
  %21 = load %struct.Node_t** %20, align 8, !dbg !79
  %22 = load %struct.Node_t** %node_addr, align 8, !dbg !79
  %23 = icmp eq %struct.Node_t* %21, %22, !dbg !79
  br i1 %23, label %bb2, label %return, !dbg !79

bb2:                                              ; preds = %bb1
  %24 = load %struct.Node_t** %node_addr, align 8, !dbg !80
  %25 = getelementptr inbounds %struct.Node_t* %24, i32 0, i32 1, !dbg !80
  %26 = load %struct.Node_t** %25, align 8, !dbg !80
  %27 = load %struct.Node_t*** %head_addr, align 8, !dbg !80
  store %struct.Node_t* %26, %struct.Node_t** %27, align 8, !dbg !80
  br label %return, !dbg !80

return:                                           ; preds = %bb, %bb1, %bb2
  ret void, !dbg !81
}

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

define i32 @main() nounwind {
entry:
  %retval = alloca i32
  %0 = alloca i32
  %a0 = alloca %struct.Node_t**
  %a1 = alloca %struct.Node_t*
  %"alloca point" = bitcast i32 0 to i32
  %1 = call noalias i8* @malloc(i64 8) nounwind, !dbg !82
  %2 = bitcast i8* %1 to %struct.Node_t**, !dbg !82
  store %struct.Node_t** %2, %struct.Node_t*** %a0, align 8, !dbg !82
  %3 = load %struct.Node_t*** %a0, align 8, !dbg !84
  %4 = bitcast %struct.Node_t** %3 to i8*, !dbg !84
  call void @klee_make_symbolic(i8* %4, i64 8, i8* getelementptr inbounds ([3 x i8]* @.str, i64 0, i64 0)) nounwind, !dbg !84
  %5 = call noalias i8* @malloc(i64 24) nounwind, !dbg !85
  %6 = bitcast i8* %5 to %struct.Node_t*, !dbg !85
  store %struct.Node_t* %6, %struct.Node_t** %a1, align 8, !dbg !85
  %7 = load %struct.Node_t** %a1, align 8, !dbg !86
  %8 = bitcast %struct.Node_t* %7 to i8*, !dbg !86
  call void @klee_make_symbolic(i8* %8, i64 24, i8* getelementptr inbounds ([3 x i8]* @.str1, i64 0, i64 0)) nounwind, !dbg !86
  %9 = load %struct.Node_t*** %a0, align 8, !dbg !87
  %10 = load %struct.Node_t** %a1, align 8, !dbg !87
  call void @remove_from_list(%struct.Node_t** %9, %struct.Node_t* %10) nounwind, !dbg !87
  store i32 0, i32* %0, align 4, !dbg !88
  %11 = load i32* %0, align 4, !dbg !88
  store i32 %11, i32* %retval, align 4, !dbg !88
  %retval1 = load i32* %retval, !dbg !88
  ret i32 %retval1, !dbg !88
}

declare noalias i8* @malloc(i64) nounwind

declare void @klee_make_symbolic(i8*, i64, i8*)

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0, !15, !18, !27, !36}
!llvm.dbg.lv.memcpy = !{!46, !47, !48, !49, !53}
!llvm.dbg.lv.memmove = !{!56, !57, !58, !59, !63}
!llvm.dbg.lv.memset = !{!66, !67, !68, !69}

!0 = metadata !{i32 589870, i32 0, metadata !1, metadata !"remove_from_list", metadata !"remove_from_list", metadata !"remove_from_list", metadata !1, i32 9, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, void (%struct.Node_t**, %
!1 = metadata !{i32 589865, metadata !"link-list1.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/Release+Asserts/bin/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 589841, i32 0, i32 1, metadata !"link-list1.c", metadata !"/home/qirong/Desktop/KLEE_SOURCE/Release+Asserts/bin/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 false, metadata !"", i32 0} ; [ DW_TAG
!3 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{null, metadata !5, metadata !6}
!5 = metadata !{i32 589839, metadata !1, metadata !"", metadata !1, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !6} ; [ DW_TAG_pointer_type ]
!6 = metadata !{i32 589839, metadata !1, metadata !"", metadata !1, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !7} ; [ DW_TAG_pointer_type ]
!7 = metadata !{i32 589846, metadata !1, metadata !"Node_t", metadata !1, i32 7, i64 0, i64 0, i64 0, i32 0, metadata !8} ; [ DW_TAG_typedef ]
!8 = metadata !{i32 589843, metadata !1, metadata !"Node", metadata !1, i32 3, i64 192, i64 64, i64 0, i32 0, null, metadata !9, i32 0, null} ; [ DW_TAG_structure_type ]
!9 = metadata !{metadata !10, metadata !12, metadata !14}
!10 = metadata !{i32 589837, metadata !8, metadata !"value", metadata !1, i32 4, i64 32, i64 32, i64 0, i32 0, metadata !11} ; [ DW_TAG_member ]
!11 = metadata !{i32 589860, metadata !1, metadata !"int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!12 = metadata !{i32 589837, metadata !8, metadata !"next", metadata !1, i32 5, i64 64, i64 64, i64 64, i32 0, metadata !13} ; [ DW_TAG_member ]
!13 = metadata !{i32 589839, metadata !1, metadata !"", metadata !1, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !8} ; [ DW_TAG_pointer_type ]
!14 = metadata !{i32 589837, metadata !8, metadata !"prev", metadata !1, i32 6, i64 64, i64 64, i64 128, i32 0, metadata !13} ; [ DW_TAG_member ]
!15 = metadata !{i32 589870, i32 0, metadata !1, metadata !"main", metadata !"main", metadata !"main", metadata !1, i32 27, metadata !16, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 ()* @main} ; [ DW_TAG_subprogram ]
!16 = metadata !{i32 589845, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !17, i32 0, null} ; [ DW_TAG_subroutine_type ]
!17 = metadata !{metadata !11}
!18 = metadata !{i32 589870, i32 0, metadata !19, metadata !"memcpy", metadata !"memcpy", metadata !"memcpy", metadata !19, i32 12, metadata !21, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!19 = metadata !{i32 589865, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !20} ; [ DW_TAG_file_type ]
!20 = metadata !{i32 589841, i32 0, i32 1, metadata !"memcpy.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!21 = metadata !{i32 589845, metadata !19, metadata !"", metadata !19, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !22, i32 0, null} ; [ DW_TAG_subroutine_type ]
!22 = metadata !{metadata !23, metadata !23, metadata !23, metadata !24}
!23 = metadata !{i32 589839, metadata !19, metadata !"", metadata !19, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!24 = metadata !{i32 589846, metadata !25, metadata !"size_t", metadata !25, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !26} ; [ DW_TAG_typedef ]
!25 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !20} ; [ DW_TAG_file_type ]
!26 = metadata !{i32 589860, metadata !19, metadata !"long unsigned int", metadata !19, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!27 = metadata !{i32 589870, i32 0, metadata !28, metadata !"memmove", metadata !"memmove", metadata !"memmove", metadata !28, i32 12, metadata !30, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!28 = metadata !{i32 589865, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !29} ; [ DW_TAG_file_type ]
!29 = metadata !{i32 589841, i32 0, i32 1, metadata !"memmove.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!30 = metadata !{i32 589845, metadata !28, metadata !"", metadata !28, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !31, i32 0, null} ; [ DW_TAG_subroutine_type ]
!31 = metadata !{metadata !32, metadata !32, metadata !32, metadata !33}
!32 = metadata !{i32 589839, metadata !28, metadata !"", metadata !28, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!33 = metadata !{i32 589846, metadata !34, metadata !"size_t", metadata !34, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !35} ; [ DW_TAG_typedef ]
!34 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !29} ; [ DW_TAG_file_type ]
!35 = metadata !{i32 589860, metadata !28, metadata !"long unsigned int", metadata !28, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!36 = metadata !{i32 589870, i32 0, metadata !37, metadata !"memset", metadata !"memset", metadata !"memset", metadata !37, i32 12, metadata !39, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, null} ; [ DW_TAG_subprogram ]
!37 = metadata !{i32 589865, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !38} ; [ DW_TAG_file_type ]
!38 = metadata !{i32 589841, i32 0, i32 1, metadata !"memset.c", metadata !"/home/kuan/klee/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.9)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!39 = metadata !{i32 589845, metadata !37, metadata !"", metadata !37, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !40, i32 0, null} ; [ DW_TAG_subroutine_type ]
!40 = metadata !{metadata !41, metadata !41, metadata !42, metadata !43}
!41 = metadata !{i32 589839, metadata !37, metadata !"", metadata !37, i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!42 = metadata !{i32 589860, metadata !37, metadata !"int", metadata !37, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!43 = metadata !{i32 589846, metadata !44, metadata !"size_t", metadata !44, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !45} ; [ DW_TAG_typedef ]
!44 = metadata !{i32 589865, metadata !"stddef.h", metadata !"/home/kuan/llvm-gcc4.2-2.9-x86_64-linux/bin/../lib/gcc/x86_64-unknown-linux-gnu/4.2.1/include", metadata !38} ; [ DW_TAG_file_type ]
!45 = metadata !{i32 589860, metadata !37, metadata !"long unsigned int", metadata !37, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!46 = metadata !{i32 590081, metadata !18, metadata !"destaddr", metadata !19, i32 12, metadata !23, i32 0} ; [ DW_TAG_arg_variable ]
!47 = metadata !{i32 590081, metadata !18, metadata !"srcaddr", metadata !19, i32 12, metadata !23, i32 0} ; [ DW_TAG_arg_variable ]
!48 = metadata !{i32 590081, metadata !18, metadata !"len", metadata !19, i32 12, metadata !24, i32 0} ; [ DW_TAG_arg_variable ]
!49 = metadata !{i32 590080, metadata !50, metadata !"dest", metadata !19, i32 13, metadata !51, i32 0} ; [ DW_TAG_auto_variable ]
!50 = metadata !{i32 589835, metadata !18, i32 12, i32 0, metadata !19, i32 0} ; [ DW_TAG_lexical_block ]
!51 = metadata !{i32 589839, metadata !19, metadata !"", metadata !19, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !52} ; [ DW_TAG_pointer_type ]
!52 = metadata !{i32 589860, metadata !19, metadata !"char", metadata !19, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!53 = metadata !{i32 590080, metadata !50, metadata !"src", metadata !19, i32 14, metadata !54, i32 0} ; [ DW_TAG_auto_variable ]
!54 = metadata !{i32 589839, metadata !19, metadata !"", metadata !19, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !55} ; [ DW_TAG_pointer_type ]
!55 = metadata !{i32 589862, metadata !19, metadata !"", metadata !19, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !52} ; [ DW_TAG_const_type ]
!56 = metadata !{i32 590081, metadata !27, metadata !"dst", metadata !28, i32 12, metadata !32, i32 0} ; [ DW_TAG_arg_variable ]
!57 = metadata !{i32 590081, metadata !27, metadata !"src", metadata !28, i32 12, metadata !32, i32 0} ; [ DW_TAG_arg_variable ]
!58 = metadata !{i32 590081, metadata !27, metadata !"count", metadata !28, i32 12, metadata !33, i32 0} ; [ DW_TAG_arg_variable ]
!59 = metadata !{i32 590080, metadata !60, metadata !"a", metadata !28, i32 13, metadata !61, i32 0} ; [ DW_TAG_auto_variable ]
!60 = metadata !{i32 589835, metadata !27, i32 12, i32 0, metadata !28, i32 0} ; [ DW_TAG_lexical_block ]
!61 = metadata !{i32 589839, metadata !28, metadata !"", metadata !28, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !62} ; [ DW_TAG_pointer_type ]
!62 = metadata !{i32 589860, metadata !28, metadata !"char", metadata !28, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!63 = metadata !{i32 590080, metadata !60, metadata !"b", metadata !28, i32 14, metadata !64, i32 0} ; [ DW_TAG_auto_variable ]
!64 = metadata !{i32 589839, metadata !28, metadata !"", metadata !28, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !65} ; [ DW_TAG_pointer_type ]
!65 = metadata !{i32 589862, metadata !28, metadata !"", metadata !28, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !62} ; [ DW_TAG_const_type ]
!66 = metadata !{i32 590081, metadata !36, metadata !"dst", metadata !37, i32 12, metadata !41, i32 0} ; [ DW_TAG_arg_variable ]
!67 = metadata !{i32 590081, metadata !36, metadata !"s", metadata !37, i32 12, metadata !42, i32 0} ; [ DW_TAG_arg_variable ]
!68 = metadata !{i32 590081, metadata !36, metadata !"count", metadata !37, i32 12, metadata !43, i32 0} ; [ DW_TAG_arg_variable ]
!69 = metadata !{i32 590080, metadata !70, metadata !"a", metadata !37, i32 13, metadata !71, i32 0} ; [ DW_TAG_auto_variable ]
!70 = metadata !{i32 589835, metadata !36, i32 12, i32 0, metadata !37, i32 0} ; [ DW_TAG_lexical_block ]
!71 = metadata !{i32 589839, metadata !37, metadata !"", metadata !37, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !72} ; [ DW_TAG_pointer_type ]
!72 = metadata !{i32 589877, metadata !37, metadata !"", metadata !37, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !73} ; [ DW_TAG_volatile_type ]
!73 = metadata !{i32 589860, metadata !37, metadata !"char", metadata !37, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!74 = metadata !{i32 11, i32 0, metadata !75, null}
!75 = metadata !{i32 589835, metadata !0, i32 9, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!76 = metadata !{i32 13, i32 0, metadata !75, null}
!77 = metadata !{i32 16, i32 0, metadata !75, null}
!78 = metadata !{i32 17, i32 0, metadata !75, null}
!79 = metadata !{i32 19, i32 0, metadata !75, null}
!80 = metadata !{i32 20, i32 0, metadata !75, null}
!81 = metadata !{i32 22, i32 0, metadata !75, null}
!82 = metadata !{i32 30, i32 0, metadata !83, null}
!83 = metadata !{i32 589835, metadata !15, i32 27, i32 0, metadata !1, i32 1} ; [ DW_TAG_lexical_block ]
!84 = metadata !{i32 32, i32 0, metadata !83, null}
!85 = metadata !{i32 35, i32 0, metadata !83, null}
!86 = metadata !{i32 36, i32 0, metadata !83, null}
!87 = metadata !{i32 38, i32 0, metadata !83, null}
!88 = metadata !{i32 40, i32 0, metadata !83, null}
