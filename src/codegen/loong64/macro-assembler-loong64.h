// Copyright 2021 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDED_FROM_MACRO_ASSEMBLER_H
#error This header must be included via macro-assembler.h
#endif

#ifndef V8_CODEGEN_LOONG64_MACRO_ASSEMBLER_LOONG64_H_
#define V8_CODEGEN_LOONG64_MACRO_ASSEMBLER_LOONG64_H_

#include "src/codegen/assembler.h"
#include "src/codegen/loong64/assembler-loong64.h"
#include "src/common/globals.h"
#include "src/objects/tagged-index.h"

namespace v8 {
namespace internal {

// Forward declarations.
enum class AbortReason : uint8_t;

// Flags used for LeaveExitFrame function.
enum LeaveExitFrameMode { EMIT_RETURN = true, NO_EMIT_RETURN = false };

// Flags used for the li macro-assembler function.
enum LiFlags {
  // If the constant value can be represented in just 12 bits, then
  // optimize the li to use a single instruction, rather than lu12i_w/lu32i_d/
  // lu52i_d/ori sequence. A number of other optimizations that emits less than
  // maximum number of instructions exists.
  OPTIMIZE_SIZE = 0,
  // Always use 4 instructions (lu12i_w/ori/lu32i_d/lu52i_d sequence),
  // even if the constant could be loaded with just one, so that this value is
  // patchable later.
  CONSTANT_SIZE = 1,
  // For address loads only 3 instruction are required. Used to mark
  // constant load that will be used as address without relocation
  // information. It ensures predictable code size, so specific sites
  // in code are patchable.
  ADDRESS_LOAD = 2
};

enum RAStatus { kRAHasNotBeenSaved, kRAHasBeenSaved };

Register GetRegisterThatIsNotOneOf(Register reg1, Register reg2 = no_reg,
                                   Register reg3 = no_reg,
                                   Register reg4 = no_reg,
                                   Register reg5 = no_reg,
                                   Register reg6 = no_reg);

// -----------------------------------------------------------------------------
// Static helper functions.

#define SmiWordOffset(offset) (offset + kPointerSize / 2)

// Generate a MemOperand for loading a field from an object.
inline MemOperand FieldMemOperand(Register object, int offset) {
  return MemOperand(object, offset - kHeapObjectTag);
}

class V8_EXPORT_PRIVATE TurboAssembler : public TurboAssemblerBase {
 public:
  using TurboAssemblerBase::TurboAssemblerBase;

  // Activation support.
  void EnterFrame(StackFrame::Type type);
  void EnterFrame(StackFrame::Type type, bool load_constant_pool_pointer_reg) {
    // Out-of-line constant pool not implemented on loong64.
    UNREACHABLE();
  }
  void LeaveFrame(StackFrame::Type type);

  void AllocateStackSpace(Register bytes) { Sub_d(sp, sp, bytes); }

  void AllocateStackSpace(int bytes) {
    DCHECK_GE(bytes, 0);
    if (bytes == 0) return;
    Sub_d(sp, sp, Operand(bytes));
  }

  // Generates function and stub prologue code.
  void StubPrologue(StackFrame::Type type);
  void Prologue();

  void InitializeRootRegister() {
    ExternalReference isolate_root = ExternalReference::isolate_root(isolate());
    li(kRootRegister, Operand(isolate_root));
  }

  // Jump unconditionally to given label.
  // Use rather b(Label) for code generation.
  void jmp(Label* L) { Branch(L); }

  // -------------------------------------------------------------------------
  // Debugging.

  void Trap();
  void DebugBreak();

  // Calls Abort(msg) if the condition cc is not satisfied.
  // Use --debug_code to enable.
  void Assert(Condition cc, AbortReason reason, Register rj, Operand rk);

  // Like Assert(), but always enabled.
  void Check(Condition cc, AbortReason reason, Register rj, Operand rk);

  // Print a message to stdout and abort execution.
  void Abort(AbortReason msg);

  void Branch(Label* label, bool need_link = false);
  void Branch(Label* label, Condition cond, Register r1, const Operand& r2,
              bool need_link = false);
  void BranchShort(Label* label, Condition cond, Register r1, const Operand& r2,
                   bool need_link = false);
  void Branch(Label* L, Condition cond, Register rj, RootIndex index);

  // Floating point branches
  void CompareF32(FPURegister cmp1, FPURegister cmp2, FPUCondition cc,
                  CFRegister cd = FCC0) {
    CompareF(cmp1, cmp2, cc, cd, true);
  }

  void CompareIsNanF32(FPURegister cmp1, FPURegister cmp2,
                       CFRegister cd = FCC0) {
    CompareIsNanF(cmp1, cmp2, cd, true);
  }

  void CompareF64(FPURegister cmp1, FPURegister cmp2, FPUCondition cc,
                  CFRegister cd = FCC0) {
    CompareF(cmp1, cmp2, cc, cd, false);
  }

  void CompareIsNanF64(FPURegister cmp1, FPURegister cmp2,
                       CFRegister cd = FCC0) {
    CompareIsNanF(cmp1, cmp2, cd, false);
  }

  void BranchTrueShortF(Label* target, CFRegister cc = FCC0);
  void BranchFalseShortF(Label* target, CFRegister cc = FCC0);

  void BranchTrueF(Label* target, CFRegister cc = FCC0);
  void BranchFalseF(Label* target, CFRegister cc = FCC0);

