#include "PseudoCppPrinter_InstPrint.h"

#include "CpuGpr.h"
#include "CpuInstruction.h"
#include "CpuOpcode.h"
#include "PseudoCppPrinter.h"
#include <cstdlib>

using namespace PseudoCppPrinter;

//------------------------------------------------------------------------------------------------------------------------------------------
// Print in either hex or decimal, depending on how big the number is.
// The number is printed without padding.
//------------------------------------------------------------------------------------------------------------------------------------------
static void printHexOrDecInt32Literal(const int32_t val, std::ostream& out) {
    if (std::abs(val) < 10) {
        out << val;
    } else {
        printHexCppInt32Literal(val, false, out);
    }
}

static void printHexOrDecUint32Literal(const uint32_t val, std::ostream& out) {
    if (val < 10) {
        out << val;
    } else {
        printHexCppUint32Literal(val, false, out);
    }
}

static const char* getGprMacroNameOr0(const uint8_t gprIdx) {
    if (gprIdx == CpuGpr::ZERO) {
        return "0";
    } else {
        return getGprCppMacroName(gprIdx);
    }
}

void PseudoCppPrinter::printInst_ramToCpuLoad(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regT);
    out << " = ";
    out << CpuOpcodeUtils::getMnemonic(inst.opcode);
    out << "(";

    if (inst.opcode == CpuOpcode::LWL || inst.opcode == CpuOpcode::LWR) {
        out << getGprMacroNameOr0(inst.regT);
        out << ", ";
    }

    out << getGprMacroNameOr0(inst.regS);

    const int16_t i16 = (int16_t) inst.immediateVal;
    const int32_t i32 = i16;

    if (i32 != 0) {
        if (i32 < 0) {
            out << " - ";
            printHexCppInt32Literal(-i32, false, out);
        } else {
            out << " + ";
            printHexCppInt32Literal(i32, false, out);
        }
    }

    out << ")";
}

void PseudoCppPrinter::printInst_cpuToRamStore(std::ostream& out, const CpuInstruction& inst) {
    out << CpuOpcodeUtils::getMnemonic(inst.opcode);
    out << "(";
    out << getGprMacroNameOr0(inst.regT);
    out << ", ";
    out << getGprMacroNameOr0(inst.regS);

    const int16_t i16 = (int16_t) inst.immediateVal;
    const int32_t i32 = i16;

    if (i32 != 0) {
        if (i32 < 0) {
            out << " - ";
            printHexCppInt32Literal(-i32, false, out);
        } else {
            out << " + ";
            printHexCppInt32Literal(i32, false, out);
        }
    }

    out << ")";
}

void PseudoCppPrinter::printInst_loadOrStoreWordCop2(std::ostream& out, const CpuInstruction& inst) {
    out << CpuOpcodeUtils::getMnemonic(inst.opcode);
    out << "(";
    out << (uint32_t) inst.regT;
    out << ", ";
    out << getGprMacroNameOr0(inst.regS);

    const int16_t i16 = (int16_t) inst.immediateVal;
    const int32_t i32 = i16;

    if (i32 != 0) {
        if (i32 < 0) {
            out << " - ";
            printHexCppInt32Literal(-i32, false, out);
        } else {
            out << " + ";
            printHexCppInt32Literal(i32, false, out);
        }
    }

    out << ")";
}

void PseudoCppPrinter::printInst_addiu(std::ostream& out, const CpuInstruction& inst) {
    const int16_t i16 = (int16_t) inst.immediateVal;
    const int32_t i32 = i16;

    out << getGprCppMacroName(inst.regT);

    if (i32 == 0) {
        // If the immediate is zero, then it is basically a move or assign
        out << " = ";
        out << getGprMacroNameOr0(inst.regS);
    }
    else if (inst.regS == CpuGpr::ZERO) {
        // If the register is zero then we are just assigning an integer literal
        out << " = ";
        printHexOrDecInt32Literal(i32, out);
    }
    else if (inst.regT == inst.regS) {
        // If the source and dest reg are the same then we can use one of '+=', '-=', '++' or '--':
        if (i32 < 0) {
            if (i32 == -1) {
                out << "--";
            } else {
                out << " -= ";
                printHexOrDecInt32Literal(-i32, out);
            }
        } else {
            if (i32 == 1) {
                out << "++";
            } else {
                out << " += ";
                printHexOrDecInt32Literal(i32, out);
            }
        }
    } else {
        // Regular add or subtract
        if (i32 < 0) {
            out << " = ";
            out << getGprCppMacroName(inst.regS);
            out << " - ";
            printHexOrDecInt32Literal(-i32, out);
        } else {
            out << " = ";
            out << getGprCppMacroName(inst.regS);
            out << " + ";
            printHexOrDecInt32Literal(i32, out);
        }
    }
}

