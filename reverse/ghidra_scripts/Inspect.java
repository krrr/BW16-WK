import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.Address;

public class Inspect extends GhidraScript {
	public void run() throws Exception {
		FunctionManager fm = currentProgram.getFunctionManager();
		println("Program image base: 0x" + Long.toHexString(currentProgram.getImageBase().getOffset()));
		println("Language: " + currentProgram.getLanguage().getLanguageID());
		println("Number of functions: " + fm.getFunctionCount());
		Address e = currentProgram.getAddressFactory().getAddress("0x0c000020");
		Instruction inst = currentProgram.getListing().getInstructionAt(e);
		if (inst != null) {
			println("0x0c000020 is instruction: " + inst.toString());
		} else {
			println("0x0c000020 is NOT an instruction");
		}
		long n = 0;
		InstructionIterator ii = currentProgram.getListing().getInstructions(true);
		while (ii.hasNext()) { ii.next(); n++; }
		println("Total instructions: " + n);
		int i = 0;
		for (Function f : fm.getFunctions(true)) {
			if (i++ < 20) println("func: " + f.getName() + " @" + f.getEntryPoint());
		}
	}
}