  static int InstrCountForLi64Bit(int64_t value);
  inline void LiLower32BitHelper(Register rd, Operand j);
  void li_optimized(Register rd, Operand j, LiFlags mode = OPTIMIZE_SIZE);
  void li(Register rd, Operand j, LiFlags mode = OPTIMIZE_SIZE);
  inline void li(Register rd, int64_t j, LiFlags mode = OPTIMIZE_SIZE) {
    li(rd, Operand(j), mode);
  }
  inline void li(Register rd, int32_t j, LiFlags mode = OPTIMIZE_SIZE) {
    li(rd, Operand(static_cast<int64_t>(j)), mode);
  }
  void li(Register dst, Handle<HeapObject> value, LiFlags mode = OPTIMIZE_SIZE);
  void li(Register dst, ExternalReference value, LiFlags mode = OPTIMIZE_SIZE);
  void li(Register dst, const StringConstantBase* string,
          LiFlags mode = OPTIMIZE_SIZE);

  void LoadFromConstantsTable(Register destination, int constant_index) final;
  void LoadRootRegisterOffset(Register destination, intptr_t offset) final;
  void LoadRootRelative(Register destination, int32_t offset) final;

  inline void Move(Register output, MemOperand operand) {
    Ld_d(output, operand);
  }

  inline void GenPCRelativeJump(Register rd, int64_t offset);
  inline void GenPCRelativeJumpAndLink(Register rd, int64_t offset);

// Jump, Call, and Ret pseudo instructions implementing inter-working.
#define COND_ARGS                              \
  Condition cond = al, Register rj = zero_reg, \
            const Operand &rk = Operand(zero_reg)

  void Jump(Register target, COND_ARGS);
  void Jump(intptr_t target, RelocInfo::Mode rmode, COND_ARGS);
  void Jump(Address target, RelocInfo::Mode rmode, COND_ARGS);
  // Deffer from li, this method save target to the memory, and then load
  // it to register use ld_d, it can be used in wasm jump table for concurrent
  // patching.
  void PatchAndJump(Address target);
  void Jump(Handle<Code> code, RelocInfo::Mode rmode, COND_ARGS);
  void Jump(const ExternalReference& reference);
  void Call(Register target, COND_ARGS);
  void Call(Address target, RelocInfo::Mode rmode, COND_ARGS);
  void Call(Handle<Code> code, RelocInfo::Mode rmode = RelocInfo::CODE_TARGET,
            COND_ARGS);
  void Call(Label* target);

  // Load the builtin given by the Smi in |builtin_index| into the same
  // register.
  void LoadEntryFromBuiltinIndex(Register builtin);
  void LoadEntryFromBuiltin(Builtin builtin, Register destination);
  MemOperand EntryFromBuiltinAsOperand(Builtin builtin);

  void CallBuiltinByIndex(Register builtin);
  void CallBuiltin(Builtin builtin);

  void LoadCodeObjectEntry(Register destination, Register code_object);
  void CallCodeObject(Register code_object);

  void JumpCodeObject(Register code_object,
                      JumpMode jump_mode = JumpMode::kJump);

  // Generates an instruction sequence s.t. the return address points to the
  // instruction following the call.
  // The return address on the stack is used by frame iteration.
  void StoreReturnAddressAndCall(Register target);

  void CallForDeoptimization(Builtin target, int deopt_id, Label* exit,
                             DeoptimizeKind kind, Label* ret,
                             Label* jump_deoptimization_entry_label);

  void Ret(COND_ARGS);

  // Emit code to discard a non-negative number of pointer-sized elements
  // from the stack, clobbering only the sp register.
  void Drop(int count, Condition cond = cc_always, Register reg = no_reg,
            const Operand& op = Operand(no_reg));

  void Ld_d(Register rd, const MemOperand& rj);
  void St_d(Register rd, const MemOperand& rj);

  void Push(Handle<HeapObject> handle);
  void Push(Smi smi);

  void Push(Register src) {
    Add_d(sp, sp, Operand(-kPointerSize));
    St_d(src, MemOperand(sp, 0));
  }

  // Push two registers. Pushes leftmost register first (to highest address).
  void Push(Register src1, Register src2) {
    Sub_d(sp, sp, Operand(2 * kPointerSize));
    St_d(src1, MemOperand(sp, 1 * kPointerSize));
    St_d(src2, MemOperand(sp, 0 * kPointerSize));
  }

  // Push three registers. Pushes leftmost register first (to highest address).
  void Push(Register src1, Register src2, Register src3) {
    Sub_d(sp, sp, Operand(3 * kPointerSize));
    St_d(src1, MemOperand(sp, 2 * kPointerSize));
    St_d(src2, MemOperand(sp, 1 * kPointerSize));
    St_d(src3, MemOperand(sp, 0 * kPointerSize));
  }

  // Push four registers. Pushes leftmost register first (to highest address).
  void Push(Register src1, Register src2, Register src3, Register src4) {
    Sub_d(sp, sp, Operand(4 * kPointerSize));
    St_d(src1, MemOperand(sp, 3 * kPointerSize));
    St_d(src2, MemOperand(sp, 2 * kPointerSize));
    St_d(src3, MemOperand(sp, 1 * kPointerSize));
    St_d(src4, MemOperand(sp, 0 * kPointerSize));
  }

  // Push five registers. Pushes leftmost register first (to highest address).
  void Push(Register src1, Register src2, Register src3, Register src4,
            Register src5) {
    Sub_d(sp, sp, Operand(5 * kPointerSize));
    St_d(src1, MemOperand(sp, 4 * kPointerSize));
    St_d(src2, MemOperand(sp, 3 * kPointerSize));
    St_d(src3, MemOperand(sp, 2 * kPointerSize));
    St_d(src4, MemOperand(sp, 1 * kPointerSize));
    St_d(src5, MemOperand(sp, 0 * kPointerSize));
  }

