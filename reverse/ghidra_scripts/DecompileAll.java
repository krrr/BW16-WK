import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import java.io.*;

// 用法：全量反编译当前 program 的所有函数到 decompiled.txt（本目录下）。
//   可选环境变量 KM0_DECOMP_OUT 指定输出路径。
//   结果以本文件为准（单目标脚本 stdout 可能返回空 C）。
public class DecompileAll extends GhidraScript {
	public void run() throws Exception {
		String outPath = System.getenv("KM0_DECOMP_OUT");
		if (outPath == null || outPath.isEmpty()) {
			outPath = getSourceFile().getParent() + "/decompiled.txt";
		}
		PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(outPath)));
		DecompInterface di = new DecompInterface();
		di.openProgram(currentProgram);
		int done = 0, fail = 0;
		for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
			DecompileResults res = di.decompileFunction(f, 120, monitor);
			w.println("========================================================");
			w.println("FUNC " + f.getName() + " @" + f.getEntryPoint() + "  size=" + f.getBody().getNumAddresses());
			w.println("========================================================");
			if (res.decompileCompleted()) {
				w.println(res.getDecompiledFunction().getC());
				done++;
			} else {
				w.println("DECOMPILE FAILED: " + res.getErrorMessage());
				fail++;
			}
			w.flush();
		}
		w.close();
		println("Decompiled OK=" + done + " fail=" + fail + " -> " + outPath);
	}
}