void PseudoCppPrinter::printInst_addu(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regS == CpuGpr::ZERO && inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (inst.regS == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regS);
    }
    else if (inst.regS == inst.regD) {
        // One source reg same as dest: can use '+=' shorthand
        out << " += ";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == inst.regD) {
        // One source reg same as dest: can use '+=' shorthand
        out << " += ";
        out << getGprCppMacroName(inst.regS);
    }
    else {
        // Regular add
        out << " = ";
        out << getGprCppMacroName(inst.regS);
        out << " + ";
        out << getGprCppMacroName(inst.regT);
    }
}

void PseudoCppPrinter::printInst_and(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regS == CpuGpr::ZERO || inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (inst.regS == inst.regD) {
        // One source reg same as dest: can use '&=' shorthand
        out << " &= ";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == inst.regD) {
        // One source reg same as dest: can use '&=' shorthand
        out << " &= ";
        out << getGprCppMacroName(inst.regS);
    }
    else {
        // Regular bitwise AND
        out << " = ";
        out << getGprCppMacroName(inst.regS);
        out << " & ";
        out << getGprCppMacroName(inst.regT);
    }
}

void PseudoCppPrinter::printInst_andi(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regT);
    const uint16_t i = (uint16_t) inst.immediateVal;

    if (inst.regS == CpuGpr::ZERO || i == 0) {
        // Zero assign
        out << " = 0";
    }
    else if (inst.regS == inst.regT) {
        // One source reg same as dest: can use '&=' shorthand
        out << " &= ";
        printHexOrDecUint32Literal(i , out);
    } 
    else {
        // Regular bitwise AND
        out << " = ";
        out << getGprCppMacroName(inst.regS);
        out << " & ";
        printHexOrDecUint32Literal(i , out);
    }
}

void PseudoCppPrinter::printInst_beq(std::ostream& out, const CpuInstruction& inst) {
    if (inst.regS == CpuGpr::ZERO && inst.regT == CpuGpr::ZERO) {
        // Always branch
        out << "(true)";
    } 
    else if (inst.regS == CpuGpr::ZERO) {
        // Branch if zero
        out << "(";
        out << getGprCppMacroName(inst.regT);
        out << " == 0)";
    }
    else if (inst.regT == CpuGpr::ZERO) {
        // Branch if zero
        out << "(";
        out << getGprCppMacroName(inst.regS);
        out << " == 0)";
    }
    else {
        // Regular comparison
        out << "(";
        out << getGprCppMacroName(inst.regS);
        out << " == ";
        out << getGprCppMacroName(inst.regT);
        out << ")";
    }
}

void PseudoCppPrinter::printInst_bgez(std::ostream& out, const CpuInstruction& inst) {
    if (inst.regS == CpuGpr::ZERO) {
        out << "(true)";
    } else {
        out << "(i32(";
        out << getGprMacroNameOr0(inst.regS);
        out << ") >= 0)";
    }
}

void PseudoCppPrinter::printInst_bgtz(std::ostream& out, const CpuInstruction& inst) {
    if (inst.regS == CpuGpr::ZERO) {
        out << "(false)";
    } else {
        out << "(i32(";
        out << getGprMacroNameOr0(inst.regS);
        out << ") > 0)";
    }
}

void PseudoCppPrinter::printInst_blez(std::ostream& out, const CpuInstruction& inst) {
    if (inst.regS == CpuGpr::ZERO) {
        out << "(true)";
    } else {
        out << "(i32(";
        out << getGprMacroNameOr0(inst.regS);
        out << ") <= 0)";
    }
}

void PseudoCppPrinter::printInst_bltz(std::ostream& out, const CpuInstruction& inst) {
    if (inst.regS == CpuGpr::ZERO) {
        out << "(false)";
    } else {
        out << "(i32(";
        out << getGprMacroNameOr0(inst.regS);
        out << ") < 0)";
    }
}

void PseudoCppPrinter::printInst_bne(std::ostream& out, const CpuInstruction& inst) {
    if (inst.regS == CpuGpr::ZERO && inst.regT == CpuGpr::ZERO) {
        // Never branch
        out << "(false)";
    } 
    else if (inst.regS == CpuGpr::ZERO) {
        // Branch if not zero
        out << "(";
        out << getGprCppMacroName(inst.regT);
        out << " != 0)";
    }
    else if (inst.regT == CpuGpr::ZERO) {
        // Branch if not zero
        out << "(";
        out << getGprCppMacroName(inst.regS);
        out << " != 0)";
    }
    else {
        // Regular comparison
        out << "(";
        out << getGprCppMacroName(inst.regS);
        out << " != ";
        out << getGprCppMacroName(inst.regT);
        out << ")";
    }
}