  enum PushArrayOrder { kNormal, kReverse };
  void PushArray(Register array, Register size, Register scratch,
                 Register scratch2, PushArrayOrder order = kNormal);

  void MaybeSaveRegisters(RegList registers);
  void MaybeRestoreRegisters(RegList registers);

  void CallEphemeronKeyBarrier(Register object, Operand offset,
                               SaveFPRegsMode fp_mode);

  void CallRecordWriteStubSaveRegisters(
      Register object, Operand offset,
      RememberedSetAction remembered_set_action, SaveFPRegsMode fp_mode,
      StubCallMode mode = StubCallMode::kCallBuiltinPointer);
  void CallRecordWriteStub(
      Register object, Register slot_address,
      RememberedSetAction remembered_set_action, SaveFPRegsMode fp_mode,
      StubCallMode mode = StubCallMode::kCallBuiltinPointer);

  // For a given |object| and |offset|:
  //   - Move |object| to |dst_object|.
  //   - Compute the address of the slot pointed to by |offset| in |object| and
  //     write it to |dst_slot|.
  // This method makes sure |object| and |offset| are allowed to overlap with
  // the destination registers.
  void MoveObjectAndSlot(Register dst_object, Register dst_slot,
                         Register object, Operand offset);

  // Push multiple registers on the stack.
  // Registers are saved in numerical order, with higher numbered registers
  // saved in higher memory addresses.
  void MultiPush(RegList regs);
  void MultiPush(RegList regs1, RegList regs2);
  void MultiPush(RegList regs1, RegList regs2, RegList regs3);
  void MultiPushFPU(RegList regs);

  // Calculate how much stack space (in bytes) are required to store caller
  // registers excluding those specified in the arguments.
  int RequiredStackSizeForCallerSaved(SaveFPRegsMode fp_mode,
                                      Register exclusion1 = no_reg,
                                      Register exclusion2 = no_reg,
                                      Register exclusion3 = no_reg) const;

  // Push caller saved registers on the stack, and return the number of bytes
  // stack pointer is adjusted.
  int PushCallerSaved(SaveFPRegsMode fp_mode, Register exclusion1 = no_reg,
                      Register exclusion2 = no_reg,
                      Register exclusion3 = no_reg);
  // Restore caller saved registers from the stack, and return the number of
  // bytes stack pointer is adjusted.
  int PopCallerSaved(SaveFPRegsMode fp_mode, Register exclusion1 = no_reg,
                     Register exclusion2 = no_reg,
                     Register exclusion3 = no_reg);

  void Pop(Register dst) {
    Ld_d(dst, MemOperand(sp, 0));
    Add_d(sp, sp, Operand(kPointerSize));
  }

  // Pop two registers. Pops rightmost register first (from lower address).
  void Pop(Register src1, Register src2) {
    DCHECK(src1 != src2);
    Ld_d(src2, MemOperand(sp, 0 * kPointerSize));
    Ld_d(src1, MemOperand(sp, 1 * kPointerSize));
    Add_d(sp, sp, 2 * kPointerSize);
  }

  // Pop three registers. Pops rightmost register first (from lower address).
  void Pop(Register src1, Register src2, Register src3) {
    Ld_d(src3, MemOperand(sp, 0 * kPointerSize));
    Ld_d(src2, MemOperand(sp, 1 * kPointerSize));
    Ld_d(src1, MemOperand(sp, 2 * kPointerSize));
    Add_d(sp, sp, 3 * kPointerSize);
  }

  // Pops multiple values from the stack and load them in the
  // registers specified in regs. Pop order is the opposite as in MultiPush.
  void MultiPop(RegList regs);
  void MultiPop(RegList regs1, RegList regs2);
  void MultiPop(RegList regs1, RegList regs2, RegList regs3);

  void MultiPopFPU(RegList regs);

#define DEFINE_INSTRUCTION(instr)                          \
  void instr(Register rd, Register rj, const Operand& rk); \
  void instr(Register rd, Register rj, Register rk) {      \
    instr(rd, rj, Operand(rk));                            \
  }                                                        \
  void instr(Register rj, Register rk, int32_t j) { instr(rj, rk, Operand(j)); }

#define DEFINE_INSTRUCTION2(instr)                                 \
  void instr(Register rj, const Operand& rk);                      \
  void instr(Register rj, Register rk) { instr(rj, Operand(rk)); } \
  void instr(Register rj, int32_t j) { instr(rj, Operand(j)); }

  DEFINE_INSTRUCTION(Add_w)
  DEFINE_INSTRUCTION(Add_d)
  DEFINE_INSTRUCTION(Div_w)
  DEFINE_INSTRUCTION(Div_wu)
  DEFINE_INSTRUCTION(Div_du)
  DEFINE_INSTRUCTION(Mod_w)
  DEFINE_INSTRUCTION(Mod_wu)
  DEFINE_INSTRUCTION(Div_d)
  DEFINE_INSTRUCTION(Sub_w)
  DEFINE_INSTRUCTION(Sub_d)
  DEFINE_INSTRUCTION(Mod_d)
  DEFINE_INSTRUCTION(Mod_du)
  DEFINE_INSTRUCTION(Mul_w)
  DEFINE_INSTRUCTION(Mulh_w)
  DEFINE_INSTRUCTION(Mulh_wu)
  DEFINE_INSTRUCTION(Mul_d)
  DEFINE_INSTRUCTION(Mulh_d)
  DEFINE_INSTRUCTION2(Div_w)
  DEFINE_INSTRUCTION2(Div_d)
  DEFINE_INSTRUCTION2(Div_wu)
  DEFINE_INSTRUCTION2(Div_du)

