import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.SourceType;

// 用法：给无 xref 的裸二进制目标地址手动建函数（RF-ON/OFF 等被指针表间接调用的例程）。
//   analyzeHeadless <proj> <projname> -process km0_image2_all.bin -noanalysis \
//     -scriptPath reverse/ghidra_scripts -postScript MakeFuncs2.java
// 注意：createFunction 必须传显式 body（AddressSet），传 null 会报
// "Function body must contain the entrypoint"。
public class MakeFuncs2 extends GhidraScript {
	String[] addrs = { "0x0c008288", "0x0c008340", "0x0c007e38", "0x0c00841c" };
	public void run() throws Exception {
		FunctionManager fm = currentProgram.getFunctionManager();
		Listing listing = currentProgram.getListing();
		for (String t : addrs) {
			Address a = currentProgram.getAddressFactory().getAddress(t);
			if (listing.getInstructionAt(a) == null) { try { disassemble(a); } catch (Exception e) {} }
			Function cf = fm.getFunctionContaining(a);
			if (cf != null && cf.getEntryPoint().equals(a)) { println(t + " already fn"); continue; }
			println(t + " containing=" + (cf == null ? "null" : cf.getName()));
			AddressSet body = new AddressSet();
			Address cur = a;
			for (int g = 0; g < 2000 && cur != null; g++) {
				Instruction in = listing.getInstructionAt(cur);
				if (in == null) break;
				body.add(cur);
				if (in.getMnemonicString().equals("pop") || in.getMnemonicString().equals("bx")) break;
				cur = in.getFallThrough();
			}
			try {
				Function f = fm.createFunction(null, a, body, SourceType.USER_DEFINED);
				println(t + " created " + f.getName() + " bodyLen=" + body.getNumAddresses());
			} catch (Exception e) { println(t + " create err: " + e.getMessage()); }
		}
	}
}