void PseudoCppPrinter::printInst_lui(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regT);
    out << " = ";
    printHexOrDecUint32Literal(inst.immediateVal << 16, out);
}

void PseudoCppPrinter::printInst_mfhi(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);
    out << " = hi";
}

void PseudoCppPrinter::printInst_mflo(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);
    out << " = lo";
}

void PseudoCppPrinter::printInst_nor(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regS == CpuGpr::ZERO && inst.regT == CpuGpr::ZERO) {
        // All one bits
        out << " = ";
        printHexCppUint32Literal(0xFFFFFFFFu, false, out);
    }
    else if (inst.regS == CpuGpr::ZERO) {
        // Bitwise NOT of register
        out << " = ~";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == CpuGpr::ZERO) {
        // Bitwise NOT of register
        out << " = ~";
        out << getGprCppMacroName(inst.regS);
    }
    else {
        // Regular case
        out << " = ~(";
        out << getGprCppMacroName(inst.regS);
        out << " | ";
        out << getGprCppMacroName(inst.regT);
        out << ")";
    }
}

void PseudoCppPrinter::printInst_or(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regS == CpuGpr::ZERO && inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (inst.regS == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regS);
    }
    else if (inst.regS == inst.regD) {
        // One source reg same as dest: can use '|=' shorthand
        out << " |= ";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == inst.regD) {
        // One source reg same as dest: can use '|=' shorthand
        out << " |= ";
        out << getGprCppMacroName(inst.regS);
    }
    else {
        // Regular bitwise OR
        out << " = ";
        out << getGprCppMacroName(inst.regS);
        out << " | ";
        out << getGprCppMacroName(inst.regT);
    }
}

void PseudoCppPrinter::printInst_ori(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regT);
    const uint16_t u16 = (uint16_t) inst.immediateVal;

    if (inst.regS == CpuGpr::ZERO) {
        // Assigning a constant
        out << " = ";
        printHexOrDecUint32Literal(u16, out);
    } 
    else if (u16 == 0) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regS);
    }
    else if (inst.regS == inst.regT) {
        // Can use '|=' shorthand
        out << " |= ";
        printHexOrDecUint32Literal(u16, out);
    }
    else {
        // Regular bitwise OR
        out << " = ";
        out << getGprCppMacroName(inst.regS);
        out << " | ";
        printHexOrDecUint32Literal(u16, out);
    }
}

void PseudoCppPrinter::printInst_sll(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);
    const uint32_t shiftAmount = inst.immediateVal & 0x1Fu;

    if (inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (shiftAmount == 0) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == inst.regD) {
        // Can use '<<=' shorthand
        out << " <<= ";
        out << shiftAmount;
    }
    else {
        // Regular shift instruction
        out << " = ";
        out << getGprCppMacroName(inst.regT);
        out << " << ";
        out << shiftAmount;
    }
}

void PseudoCppPrinter::printInst_sllv(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (inst.regS == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprMacroNameOr0(inst.regT);
    }
    else {
        // Regular left shift
        out << " = ";
        out << getGprMacroNameOr0(inst.regT);
        out << " << ";
        out << getGprMacroNameOr0(inst.regS);
    }
}

void PseudoCppPrinter::printInst_slt(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regS == CpuGpr::ZERO) {
        // This is more readable when comparing against zero
        out << " = (i32(";
        out << getGprMacroNameOr0(inst.regT);
        out << ") > 0)";
    }
    else if (inst.regT == CpuGpr::ZERO) {
        // This is more readable when comparing against zero
        out << " = (i32(";
        out << getGprMacroNameOr0(inst.regS);
        out << ") < 0)";
    }
    else {
        out << " = (i32(";
        out << getGprMacroNameOr0(inst.regS);
        out << ") < i32(";
        out << getGprMacroNameOr0(inst.regT);
        out << "))";
    }
}

void PseudoCppPrinter::printInst_slti(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regT);
    const int16_t i16 = (int16_t) inst.immediateVal;

    out << " = (i32(";
    out << getGprMacroNameOr0(inst.regS);
    out << ") < ";
    printHexOrDecInt32Literal(i16, out);
    out << ")";
}

