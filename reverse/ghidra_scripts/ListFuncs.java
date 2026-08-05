import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;

public class ListFuncs extends GhidraScript {
	public void run() throws Exception {
		for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
			AddressRangeIterator body = f.getBody().getAddressRanges();
			long size = 0;
			for (AddressRange r : body) size += r.getLength();
			println("FUN 0x" + f.getEntryPoint() + " size=" + size + " " + f.getName());
		}
	}
}