  DEFINE_INSTRUCTION(And)
  DEFINE_INSTRUCTION(Or)
  DEFINE_INSTRUCTION(Xor)
  DEFINE_INSTRUCTION(Nor)
  DEFINE_INSTRUCTION2(Neg)
  DEFINE_INSTRUCTION(Andn)
  DEFINE_INSTRUCTION(Orn)

  DEFINE_INSTRUCTION(Slt)
  DEFINE_INSTRUCTION(Sltu)
  DEFINE_INSTRUCTION(Slti)
  DEFINE_INSTRUCTION(Sltiu)
  DEFINE_INSTRUCTION(Sle)
  DEFINE_INSTRUCTION(Sleu)
  DEFINE_INSTRUCTION(Sgt)
  DEFINE_INSTRUCTION(Sgtu)
  DEFINE_INSTRUCTION(Sge)
  DEFINE_INSTRUCTION(Sgeu)

  DEFINE_INSTRUCTION(Rotr_w)
  DEFINE_INSTRUCTION(Rotr_d)

#undef DEFINE_INSTRUCTION
#undef DEFINE_INSTRUCTION2
#undef DEFINE_INSTRUCTION3

  void SmiUntag(Register dst, const MemOperand& src);
  void SmiUntag(Register dst, Register src) {
    if (SmiValuesAre32Bits()) {
      srai_d(dst, src, kSmiShift);
    } else {
      DCHECK(SmiValuesAre31Bits());
      srai_w(dst, src, kSmiShift);
    }
  }

  void SmiUntag(Register reg) { SmiUntag(reg, reg); }

  int CalculateStackPassedWords(int num_reg_arguments,
                                int num_double_arguments);

  // Before calling a C-function from generated code, align arguments on stack.
  // After aligning the frame, non-register arguments must be stored on the
  // stack, after the argument-slots using helper: CFunctionArgumentOperand().
  // The argument count assumes all arguments are word sized.
  // Some compilers/platforms require the stack to be aligned when calling
  // C++ code.
  // Needs a scratch register to do some arithmetic. This register will be
  // trashed.
  void PrepareCallCFunction(int num_reg_arguments, int num_double_registers,
                            Register scratch);
  void PrepareCallCFunction(int num_reg_arguments, Register scratch);

  // Calls a C function and cleans up the space for arguments allocated
  // by PrepareCallCFunction. The called function is not allowed to trigger a
  // garbage collection, since that might move the code and invalidate the
  // return address (unless this is somehow accounted for by the called
  // function).
  void CallCFunction(ExternalReference function, int num_arguments);
  void CallCFunction(Register function, int num_arguments);
  void CallCFunction(ExternalReference function, int num_reg_arguments,
                     int num_double_arguments);
  void CallCFunction(Register function, int num_reg_arguments,
                     int num_double_arguments);

  // See comments at the beginning of Builtins::Generate_CEntry.
  inline void PrepareCEntryArgs(int num_args) { li(a0, num_args); }
  inline void PrepareCEntryFunction(const ExternalReference& ref) {
    li(a1, ref);
  }

  void CheckPageFlag(const Register& object, int mask, Condition cc,
                     Label* condition_met);
#undef COND_ARGS

  // Performs a truncating conversion of a floating point number as used by
  // the JS bitwise operations. See ECMA-262 9.5: ToInt32.
  // Exits with 'result' holding the answer.
  void TruncateDoubleToI(Isolate* isolate, Zone* zone, Register result,
                         DoubleRegister double_input, StubCallMode stub_mode);

  // Conditional move.
  void Movz(Register rd, Register rj, Register rk);
  void Movn(Register rd, Register rj, Register rk);

  void LoadZeroIfFPUCondition(Register dest, CFRegister = FCC0);
  void LoadZeroIfNotFPUCondition(Register dest, CFRegister = FCC0);

  void LoadZeroIfConditionNotZero(Register dest, Register condition);
  void LoadZeroIfConditionZero(Register dest, Register condition);
  void LoadZeroOnCondition(Register rd, Register rj, const Operand& rk,
                           Condition cond);

  void Clz_w(Register rd, Register rj);
  void Clz_d(Register rd, Register rj);
  void Ctz_w(Register rd, Register rj);
  void Ctz_d(Register rd, Register rj);
  void Popcnt_w(Register rd, Register rj);
  void Popcnt_d(Register rd, Register rj);

  void ExtractBits(Register dest, Register source, Register pos, int size,
                   bool sign_extend = false);
  void InsertBits(Register dest, Register source, Register pos, int size);

  void Bstrins_w(Register rk, Register rj, uint16_t msbw, uint16_t lswb);
  void Bstrins_d(Register rk, Register rj, uint16_t msbw, uint16_t lsbw);
  void Bstrpick_w(Register rk, Register rj, uint16_t msbw, uint16_t lsbw);
  void Bstrpick_d(Register rk, Register rj, uint16_t msbw, uint16_t lsbw);
  void Neg_s(FPURegister fd, FPURegister fj);
  void Neg_d(FPURegister fd, FPURegister fk);

  // Convert single to unsigned word.
  void Trunc_uw_s(FPURegister fd, FPURegister fj, FPURegister scratch);
  void Trunc_uw_s(Register rd, FPURegister fj, FPURegister scratch);

  // Change endianness
  void ByteSwapSigned(Register dest, Register src, int operand_size);
  void ByteSwapUnsigned(Register dest, Register src, int operand_size);

  void Ld_b(Register rd, const MemOperand& rj);
  void Ld_bu(Register rd, const MemOperand& rj);
  void St_b(Register rd, const MemOperand& rj);

