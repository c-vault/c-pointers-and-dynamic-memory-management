To install the C Pointers & Dynamic Memory Management Source Disk:

1) Make a directory on you hard disk. POINTERS is what we will use
   in this example:

             c:\>MD \POINTERS

2) Change directories so that POINTERS is the current directory:

             c:\>CD \POINTERS

3) Copy the contents of the diskette into your new directory:

             c:\POINTERS>COPY B:*.*

------------------------------------------------------------

The source files were originally developed on a Macintosh and then
ported to DOS and compiled using TURBO C.  To port the source to 
DOS changing the name was necessary.  Here is a list of the source
files in the book and the corresponding DOS file names.

Source 1.1  pointer_intro.c  PT_INTR.C
Source 2.1  globals.c  GLOBALS.C
Source 3.1  declare.c  DECLARE.C
Source 3.2  address_operator.c  ADDRESS_OP.C
Source 3.3  dereference.c  DEREFERENC.C
Source 4.1  stack_copy.c  STACK_CP.C
Source 4.2  pass_address.c  PASS_ADD.C
Source 5.1  index.c  INDEX.C
Source 5.2  test_scores.c  TST_SCOR.C
Source 6.1  struct_test.c  STRUCTST.C
Source 6.2  tiny_dict.c  TINY_DCT.C
Source 7.1  traverse.c  TRAVERSE.C
Source 7.2  count.c  COUNT.C
Source 8.1  strings.c  STRINGS.C
Source 8.2  string_tst.c  STR_TEST.C
Source 8.3  dynamic_scores.c  DYN_SCOR.C
Source 8.4  hyper_dict.c  HYPER_DC.C
Source 9.1  stack_variable.c  STACK_VR.C
Source 9.2  dynamic_initialize.c  DYN_INIT.C
Source 10.1 class_ppbuf.c  CLASSPPB.C
Source 10.2 ppbuf_utils.c  PPBUF_UT.C
Source 10.3 dissect.c  DISSECT.C
Source 10.4 cmd_line.c  CMD_LINE.C
Source 11.1 string_stack.c  STR_STCK.C
Source 11.2 test_stack.c  TST_STAK.C
Source 11.3 gll.c  GLL.C
Source 11.4 tstgll.c  TSTGLL.C
Source 11.5 generic_tree.c  GEN_TREE.C
Source 12.1 funcptr.c  FUNCPTR.C
Source 12.2 generic_compare.c  GEN_CMP.C
Source 12.3 dispatcher.c  DISPATCH.C
Source 12.4 changeprio.c  CHANGEPR.C
Source 13.1 fooalign.c  FOOALIGN.C
Source 13.2 - 13.14 debug_memory.c  DBG_MEM.C 
Source 13.15 memory_test.c  DBG_MEMT.C
Source 14.1 forget_null.c  FORGETNU.C
Source 14.2 bad_request.c  BAD_REQ.C
Source 14.3 bad_request2.c BAD_REQ2.C
Source 14.4 bad_array.c  BAD_ARRY.C
Source 14.5 tokenize_it.c  TOKENIZE.C
Source 14.6 terminator.c  TEMINATE.C
Source 14.7 BAD_multiple_exit.c  BAD_MULT.C
Source 14.8 BETTER_multiple_exit.c  BETTER_M.C
Source 14.9 BAD_precedence.c  BAD_PREC.C
Source 14.10 dangling1.c  DANGLING1.C
Source 14.11 dangling2.c  DANGLING2.C

Other Sources (paper computers and Chapter 15):
ptrptr.c  PTRPTR.C
simple_clex.c  SIMPLE_C.C
tokens.c TOKENS.C
GOOD_multiple.c  GOOD_MUL.C
ppblock_utils.c  PPBLOCK.C
declaring.c  DECLARIN.C
qsort.c  QSORT.C

PORTING NOTES:

1) All warnings received in TURBO C can be safely ignored.

2) TURBO C has a strdup() so the strdup() in strings.c should be
   commented out.

3) Macintosh console routines (non-portable) were changed to TURBO C
routines.  Mac programmers should refer to the text to change the calls
back.  (only affects two source files).
