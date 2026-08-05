import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;

public class Decompile extends GhidraScript {
	String[] addrs = {
		"0x0c008288", "0x0c008340", "0x0c007e38"
	};
	public void run() throws Exception {
		DecompInterface di = new DecompInterface();
		di.openProgram(currentProgram);
		for (String t : addrs) {
			Address a = currentProgram.getAddressFactory().getAddress(t);
			Function f = currentProgram.getFunctionManager().getFunctionContaining(a);
			if (f == null) {
				println("=== NO FUNCTION at " + t + " ===");
				continue;
			}
			DecompileResults res = di.decompileFunction(f, 60, monitor);
			println("========== " + f.getName() + " @" + f.getEntryPoint() + " size=" + f.getBody().getNumAddresses() + " ==========");
			if (res.decompileCompleted()) {
				println(res.getDecompiledFunction().getC());
			} else {
				println("decompile failed: " + res.getErrorMessage());
			}
		}
	}
}