  void Ld_h(Register rd, const MemOperand& rj);
  void Ld_hu(Register rd, const MemOperand& rj);
  void St_h(Register rd, const MemOperand& rj);

  void Ld_w(Register rd, const MemOperand& rj);
  void Ld_wu(Register rd, const MemOperand& rj);
  void St_w(Register rd, const MemOperand& rj);

  void Fld_s(FPURegister fd, const MemOperand& src);
  void Fst_s(FPURegister fj, const MemOperand& dst);

  void Fld_d(FPURegister fd, const MemOperand& src);
  void Fst_d(FPURegister fj, const MemOperand& dst);

  void Ll_w(Register rd, const MemOperand& rj);
  void Sc_w(Register rd, const MemOperand& rj);

  void Ll_d(Register rd, const MemOperand& rj);
  void Sc_d(Register rd, const MemOperand& rj);

  // These functions assume (and assert) that src1!=src2. It is permitted
  // for the result to alias either input register.
  void Float32Max(FPURegister dst, FPURegister src1, FPURegister src2,
                  Label* out_of_line);
  void Float32Min(FPURegister dst, FPURegister src1, FPURegister src2,
                  Label* out_of_line);
  void Float64Max(FPURegister dst, FPURegister src1, FPURegister src2,
                  Label* out_of_line);
  void Float64Min(FPURegister dst, FPURegister src1, FPURegister src2,
                  Label* out_of_line);

  // Generate out-of-line cases for the macros above.
  void Float32MaxOutOfLine(FPURegister dst, FPURegister src1, FPURegister src2);
  void Float32MinOutOfLine(FPURegister dst, FPURegister src1, FPURegister src2);
  void Float64MaxOutOfLine(FPURegister dst, FPURegister src1, FPURegister src2);
  void Float64MinOutOfLine(FPURegister dst, FPURegister src1, FPURegister src2);

  bool IsDoubleZeroRegSet() { return has_double_zero_reg_set_; }

  void mov(Register rd, Register rj) { or_(rd, rj, zero_reg); }

  inline void Move(Register dst, Handle<HeapObject> handle) { li(dst, handle); }
  inline void Move(Register dst, Smi smi) { li(dst, Operand(smi)); }

  inline void Move(Register dst, Register src) {
    if (dst != src) {
      mov(dst, src);
    }
  }

  inline void FmoveLow(Register dst_low, FPURegister src) {
    movfr2gr_s(dst_low, src);
  }

  void FmoveLow(FPURegister dst, Register src_low);

  inline void Move(FPURegister dst, FPURegister src) { Move_d(dst, src); }

  inline void Move_d(FPURegister dst, FPURegister src) {
    if (dst != src) {
      fmov_d(dst, src);
    }
  }

  inline void Move_s(FPURegister dst, FPURegister src) {
    if (dst != src) {
      fmov_s(dst, src);
    }
  }

  void Move(FPURegister dst, float imm) { Move(dst, bit_cast<uint32_t>(imm)); }
  void Move(FPURegister dst, double imm) { Move(dst, bit_cast<uint64_t>(imm)); }
  void Move(FPURegister dst, uint32_t src);
  void Move(FPURegister dst, uint64_t src);

  // AddOverflow_d sets overflow register to a negative value if
  // overflow occured, otherwise it is zero or positive
  void AddOverflow_d(Register dst, Register left, const Operand& right,
                     Register overflow);
  // SubOverflow_d sets overflow register to a negative value if
  // overflow occured, otherwise it is zero or positive
  void SubOverflow_d(Register dst, Register left, const Operand& right,
                     Register overflow);
  // MulOverflow_w sets overflow register to zero if no overflow occured
  void MulOverflow_w(Register dst, Register left, const Operand& right,
                     Register overflow);

  // TODO(LOONG_dev): LOONG64 Remove this constant
  // Number of instructions needed for calculation of switch table entry address
  static const int kSwitchTablePrologueSize = 5;

  // GetLabelFunction must be lambda '[](size_t index) -> Label*' or a
  // functor/function with 'Label *func(size_t index)' declaration.
  template <typename Func>
  void GenerateSwitchTable(Register index, size_t case_count,
                           Func GetLabelFunction);

  // Load an object from the root table.
  void LoadRoot(Register destination, RootIndex index) final;
  void LoadRoot(Register destination, RootIndex index, Condition cond,
                Register src1, const Operand& src2);

  void LoadMap(Register destination, Register object);

  // If the value is a NaN, canonicalize the value else, do nothing.
  void FPUCanonicalizeNaN(const DoubleRegister dst, const DoubleRegister src);

  // ---------------------------------------------------------------------------
  // FPU macros. These do not handle special cases like NaN or +- inf.

  // Convert unsigned word to double.
  void Ffint_d_uw(FPURegister fd, FPURegister fj);
  void Ffint_d_uw(FPURegister fd, Register rj);

  // Convert unsigned long to double.
  void Ffint_d_ul(FPURegister fd, FPURegister fj);
  void Ffint_d_ul(FPURegister fd, Register rj);

  // Convert unsigned word to float.
  void Ffint_s_uw(FPURegister fd, FPURegister fj);
  void Ffint_s_uw(FPURegister fd, Register rj);

  // Convert unsigned long to float.
  void Ffint_s_ul(FPURegister fd, FPURegister fj);
  void Ffint_s_ul(FPURegister fd, Register rj);

  // Convert double to unsigned word.
  void Ftintrz_uw_d(FPURegister fd, FPURegister fj, FPURegister scratch);
  void Ftintrz_uw_d(Register rd, FPURegister fj, FPURegister scratch);

  // Convert single to unsigned word.
  void Ftintrz_uw_s(FPURegister fd, FPURegister fs, FPURegister scratch);
  void Ftintrz_uw_s(Register rd, FPURegister fs, FPURegister scratch);

