import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;

public class FindRefs extends GhidraScript {
	String[] addrs = { "0x0c008268", "0x0c008320", "0x0c008080" };
	public void run() throws Exception {
		FunctionManager fm = currentProgram.getFunctionManager();
		ReferenceManager rm = currentProgram.getReferenceManager();
		for (String t : addrs) {
			Address a = currentProgram.getAddressFactory().getAddress(t);
			Function cf = fm.getFunctionContaining(a);
			println("### addr " + t + " containing-func=" + (cf == null ? "null" : cf.getName() + "@" + cf.getEntryPoint()));
			ReferenceIterator it = rm.getReferencesTo(a);
			int n = 0;
			while (it.hasNext() && n < 30) {
				Reference r = it.next();
				Address from = r.getFromAddress();
				Function f2 = fm.getFunctionContaining(from);
				println("  REF from " + from + (f2 != null ? "  (in " + f2.getName() + ")" : "") + "  type=" + r.getReferenceType());
				n++;
			}
			if (n == 0) println("  (no references)");
		}
	}
}