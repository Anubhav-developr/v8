// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_BACKEND_ARM64_INSTRUCTION_CODES_ARM64_H_
#define V8_COMPILER_BACKEND_ARM64_INSTRUCTION_CODES_ARM64_H_

namespace v8 {
namespace internal {
namespace compiler {

// ARM64-specific opcodes that specify which assembly sequence to emit.
// Most opcodes specify a single instruction.
#define TARGET_ARCH_OPCODE_LIST(V)    \
  V(Arm64Add)                         \
  V(Arm64Add32)                       \
  V(Arm64And)                         \
  V(Arm64And32)                       \
  V(Arm64Bic)                         \
  V(Arm64Bic32)                       \
  V(Arm64Clz)                         \
  V(Arm64Clz32)                       \
  V(Arm64Cmp)                         \
  V(Arm64Cmp32)                       \
  V(Arm64Cmn)                         \
  V(Arm64Cmn32)                       \
  V(Arm64Cnt)                         \
  V(Arm64Cnt32)                       \
  V(Arm64Cnt64)                       \
  V(Arm64Tst)                         \
  V(Arm64Tst32)                       \
  V(Arm64Or)                          \
  V(Arm64Or32)                        \
  V(Arm64Orn)                         \
  V(Arm64Orn32)                       \
  V(Arm64Eor)                         \
  V(Arm64Eor32)                       \
  V(Arm64Eon)                         \
  V(Arm64Eon32)                       \
  V(Arm64Sadalp)                      \
  V(Arm64Saddlp)                      \
  V(Arm64Sub)                         \
  V(Arm64Sub32)                       \
  V(Arm64Mul)                         \
  V(Arm64Mul32)                       \
  V(Arm64Smull)                       \
  V(Arm64Smull2)                      \
  V(Arm64Uadalp)                      \
  V(Arm64Uaddlp)                      \
  V(Arm64Umull)                       \
  V(Arm64Umull2)                      \
  V(Arm64Madd)                        \
  V(Arm64Madd32)                      \
  V(Arm64Msub)                        \
  V(Arm64Msub32)                      \
  V(Arm64Mneg)                        \
  V(Arm64Mneg32)                      \
  V(Arm64Idiv)                        \
  V(Arm64Idiv32)                      \
  V(Arm64Udiv)                        \
  V(Arm64Udiv32)                      \
  V(Arm64Imod)                        \
  V(Arm64Imod32)                      \
  V(Arm64Umod)                        \
  V(Arm64Umod32)                      \
  V(Arm64Not)                         \
  V(Arm64Not32)                       \
  V(Arm64Lsl)                         \
  V(Arm64Lsl32)                       \
  V(Arm64Lsr)                         \
  V(Arm64Lsr32)                       \
  V(Arm64Asr)                         \
  V(Arm64Asr32)                       \
  V(Arm64Ror)                         \
  V(Arm64Ror32)                       \
  V(Arm64Mov32)                       \
  V(Arm64Sxtb32)                      \
  V(Arm64Sxth32)                      \
  V(Arm64Sxtb)                        \
  V(Arm64Sxth)                        \
  V(Arm64Sxtw)                        \
  V(Arm64Sbfx)                        \
  V(Arm64Sbfx32)                      \
  V(Arm64Ubfx)                        \
  V(Arm64Ubfx32)                      \
  V(Arm64Ubfiz32)                     \
  V(Arm64Bfi)                         \
  V(Arm64Rbit)                        \
  V(Arm64Rbit32)                      \
  V(Arm64Rev)                         \
  V(Arm64Rev32)                       \
  V(Arm64TestAndBranch32)             \
  V(Arm64TestAndBranch)               \
  V(Arm64CompareAndBranch32)          \
  V(Arm64CompareAndBranch)            \
  V(Arm64Claim)                       \
  V(Arm64Poke)                        \
  V(Arm64PokePair)                    \
  V(Arm64Peek)                        \
  V(Arm64Float32Cmp)                  \
  V(Arm64Float32Add)                  \
  V(Arm64Float32Sub)                  \
  V(Arm64Float32Mul)                  \
  V(Arm64Float32Div)                  \
  V(Arm64Float32Abs)                  \
  V(Arm64Float32Abd)                  \
  V(Arm64Float32Neg)                  \
  V(Arm64Float32Sqrt)                 \
  V(Arm64Float32Fnmul)                \
  V(Arm64Float32RoundDown)            \
  V(Arm64Float32Max)                  \
  V(Arm64Float32Min)                  \
  V(Arm64Float64Cmp)                  \
  V(Arm64Float64Add)                  \
  V(Arm64Float64Sub)                  \
  V(Arm64Float64Mul)                  \
  V(Arm64Float64Div)                  \
  V(Arm64Float64Mod)                  \
  V(Arm64Float64Max)                  \
  V(Arm64Float64Min)                  \
  V(Arm64Float64Abs)                  \
  V(Arm64Float64Abd)                  \
  V(Arm64Float64Neg)                  \
  V(Arm64Float64Sqrt)                 \
  V(Arm64Float64Fnmul)                \
  V(Arm64Float64RoundDown)            \
  V(Arm64Float32RoundUp)              \
  V(Arm64Float64RoundUp)              \
  V(Arm64Float64RoundTiesAway)        \
  V(Arm64Float32RoundTruncate)        \
  V(Arm64Float64RoundTruncate)        \
  V(Arm64Float32RoundTiesEven)        \
  V(Arm64Float64RoundTiesEven)        \
  V(Arm64Float64SilenceNaN)           \
  V(Arm64Float32ToFloat64)            \
  V(Arm64Float64ToFloat32)            \
  V(Arm64Float32ToInt32)              \
  V(Arm64Float64ToInt32)              \
  V(Arm64Float32ToUint32)             \
  V(Arm64Float64ToUint32)             \
  V(Arm64Float32ToInt64)              \
  V(Arm64Float64ToInt64)              \
  V(Arm64Float32ToUint64)             \
  V(Arm64Float64ToUint64)             \
  V(Arm64Int32ToFloat32)              \
  V(Arm64Int32ToFloat64)              \
  V(Arm64Int64ToFloat32)              \
  V(Arm64Int64ToFloat64)              \
  V(Arm64Uint32ToFloat32)             \
  V(Arm64Uint32ToFloat64)             \
  V(Arm64Uint64ToFloat32)             \
  V(Arm64Uint64ToFloat64)             \
  V(Arm64Float64ExtractLowWord32)     \
  V(Arm64Float64ExtractHighWord32)    \
  V(Arm64Float64InsertLowWord32)      \
  V(Arm64Float64InsertHighWord32)     \
  V(Arm64Float64MoveU64)              \
  V(Arm64U64MoveFloat64)              \
  V(Arm64LdrS)                        \
  V(Arm64StrS)                        \
  V(Arm64LdrD)                        \
  V(Arm64StrD)                        \
  V(Arm64LdrQ)                        \
  V(Arm64StrQ)                        \
  V(Arm64Ldrb)                        \
  V(Arm64Ldrsb)                       \
  V(Arm64LdrsbW)                      \
  V(Arm64Strb)                        \
  V(Arm64Ldrh)                        \
  V(Arm64Ldrsh)                       \
  V(Arm64LdrshW)                      \
  V(Arm64Strh)                        \
  V(Arm64Ldrsw)                       \
  V(Arm64LdrW)                        \
  V(Arm64StrW)                        \
  V(Arm64Ldr)                         \
  V(Arm64LdrDecompressTaggedSigned)   \
  V(Arm64LdrDecompressTaggedPointer)  \
  V(Arm64LdrDecompressAnyTagged)      \
  V(Arm64LdarDecompressTaggedSigned)  \
  V(Arm64LdarDecompressTaggedPointer) \
  V(Arm64LdarDecompressAnyTagged)     \
  V(Arm64Str)                         \
  V(Arm64StrCompressTagged)           \
  V(Arm64StlrCompressTagged)          \
  V(Arm64DmbIsh)                      \
  V(Arm64DsbIsb)                      \
  V(Arm64Sxtl)                        \
  V(Arm64Sxtl2)                       \
  V(Arm64Uxtl)                        \
  V(Arm64Uxtl2)                       \
  V(Arm64FSplat)                      \
  V(Arm64FAbs)                        \
  V(Arm64FSqrt)                       \
  V(Arm64FNeg)                        \
  V(Arm64FExtractLane)                \
  V(Arm64FReplaceLane)                \
  V(Arm64FAdd)                        \
  V(Arm64FSub)                        \
  V(Arm64FMul)                        \
  V(Arm64FMulElement)                 \
  V(Arm64FDiv)                        \
  V(Arm64FMin)                        \
  V(Arm64FMax)                        \
  V(Arm64FEq)                         \
  V(Arm64FNe)                         \
  V(Arm64FLt)                         \
  V(Arm64FLe)                         \
  V(Arm64F64x2Qfma)                   \
  V(Arm64F64x2Qfms)                   \
  V(Arm64F64x2Pmin)                   \
  V(Arm64F64x2Pmax)                   \
  V(Arm64F64x2ConvertLowI32x4S)       \
  V(Arm64F64x2ConvertLowI32x4U)       \
  V(Arm64F64x2PromoteLowF32x4)        \
  V(Arm64F32x4SConvertI32x4)          \
  V(Arm64F32x4UConvertI32x4)          \
  V(Arm64F32x4RecipApprox)            \
  V(Arm64F32x4RecipSqrtApprox)        \
  V(Arm64F32x4Qfma)                   \
  V(Arm64F32x4Qfms)                   \
  V(Arm64F32x4Pmin)                   \
  V(Arm64F32x4Pmax)                   \
  V(Arm64F32x4DemoteF64x2Zero)        \
  V(Arm64ISplat)                      \
  V(Arm64IAbs)                        \
  V(Arm64INeg)                        \
  V(Arm64IExtractLane)                \
  V(Arm64IReplaceLane)                \
  V(Arm64I64x2Shl)                    \
  V(Arm64I64x2ShrS)                   \
  V(Arm64IAdd)                        \
  V(Arm64ISub)                        \
  V(Arm64I64x2Mul)                    \
  V(Arm64IEq)                         \
  V(Arm64INe)                         \
  V(Arm64IGtS)                        \
  V(Arm64IGeS)                        \
  V(Arm64I64x2ShrU)                   \
  V(Arm64I64x2BitMask)                \
  V(Arm64I32x4SConvertF32x4)          \
  V(Arm64I32x4Shl)                    \
  V(Arm64I32x4ShrS)                   \
  V(Arm64I32x4Mul)                    \
  V(Arm64Mla)                         \
  V(Arm64Mls)                         \
  V(Arm64IMinS)                       \
  V(Arm64IMaxS)                       \
  V(Arm64I32x4UConvertF32x4)          \
  V(Arm64I32x4ShrU)                   \
  V(Arm64IMinU)                       \
  V(Arm64IMaxU)                       \
  V(Arm64IGtU)                        \
  V(Arm64IGeU)                        \
  V(Arm64I32x4BitMask)                \
  V(Arm64I32x4DotI16x8S)              \
  V(Arm64I32x4TruncSatF64x2SZero)     \
  V(Arm64I32x4TruncSatF64x2UZero)     \
  V(Arm64IExtractLaneU)               \
  V(Arm64IExtractLaneS)               \
  V(Arm64I16x8Shl)                    \
  V(Arm64I16x8ShrS)                   \
  V(Arm64I16x8SConvertI32x4)          \
  V(Arm64IAddSatS)                    \
  V(Arm64ISubSatS)                    \
  V(Arm64I16x8Mul)                    \
  V(Arm64I16x8ShrU)                   \
  V(Arm64I16x8UConvertI32x4)          \
  V(Arm64IAddSatU)                    \
  V(Arm64ISubSatU)                    \
  V(Arm64RoundingAverageU)            \
  V(Arm64I16x8Q15MulRSatS)            \
  V(Arm64I16x8BitMask)                \
  V(Arm64I8x16Shl)                    \
  V(Arm64I8x16ShrS)                   \
  V(Arm64I8x16SConvertI16x8)          \
  V(Arm64I8x16ShrU)                   \
  V(Arm64I8x16UConvertI16x8)          \
  V(Arm64I8x16BitMask)                \
  V(Arm64S128Const)                   \
  V(Arm64S128Zero)                    \
  V(Arm64S128Dup)                     \
  V(Arm64S128And)                     \
  V(Arm64S128Or)                      \
  V(Arm64S128Xor)                     \
  V(Arm64S128Not)                     \
  V(Arm64S128Select)                  \
  V(Arm64S128AndNot)                  \
  V(Arm64Ssra)                        \
  V(Arm64Usra)                        \
  V(Arm64S32x4ZipLeft)                \
  V(Arm64S32x4ZipRight)               \
  V(Arm64S32x4UnzipLeft)              \
  V(Arm64S32x4UnzipRight)             \
  V(Arm64S32x4TransposeLeft)          \
  V(Arm64S32x4TransposeRight)         \
  V(Arm64S32x4Shuffle)                \
  V(Arm64S16x8ZipLeft)                \
  V(Arm64S16x8ZipRight)               \
  V(Arm64S16x8UnzipLeft)              \
  V(Arm64S16x8UnzipRight)             \
  V(Arm64S16x8TransposeLeft)          \
  V(Arm64S16x8TransposeRight)         \
  V(Arm64S8x16ZipLeft)                \
  V(Arm64S8x16ZipRight)               \
  V(Arm64S8x16UnzipLeft)              \
  V(Arm64S8x16UnzipRight)             \
  V(Arm64S8x16TransposeLeft)          \
  V(Arm64S8x16TransposeRight)         \
  V(Arm64S8x16Concat)                 \
  V(Arm64I8x16Swizzle)                \
  V(Arm64I8x16Shuffle)                \
  V(Arm64S32x2Reverse)                \
  V(Arm64S16x4Reverse)                \
  V(Arm64S16x2Reverse)                \
  V(Arm64S8x8Reverse)                 \
  V(Arm64S8x4Reverse)                 \
  V(Arm64S8x2Reverse)                 \
  V(Arm64V128AnyTrue)                 \
  V(Arm64I64x2AllTrue)                \
  V(Arm64I32x4AllTrue)                \
  V(Arm64I16x8AllTrue)                \
  V(Arm64I8x16AllTrue)                \
  V(Arm64LoadSplat)                   \
  V(Arm64LoadLane)                    \
  V(Arm64StoreLane)                   \
  V(Arm64S128Load8x8S)                \
  V(Arm64S128Load8x8U)                \
  V(Arm64S128Load16x4S)               \
  V(Arm64S128Load16x4U)               \
  V(Arm64S128Load32x2S)               \
  V(Arm64S128Load32x2U)               \
  V(Arm64Word64AtomicLoadUint64)      \
  V(Arm64Word64AtomicStoreWord64)     \
  V(Arm64Word64AtomicAddUint64)       \
  V(Arm64Word64AtomicSubUint64)       \
  V(Arm64Word64AtomicAndUint64)       \
  V(Arm64Word64AtomicOrUint64)        \
  V(Arm64Word64AtomicXorUint64)       \
  V(Arm64Word64AtomicExchangeUint64)  \
  V(Arm64Word64AtomicCompareExchangeUint64)

// Addressing modes represent the "shape" of inputs to an instruction.
// Many instructions support multiple addressing modes. Addressing modes
// are encoded into the InstructionCode of the instruction and tell the
// code generator after register allocation which assembler method to call.
//
// We use the following local notation for addressing modes:
//
// R = register
// O = register or stack slot
// D = double register
// I = immediate (handle, external, int32)
// MRI = [register + immediate]
// MRR = [register + register]
#define TARGET_ADDRESSING_MODE_LIST(V)                          \
  V(MRI)              /* [%r0 + K] */                           \
  V(MRR)              /* [%r0 + %r1] */                         \
  V(Operand2_R_LSL_I) /* %r0 LSL K */                           \
  V(Operand2_R_LSR_I) /* %r0 LSR K */                           \
  V(Operand2_R_ASR_I) /* %r0 ASR K */                           \
  V(Operand2_R_ROR_I) /* %r0 ROR K */                           \
  V(Operand2_R_UXTB)  /* %r0 UXTB (unsigned extend byte) */     \
  V(Operand2_R_UXTH)  /* %r0 UXTH (unsigned extend halfword) */ \
  V(Operand2_R_SXTB)  /* %r0 SXTB (signed extend byte) */       \
  V(Operand2_R_SXTH)  /* %r0 SXTH (signed extend halfword) */   \
  V(Operand2_R_SXTW)  /* %r0 SXTW (signed extend word) */       \
  V(Root)             /* [%rr + K] */

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_BACKEND_ARM64_INSTRUCTION_CODES_ARM64_H_