  // Convert double to unsigned long.
  void Ftintrz_ul_d(FPURegister fd, FPURegister fj, FPURegister scratch,
                    Register result = no_reg);
  void Ftintrz_ul_d(Register rd, FPURegister fj, FPURegister scratch,
                    Register result = no_reg);

  // Convert single to unsigned long.
  void Ftintrz_ul_s(FPURegister fd, FPURegister fj, FPURegister scratch,
                    Register result = no_reg);
  void Ftintrz_ul_s(Register rd, FPURegister fj, FPURegister scratch,
                    Register result = no_reg);

  // Round double functions
  void Trunc_d(FPURegister fd, FPURegister fj);
  void Round_d(FPURegister fd, FPURegister fj);
  void Floor_d(FPURegister fd, FPURegister fj);
  void Ceil_d(FPURegister fd, FPURegister fj);

  // Round float functions
  void Trunc_s(FPURegister fd, FPURegister fj);
  void Round_s(FPURegister fd, FPURegister fj);
  void Floor_s(FPURegister fd, FPURegister fj);
  void Ceil_s(FPURegister fd, FPURegister fj);

  // Jump the register contains a smi.
  void JumpIfSmi(Register value, Label* smi_label);

  void JumpIfEqual(Register a, int32_t b, Label* dest) {
    UseScratchRegisterScope temps(this);
    Register scratch = temps.Acquire();
    li(scratch, Operand(b));
    Branch(dest, eq, a, Operand(scratch));
  }

  void JumpIfLessThan(Register a, int32_t b, Label* dest) {
    UseScratchRegisterScope temps(this);
    Register scratch = temps.Acquire();
    li(scratch, Operand(b));
    Branch(dest, lt, a, Operand(scratch));
  }

  // Push a standard frame, consisting of ra, fp, context and JS function.
  void PushStandardFrame(Register function_reg);

  // Get the actual activation frame alignment for target environment.
  static int ActivationFrameAlignment();

  // Load Scaled Address instructions. Parameter sa (shift argument) must be
  // between [1, 31] (inclusive). The scratch register may be clobbered.
  void Alsl_w(Register rd, Register rj, Register rk, uint8_t sa,
              Register scratch = t7);
  void Alsl_d(Register rd, Register rj, Register rk, uint8_t sa,
              Register scratch = t7);

  // Compute the start of the generated instruction stream from the current PC.
  // This is an alternative to embedding the {CodeObject} handle as a reference.
  void ComputeCodeStartAddress(Register dst);

  // Control-flow integrity:

  // Define a function entrypoint. This doesn't emit any code for this
  // architecture, as control-flow integrity is not supported for it.
  void CodeEntry() {}
  // Define an exception handler.
  void ExceptionHandler() {}
  // Define an exception handler and bind a label.
  void BindExceptionHandler(Label* label) { bind(label); }

 protected:
  inline Register GetRkAsRegisterHelper(const Operand& rk, Register scratch);
  inline int32_t GetOffset(Label* L, OffsetSize bits);

 private:
  bool has_double_zero_reg_set_ = false;

  // Performs a truncating conversion of a floating point number as used by
  // the JS bitwise operations. See ECMA-262 9.5: ToInt32. Goes to 'done' if it
  // succeeds, otherwise falls through if result is saturated. On return
  // 'result' either holds answer, or is clobbered on fall through.
  void TryInlineTruncateDoubleToI(Register result, DoubleRegister input,
                                  Label* done);

  bool BranchShortOrFallback(Label* L, Condition cond, Register rj,
                             const Operand& rk, bool need_link);

  // f32 or f64
  void CompareF(FPURegister cmp1, FPURegister cmp2, FPUCondition cc,
                CFRegister cd, bool f32 = true);

  void CompareIsNanF(FPURegister cmp1, FPURegister cmp2, CFRegister cd,
                     bool f32 = true);

  void CallCFunctionHelper(Register function, int num_reg_arguments,
                           int num_double_arguments);

  void RoundDouble(FPURegister dst, FPURegister src, FPURoundingMode mode);

  void RoundFloat(FPURegister dst, FPURegister src, FPURoundingMode mode);

  // Push a fixed frame, consisting of ra, fp.
  void PushCommonFrame(Register marker_reg = no_reg);
};

// MacroAssembler implements a collection of frequently used macros.
class V8_EXPORT_PRIVATE MacroAssembler : public TurboAssembler {
 public:
  using TurboAssembler::TurboAssembler;

  // It assumes that the arguments are located below the stack pointer.
  // argc is the number of arguments not including the receiver.
  // TODO(LOONG_dev): LOONG64: Remove this function once we stick with the
  // reversed arguments order.
  void LoadReceiver(Register dest, Register argc) {
    Ld_d(dest, MemOperand(sp, 0));
  }

  void StoreReceiver(Register rec, Register argc, Register scratch) {
    St_d(rec, MemOperand(sp, 0));
  }

  bool IsNear(Label* L, Condition cond, int rs_reg);

  // Swap two registers.  If the scratch register is omitted then a slightly
  // less efficient form using xor instead of mov is emitted.
  void Swap(Register reg1, Register reg2, Register scratch = no_reg);

  void PushRoot(RootIndex index) {
    UseScratchRegisterScope temps(this);
    Register scratch = temps.Acquire();
    LoadRoot(scratch, index);
    Push(scratch);
  }

  // Compare the object in a register to a value and jump if they are equal.
  void JumpIfRoot(Register with, RootIndex index, Label* if_equal) {
    UseScratchRegisterScope temps(this);
    Register scratch = temps.Acquire();
    LoadRoot(scratch, index);
    Branch(if_equal, eq, with, Operand(scratch));
  }

