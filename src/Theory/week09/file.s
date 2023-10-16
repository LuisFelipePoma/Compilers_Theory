	.text
	.file	"file.c"
	.globl	prefix_sum                      # -- Begin function prefix_sum
	.p2align	4, 0x90
	.type	prefix_sum,@function
prefix_sum:                             # @prefix_sum
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	%edx, -20(%rbp)
	xorl	%eax, %eax
	cmpl	-20(%rbp), %eax
	jge	.LBB0_10
# %bb.1:
	movl	$0, -24(%rbp)
.LBB0_2:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_4 Depth 2
	movl	$0, -28(%rbp)
	movl	$0, -32(%rbp)
	movl	-32(%rbp), %eax
	cmpl	-24(%rbp), %eax
	jge	.LBB0_7
# %bb.3:                                #   in Loop: Header=BB0_2 Depth=1
	jmp	.LBB0_4
.LBB0_4:                                #   Parent Loop BB0_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movq	-8(%rbp), %rax
	movslq	-32(%rbp), %rcx
	movl	(%rax,%rcx,4), %eax
	addl	-28(%rbp), %eax
	movl	%eax, -28(%rbp)
	movl	-32(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -32(%rbp)
# %bb.5:                                #   in Loop: Header=BB0_4 Depth=2
	movl	-32(%rbp), %eax
	cmpl	-24(%rbp), %eax
	jl	.LBB0_4
# %bb.6:                                #   in Loop: Header=BB0_2 Depth=1
	movl	-28(%rbp), %edx
	movq	-16(%rbp), %rax
	movslq	-24(%rbp), %rcx
	movl	%edx, (%rax,%rcx,4)
.LBB0_7:                                #   in Loop: Header=BB0_2 Depth=1
	movl	-24(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -24(%rbp)
# %bb.8:                                #   in Loop: Header=BB0_2 Depth=1
	movl	-24(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.LBB0_2
# %bb.9:
	jmp	.LBB0_10
.LBB0_10:
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	prefix_sum, .Lfunc_end0-prefix_sum
	.cfi_endproc
                                        # -- End function
	.ident	"Debian clang version 14.0.6"
	.section	".note.GNU-stack","",@progbits
	.addrsig
