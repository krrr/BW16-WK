import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.address.Address;

public class ScanStrings extends GhidraScript {
	public void run() throws Exception {
		MemoryBlock block = null;
		for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
			if (b.getStart().getOffset() == 0x0c000000L) { block = b; break; }
		}
		if (block == null) {
			for (MemoryBlock b : currentProgram.getMemory().getBlocks()) println("block: " + b.getName() + " " + b.getStart() + " len=" + b.getSize());
			return;
		}
		byte[] data = new byte[(int) block.getSize()];
		block.getBytes(block.getStart(), data);
		StringBuilder sb = new StringBuilder();
		int min = 4;
		for (int i = 0; i < data.length; i++) {
			int c = data[i] & 0xff;
			if (c >= 0x20 && c < 0x7f) {
				sb.append((char) c);
			} else {
				if (sb.length() >= min) {
					println("STR 0x" + block.getStart().add(i - sb.length()) + " : " + sb.toString());
				}
				sb.setLength(0);
			}
		}
		if (sb.length() >= min) println("STR 0x" + block.getStart().add(data.length - sb.length()) + " : " + sb.toString());
	}
}