  // Compare the object in a register to a value and jump if they are not equal.
  void JumpIfNotRoot(Register with, RootIndex index, Label* if_not_equal) {
    UseScratchRegisterScope temps(this);
    Register scratch = temps.Acquire();
    LoadRoot(scratch, index);
    Branch(if_not_equal, ne, with, Operand(scratch));
  }

  // Checks if value is in range [lower_limit, higher_limit] using a single
  // comparison.
  void JumpIfIsInRange(Register value, unsigned lower_limit,
                       unsigned higher_limit, Label* on_in_range);

  // ---------------------------------------------------------------------------
  // GC Support

  // Notify the garbage collector that we wrote a pointer into an object.
  // |object| is the object being stored into, |value| is the object being
  // stored.
  // The offset is the offset from the start of the object, not the offset from
  // the tagged HeapObject pointer. For use with FieldOperand(reg, off).
  void RecordWriteField(
      Register object, int offset, Register value, RAStatus ra_status,
      SaveFPRegsMode save_fp,
      RememberedSetAction remembered_set_action = RememberedSetAction::kEmit,
      SmiCheck smi_check = SmiCheck::kInline);

  // For a given |object| notify the garbage collector that the slot at |offset|
  // has been written.  |value| is the object being stored.
  void RecordWrite(
      Register object, Operand offset, Register value, RAStatus ra_status,
      SaveFPRegsMode save_fp,
      RememberedSetAction remembered_set_action = RememberedSetAction::kEmit,
      SmiCheck smi_check = SmiCheck::kInline);

  // ---------------------------------------------------------------------------
  // Pseudo-instructions.

  // Convert double to unsigned long.
  void Ftintrz_l_ud(FPURegister fd, FPURegister fj, FPURegister scratch);

  void Ftintrz_l_d(FPURegister fd, FPURegister fj);
  void Ftintrne_l_d(FPURegister fd, FPURegister fj);
  void Ftintrm_l_d(FPURegister fd, FPURegister fj);
  void Ftintrp_l_d(FPURegister fd, FPURegister fj);

  void Ftintrz_w_d(FPURegister fd, FPURegister fj);
  void Ftintrne_w_d(FPURegister fd, FPURegister fj);
  void Ftintrm_w_d(FPURegister fd, FPURegister fj);
  void Ftintrp_w_d(FPURegister fd, FPURegister fj);

  void Madd_s(FPURegister fd, FPURegister fa, FPURegister fj, FPURegister fk);
  void Madd_d(FPURegister fd, FPURegister fa, FPURegister fj, FPURegister fk);
  void Msub_s(FPURegister fd, FPURegister fa, FPURegister fj, FPURegister fk);
  void Msub_d(FPURegister fd, FPURegister fa, FPURegister fj, FPURegister fk);

  // Enter exit frame.
  // argc - argument count to be dropped by LeaveExitFrame.
  // save_doubles - saves FPU registers on stack, currently disabled.
  // stack_space - extra stack space.
  void EnterExitFrame(bool save_doubles, int stack_space = 0,
                      StackFrame::Type frame_type = StackFrame::EXIT);

  // Leave the current exit frame.
  void LeaveExitFrame(bool save_doubles, Register arg_count,
                      bool do_return = NO_EMIT_RETURN,
                      bool argument_count_is_length = false);

  // Make sure the stack is aligned. Only emits code in debug mode.
  void AssertStackIsAligned();

  // Load the global proxy from the current context.
  void LoadGlobalProxy(Register dst) {
    LoadNativeContextSlot(dst, Context::GLOBAL_PROXY_INDEX);
  }

  void LoadNativeContextSlot(Register dst, int index);

  // Load the initial map from the global function. The registers
  // function and map can be the same, function is then overwritten.
  void LoadGlobalFunctionInitialMap(Register function, Register map,
                                    Register scratch);

  // -------------------------------------------------------------------------
  // JavaScript invokes.

  // Invoke the JavaScript function code by either calling or jumping.
  void InvokeFunctionCode(Register function, Register new_target,
                          Register expected_parameter_count,
                          Register actual_parameter_count, InvokeType type);

  // On function call, call into the debugger.
  void CallDebugOnFunctionCall(Register fun, Register new_target,
                               Register expected_parameter_count,
                               Register actual_parameter_count);

  // Invoke the JavaScript function in the given register. Changes the
  // current context to the context in the function before invoking.
  void InvokeFunctionWithNewTarget(Register function, Register new_target,
                                   Register actual_parameter_count,
                                   InvokeType type);
  void InvokeFunction(Register function, Register expected_parameter_count,
                      Register actual_parameter_count, InvokeType type);

  // Exception handling.

  // Push a new stack handler and link into stack handler chain.
  void PushStackHandler();

  // Unlink the stack handler on top of the stack from the stack handler chain.
  // Must preserve the result register.
  void PopStackHandler();

  // -------------------------------------------------------------------------
  // Support functions.

  void GetObjectType(Register function, Register map, Register type_reg);

  void GetInstanceTypeRange(Register map, Register type_reg,
                            InstanceType lower_limit, Register range);

  // -------------------------------------------------------------------------
  // Runtime calls.

  // Call a runtime routine.
  void CallRuntime(const Runtime::Function* f, int num_arguments,
                   SaveFPRegsMode save_doubles = SaveFPRegsMode::kIgnore);