void PseudoCppPrinter::printInst_sltiu(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regT);
    const uint32_t i = (uint32_t)(int32_t)(int16_t) inst.immediateVal;      // Needs to be sign extended as per the MIPS docs

    if (i == 0) {
        // Can never be < 0!
        out << " = 0";
    }
    else {
        out << " = (";
        out << getGprMacroNameOr0(inst.regS);
        out << " < ";
        printHexOrDecUint32Literal(i, out);
        out << ")";
    }
}

void PseudoCppPrinter::printInst_sltu(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regS == CpuGpr::ZERO) {
        // This is more readable when comparing against zero
        out << " = (";
        out << getGprMacroNameOr0(inst.regT);
        out << " > 0)";
    }
    else if (inst.regT == CpuGpr::ZERO) {
        // This can never be true for unsigned numbers (less than 0) - so it just evaluates to '0'
        out << " = 0";
    }
    else {
        out << " = (";
        out << getGprMacroNameOr0(inst.regS);
        out << " < ";
        out << getGprMacroNameOr0(inst.regT);
        out << ")";
    }
}

void PseudoCppPrinter::printInst_sra(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);
    const uint32_t shiftAmount = inst.immediateVal & 0x1Fu;

    if (inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (shiftAmount == 0) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regT);
    }
    else {
        // Regular shift instruction
        out << " = u32(i32(";
        out << getGprCppMacroName(inst.regT);
        out << ") >> ";
        out << shiftAmount;
        out << ")";
    }
}

void PseudoCppPrinter::printInst_srav(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (inst.regS == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprMacroNameOr0(inst.regT);
    }
    else {
        // Regular right shift
        out << " = i32(";
        out << getGprMacroNameOr0(inst.regT);
        out << ") >> ";
        out << getGprMacroNameOr0(inst.regS);
    }
}

void PseudoCppPrinter::printInst_srl(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);
    const uint32_t shiftAmount = inst.immediateVal & 0x1Fu;

    if (inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (shiftAmount == 0) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == inst.regD) {
        // Can use '>>=' shorthand
        out << " >>= ";
        out << shiftAmount;
    }
    else {
        // Regular shift instruction
        out << " = ";
        out << getGprCppMacroName(inst.regT);
        out << " >> ";
        out << shiftAmount;
    }
}

void PseudoCppPrinter::printInst_subu(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regS == CpuGpr::ZERO && inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (inst.regS == CpuGpr::ZERO) {
        // Assigning negative number
        out << " = -";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regS);
    }
    else if (inst.regS == inst.regD) {
        // Regular subtract with '-=' shorthand
        out << " -= ";
        out << getGprCppMacroName(inst.regT);
    }
    else {
        // Regular subtract
        out << " = ";
        out << getGprCppMacroName(inst.regS);
        out << " - ";
        out << getGprCppMacroName(inst.regT);
    }
}

void PseudoCppPrinter::printInst_srlv(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (inst.regS == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprMacroNameOr0(inst.regT);
    }
    else {
        // Regular right shift
        out << " = ";
        out << getGprMacroNameOr0(inst.regT);
        out << " >> ";
        out << getGprMacroNameOr0(inst.regS);
    }
}

void PseudoCppPrinter::printInst_xor(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regD);

    if (inst.regS == CpuGpr::ZERO && inst.regT == CpuGpr::ZERO) {
        // Zero assign
        out << " = 0";
    }
    else if (inst.regS == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == CpuGpr::ZERO) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regS);
    }
    else if (inst.regS == inst.regD) {
        // Regular XOR with '^=' shorthand
        out << " ^= ";
        out << getGprCppMacroName(inst.regT);
    }
    else if (inst.regT == inst.regD) {
        // Regular XOR with '^=' shorthand
        out << " ^= ";
        out << getGprCppMacroName(inst.regS);
    }
    else {
        // Regular XOR
        out << " = ";
        out << getGprCppMacroName(inst.regS);
        out << " ^ ";
        out << getGprCppMacroName(inst.regT);
    }
}

void PseudoCppPrinter::printInst_xori(std::ostream& out, const CpuInstruction& inst) {
    out << getGprCppMacroName(inst.regT);
    const uint16_t i = (uint16_t) inst.immediateVal;

    if (inst.regS == CpuGpr::ZERO) {
        // Literal assign
        out << " = ";
        printHexOrDecUint32Literal(i, out);
    }
    else if (i == 0) {
        // Move instruction
        out << " = ";
        out << getGprCppMacroName(inst.regS);
    }
    else if (inst.regS == inst.regT) {
        // Can use '^=' shorthand
        out << " ^= ";
        printHexOrDecUint32Literal(i, out);
    }
    else {
        // Regular XOR
        out << " = ";
        out << getGprCppMacroName(inst.regS);
        out << " ^ ";
        printHexOrDecUint32Literal(i, out);
    }
}