  // Convenience function: Same as above, but takes the fid instead.
  void CallRuntime(Runtime::FunctionId fid,
                   SaveFPRegsMode save_doubles = SaveFPRegsMode::kIgnore) {
    const Runtime::Function* function = Runtime::FunctionForId(fid);
    CallRuntime(function, function->nargs, save_doubles);
  }

  // Convenience function: Same as above, but takes the fid instead.
  void CallRuntime(Runtime::FunctionId fid, int num_arguments,
                   SaveFPRegsMode save_doubles = SaveFPRegsMode::kIgnore) {
    CallRuntime(Runtime::FunctionForId(fid), num_arguments, save_doubles);
  }

  // Convenience function: tail call a runtime routine (jump).
  void TailCallRuntime(Runtime::FunctionId fid);

  // Jump to the builtin routine.
  void JumpToExternalReference(const ExternalReference& builtin,
                               bool builtin_exit_frame = false);

  // Generates a trampoline to jump to the off-heap instruction stream.
  void JumpToInstructionStream(Address entry);

  // ---------------------------------------------------------------------------
  // In-place weak references.
  void LoadWeakValue(Register out, Register in, Label* target_if_cleared);

  // -------------------------------------------------------------------------
  // StatsCounter support.

  void IncrementCounter(StatsCounter* counter, int value, Register scratch1,
                        Register scratch2) {
    if (!FLAG_native_code_counters) return;
    EmitIncrementCounter(counter, value, scratch1, scratch2);
  }
  void EmitIncrementCounter(StatsCounter* counter, int value, Register scratch1,
                            Register scratch2);
  void DecrementCounter(StatsCounter* counter, int value, Register scratch1,
                        Register scratch2) {
    if (!FLAG_native_code_counters) return;
    EmitDecrementCounter(counter, value, scratch1, scratch2);
  }
  void EmitDecrementCounter(StatsCounter* counter, int value, Register scratch1,
                            Register scratch2);

  // -------------------------------------------------------------------------
  // Stack limit utilities

  enum StackLimitKind { kInterruptStackLimit, kRealStackLimit };
  void LoadStackLimit(Register destination, StackLimitKind kind);
  void StackOverflowCheck(Register num_args, Register scratch1,
                          Register scratch2, Label* stack_overflow);

  // ---------------------------------------------------------------------------
  // Smi utilities.

  void SmiTag(Register dst, Register src) {
    STATIC_ASSERT(kSmiTag == 0);
    if (SmiValuesAre32Bits()) {
      slli_d(dst, src, 32);
    } else {
      DCHECK(SmiValuesAre31Bits());
      add_w(dst, src, src);
    }
  }

  void SmiTag(Register reg) { SmiTag(reg, reg); }

  // Left-shifted from int32 equivalent of Smi.
  void SmiScale(Register dst, Register src, int scale) {
    if (SmiValuesAre32Bits()) {
      // The int portion is upper 32-bits of 64-bit word.
      srai_d(dst, src, kSmiShift - scale);
    } else {
      DCHECK(SmiValuesAre31Bits());
      DCHECK_GE(scale, kSmiTagSize);
      slli_w(dst, src, scale - kSmiTagSize);
    }
  }

  // Test if the register contains a smi.
  inline void SmiTst(Register value, Register scratch) {
    And(scratch, value, Operand(kSmiTagMask));
  }

  // Jump if the register contains a non-smi.
  void JumpIfNotSmi(Register value, Label* not_smi_label);

  // Abort execution if argument is a smi, enabled via --debug-code.
  void AssertNotSmi(Register object);
  void AssertSmi(Register object);

  // Abort execution if argument is not a Constructor, enabled via --debug-code.
  void AssertConstructor(Register object);

  // Abort execution if argument is not a JSFunction, enabled via --debug-code.
  void AssertFunction(Register object);

  // Abort execution if argument is not a JSBoundFunction,
  // enabled via --debug-code.
  void AssertBoundFunction(Register object);

  // Abort execution if argument is not a JSGeneratorObject (or subclass),
  // enabled via --debug-code.
  void AssertGeneratorObject(Register object);

  // Abort execution if argument is not undefined or an AllocationSite, enabled
  // via --debug-code.
  void AssertUndefinedOrAllocationSite(Register object, Register scratch);

  template <typename Field>
  void DecodeField(Register dst, Register src) {
    Bstrpick_d(dst, src, Field::kShift + Field::kSize - 1, Field::kShift);
  }

  template <typename Field>
  void DecodeField(Register reg) {
    DecodeField<Field>(reg, reg);
  }

 private:
  // Helper functions for generating invokes.
  void InvokePrologue(Register expected_parameter_count,
                      Register actual_parameter_count, Label* done,
                      InvokeType type);

  friend class CommonFrame;

  DISALLOW_IMPLICIT_CONSTRUCTORS(MacroAssembler);
};

template <typename Func>
void TurboAssembler::GenerateSwitchTable(Register index, size_t case_count,
                                         Func GetLabelFunction) {
  // Ensure that dd-ed labels following this instruction use 8 bytes aligned
  // addresses.
  BlockTrampolinePoolFor(static_cast<int>(case_count) * 2 +
                         kSwitchTablePrologueSize);
  UseScratchRegisterScope temps(this);
  Register scratch = temps.Acquire();
  Align(8);  // next is 4 instrs.
  pcaddi(scratch, 4);
  // alsl_d will do sa
  alsl_d(scratch, index, scratch, kPointerSizeLog2);
  Ld_d(scratch, MemOperand(scratch, 0));
  jirl(zero_reg, scratch, 0);
  for (size_t index = 0; index < case_count; ++index) {
    dd(GetLabelFunction(index));
  }
}

#define ACCESS_MASM(masm) masm->

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_LOONG64_MACRO_ASSEMBLER_LOONG64_H_
