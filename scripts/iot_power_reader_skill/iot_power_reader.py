"""
IOT Power USB Current Meter Real-time Data Reader (Python + iot_parser.dll)
-------------------------------------------------------------------------
Reads real-time Voltage (V), Current (mA/A), and Power (W) from IOT Power USB meters.
Supports USB Direct Devices (e.g., CC-1A60BA0B0).
Automatically extracts `iot_parser.dll` from IOT Power.exe if not found.
"""

import os
import sys
import time
import ctypes
import argparse
import subprocess
from typing import Optional, Dict, Any, List

DEFAULT_EXE_PATH = r"E:\Softwares\IOT Power\IOT Power.exe"
DEFAULT_DLL_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "iot_parser.dll")


def extract_dll_from_exe(exe_path: str, target_dll_path: str) -> bool:
    """
    Extracts `costura64.iot_parser.dll` embedded inside `IOT Power.exe` using PowerShell .NET Reflection.
    """
    if not os.path.exists(exe_path):
        print(f"[Error] Executable not found at: {exe_path}", flush=True)
        return False

    print(f"[Info] Extracting embedded iot_parser.dll from: {exe_path}", flush=True)
    
    safe_exe = exe_path.replace("\\", "\\\\")
    safe_out = target_dll_path.replace("\\", "\\\\")

    ps_script = (
        f"$asm = [System.Reflection.Assembly]::LoadFile('{safe_exe}'); "
        f"$costura = $asm.GetType('Costura.AssemblyLoader'); "
        f"$m = $costura.GetMethod('LoadStream', [System.Reflection.BindingFlags]'Public,NonPublic,Static', $null, @([string]), $null); "
        f"$s = $m.Invoke($null, @('costura64.iot_parser.dll')); "
        f"$fs = [System.IO.File]::Create('{safe_out}'); "
        f"$s.CopyTo($fs); $fs.Close();"
    )

    try:
        res = subprocess.run(["powershell", "-NoProfile", "-Command", ps_script], capture_output=True, text=True)
        if res.returncode == 0 and os.path.exists(target_dll_path) and os.path.getsize(target_dll_path) > 0:
            print(f"[Success] Extracted iot_parser.dll ({os.path.getsize(target_dll_path)} bytes) -> {target_dll_path}", flush=True)
            return True
        else:
            print(f"[Error] PowerShell extraction failed. Error: {res.stderr}", flush=True)
            return False
    except Exception as e:
        print(f"[Error] Failed to execute PowerShell: {e}", flush=True)
        return False


class IoTPowerReader:
    """
    Python wrapper for iot_parser.dll
    """
    def __init__(self, dll_path: str):
        if not os.path.exists(dll_path):
            raise FileNotFoundError(f"DLL not found at: {dll_path}")
        
        try:
            self.dll = ctypes.CDLL(dll_path)
        except Exception as e:
            raise RuntimeError(f"Failed to load DLL {dll_path}: {e}")

        self._init_api_signatures()
        self._is_opened = False

    def _init_api_signatures(self):
        # void iot_uart_send_initial()
        self.dll.iot_uart_send_initial.restype = None
        self.dll.iot_uart_send_initial.argtypes = []

        # byte iot_uart_open(int com)
        self.dll.iot_uart_open.restype = ctypes.c_uint8
        self.dll.iot_uart_open.argtypes = [ctypes.c_int]

        # byte iot_uart_open_usb(char* ptr, uint32 len)
        self.dll.iot_uart_open_usb.restype = ctypes.c_uint8
        self.dll.iot_uart_open_usb.argtypes = [ctypes.c_char_p, ctypes.c_uint32]

        # char* iot_usb_get_devices()
        self.dll.iot_usb_get_devices.restype = ctypes.c_char_p
        self.dll.iot_usb_get_devices.argtypes = []

        # byte iot_uart_is_open()
        self.dll.iot_uart_is_open.restype = ctypes.c_uint8
        self.dll.iot_uart_is_open.argtypes = []

        # void iot_uart_request_close()
        self.dll.iot_uart_request_close.restype = None
        self.dll.iot_uart_request_close.argtypes = []

        # byte iot_parse()
        self.dll.iot_parse.restype = ctypes.c_uint8
        self.dll.iot_parse.argtypes = []

        # double iot_get_current(int i)
        self.dll.iot_get_current.restype = ctypes.c_double
        self.dll.iot_get_current.argtypes = [ctypes.c_int]

        # double iot_get_voltage(int i)
        self.dll.iot_get_voltage.restype = ctypes.c_double
        self.dll.iot_get_voltage.argtypes = [ctypes.c_int]

        # float iot_get_total_wattage()
        self.dll.iot_get_total_wattage.restype = ctypes.c_float
        self.dll.iot_get_total_wattage.argtypes = []

        # uint32 iot_get_pack_id()
        self.dll.iot_get_pack_id.restype = ctypes.c_uint32
        self.dll.iot_get_pack_id.argtypes = []

        # byte iot_get_current_uint()
        self.dll.iot_get_current_uint.restype = ctypes.c_uint8
        self.dll.iot_get_current_uint.argtypes = []

        # byte iot_get_power_on()
        self.dll.iot_get_power_on.restype = ctypes.c_uint8
        self.dll.iot_get_power_on.argtypes = []

    def get_scanned_usb_devices(self) -> List[str]:
        """Returns list of scanned USB direct devices"""
        try:
            raw = self.dll.iot_usb_get_devices()
            if raw:
                dev_str = raw.decode('utf-8', errors='ignore').strip()
                if dev_str:
                    return [dev_str]
        except Exception:
            pass
        return []

    def _warmup(self, timeout: float = 2.0):
        """
        Drains initial configuration/handshake packets from DLL RX buffer until
        valid measurement data is available.
        """
        start = time.time()
        while time.time() - start < timeout:
            while self.dll.iot_parse() != 0:
                pass
            if self.dll.iot_get_voltage(0) > 0 or self.dll.iot_get_current(0) > 0 or self.dll.iot_get_pack_id() > 0:
                if self.dll.iot_get_voltage(0) > 0 or self.dll.iot_get_current(0) > 0:
                    break
            time.sleep(0.02)

    def open_usb(self, device_name: str) -> bool:
        """Opens device via USB Direct Name (e.g. 'CC-1A60BA0B0')"""
        print(f"[Info] Opening USB device '{device_name}' via WinUSB driver...", flush=True)
        b_name = device_name.encode('utf-8')
        res = self.dll.iot_uart_open_usb(b_name, len(b_name))
        if res != 0 or self.dll.iot_uart_is_open() != 0:
            self._is_opened = True
            print(f"[Success] USB device '{device_name}' connected successfully!", flush=True)
            self.dll.iot_uart_send_initial()
            self._warmup()
            return True
        else:
            print(f"[Error] Failed to open USB device '{device_name}'.", flush=True)
            return False

    def close(self):
        """Closes the connection"""
        if self._is_opened:
            self.dll.iot_uart_request_close()
            self._is_opened = False
            print("[Info] Connection closed.", flush=True)

    def read_once(self) -> Dict[str, Any]:
        """
        Parses incoming serial stream and returns current measurement dict.
        Drains all pending queued packets to reach the latest real-time frame.
        """
        # Drain all queued packets in buffer to reach the most recent data frame
        while self.dll.iot_parse() != 0:
            pass

        pack_id = self.dll.iot_get_pack_id()
        voltage = self.dll.iot_get_voltage(0)
        current = self.dll.iot_get_current(0)
        unit_code = self.dll.iot_get_current_uint()
        wattage = self.dll.iot_get_total_wattage()
        is_power_on = bool(self.dll.iot_get_power_on())

        unit_str = "mA" if unit_code == 0 else "A"
        current_ma = current if unit_code == 0 else current * 1000.0

        # Calculate wattage fallback if DLL returns 0.0 for single-channel meters
        if wattage == 0.0 and (voltage > 0 or current > 0):
            wattage = voltage * (current / 1000.0 if unit_code == 0 else current)

        return {
            "timestamp": time.time(),
            "pack_id": pack_id,
            "voltage": voltage,       # Volts (V)
            "current": current,       # Current value
            "current_ma": current_ma, # Current in mA
            "unit": unit_str,         # mA or A
            "wattage": wattage,       # Watts (W)
            "is_power_on": is_power_on
        }

    def read_all_pending(self) -> List[Dict[str, Any]]:
        """
        Parses all queued frames from the DLL buffer without dropping packets.
        Returns a list of measurement dicts for each parsed packet.
        """
        results = []
        now = time.time()
        last_pack_id = None
        while self.dll.iot_parse() != 0:
            pack_id = self.dll.iot_get_pack_id()
            if pack_id == last_pack_id:
                continue
            last_pack_id = pack_id
            
            voltage = self.dll.iot_get_voltage(0)
            current = self.dll.iot_get_current(0)
            unit_code = self.dll.iot_get_current_uint()
            wattage = self.dll.iot_get_total_wattage()
            is_power_on = bool(self.dll.iot_get_power_on())

            unit_str = "mA" if unit_code == 0 else "A"
            current_ma = current if unit_code == 0 else current * 1000.0

            if wattage == 0.0 and (voltage > 0 or current > 0):
                wattage = voltage * (current / 1000.0 if unit_code == 0 else current)

            results.append({
                "timestamp": now,
                "pack_id": pack_id,
                "voltage": voltage,
                "current": current,
                "current_ma": current_ma,
                "unit": unit_str,
                "wattage": wattage,
                "is_power_on": is_power_on
            })
        return results


class PeakAnalyzer:
    """
    Hysteresis Peak/Pulse Analyzer for current data streams.
    Identifies high-current pulses, measures duration, period, duty cycle, and peak/idle currents.
    """
    def __init__(self, high_threshold: float, low_threshold: Optional[float] = None):
        self.high_threshold = high_threshold
        self.low_threshold = low_threshold if low_threshold is not None else high_threshold * 0.8
        
        self.in_peak = False
        self.peak_start_time = 0.0
        self.current_peak_samples = []
        self.idle_samples = []
        
        self.durations_ms: List[float] = []
        self.periods_ms: List[float] = []
        self.peak_currents: List[float] = []
        self.last_peak_start: Optional[float] = None
        self.total_samples = 0

    def feed(self, timestamp: float, current_ma: float):
        self.total_samples += 1

        # Rising Edge -> Enter Peak State
        if not self.in_peak and current_ma >= self.high_threshold:
            self.in_peak = True
            self.peak_start_time = timestamp
            self.current_peak_samples = [current_ma]
            
            if self.last_peak_start is not None:
                self.periods_ms.append((timestamp - self.last_peak_start) * 1000.0)
            self.last_peak_start = timestamp

        # Inside Peak State
        elif self.in_peak and current_ma >= self.low_threshold:
            self.current_peak_samples.append(current_ma)

        # Falling Edge -> Leave Peak State
        elif self.in_peak and current_ma < self.low_threshold:
            self.in_peak = False
            duration_ms = (timestamp - self.peak_start_time) * 1000.0
            self.durations_ms.append(duration_ms)
            if self.current_peak_samples:
                self.peak_currents.append(sum(self.current_peak_samples) / len(self.current_peak_samples))
        else:
            # Idle state sample
            self.idle_samples.append(current_ma)

    def summary(self) -> Dict[str, Any]:
        if not self.durations_ms:
            return {
                "peak_count": 0,
                "total_samples": self.total_samples,
                "msg": f"No current peaks detected above high threshold ({self.high_threshold} mA)."
            }
        
        import statistics
        avg_dur = statistics.mean(self.durations_ms)
        min_dur = min(self.durations_ms)
        max_dur = max(self.durations_ms)
        std_dur = statistics.stdev(self.durations_ms) if len(self.durations_ms) > 1 else 0.0

        avg_period = statistics.mean(self.periods_ms) if self.periods_ms else 0.0
        avg_peak_curr = statistics.mean(self.peak_currents) if self.peak_currents else 0.0
        avg_idle_curr = statistics.mean(self.idle_samples) if self.idle_samples else 0.0

        duty_cycle = (avg_dur / avg_period * 100.0) if avg_period > 0 else 0.0

        return {
            "peak_count": len(self.durations_ms),
            "total_samples": self.total_samples,
            "avg_duration_ms": avg_dur,
            "min_duration_ms": min_dur,
            "max_duration_ms": max_dur,
            "std_duration_ms": std_dur,
            "avg_period_ms": avg_period,
            "avg_peak_current_ma": avg_peak_curr,
            "avg_idle_current_ma": avg_idle_curr,
            "duty_cycle_percent": duty_cycle
        }


def main():
    parser = argparse.ArgumentParser(description="IOT Power USB Current Meter Real-time Reader & Peak Analyzer")
    parser.add_argument("-d", "--device", type=str, help="USB direct device name (e.g. CC-1A60BA0B0)")
    parser.add_argument("-i", "--interval", type=float, default=1.0, help="Polling interval in seconds (default: 1s)")
    parser.add_argument("--duration", type=float, default=None, help="Optional duration in seconds to run before stopping automatically")
    parser.add_argument("--dll", type=str, default=DEFAULT_DLL_PATH, help="Path to iot_parser.dll")
    parser.add_argument("--exe", type=str, default=DEFAULT_EXE_PATH, help="Path to IOT Power.exe for extraction")
    parser.add_argument("--extract-only", action="store_true", help="Only extract iot_parser.dll and exit")
    parser.add_argument("-o", "--output", type=str, help="Optional CSV file path to record data")
    
    # Peak analysis arguments
    parser.add_argument("--analyze-peaks", action="store_true", help="Enable pulse/peak duration analysis")
    parser.add_argument("--threshold", type=float, default=None, help="High threshold in mA for peak detection")
    parser.add_argument("--low-threshold", type=float, default=None, help="Low threshold in mA for peak hysteresis")
    parser.add_argument("--fast", action="store_true", help="Enable high-speed sampling mode (minimal sleep interval)")

    args = parser.parse_args()

    # Step 1: Check or Extract DLL
    if not os.path.exists(args.dll):
        ok = extract_dll_from_exe(args.exe, args.dll)
        if not ok:
            print("[Fatal] Could not obtain iot_parser.dll. Exiting.", flush=True)
            sys.exit(1)

    if args.extract_only:
        print(f"[Done] DLL extracted at {args.dll}", flush=True)
        sys.exit(0)

    # Step 2: Initialize Reader
    try:
        reader = IoTPowerReader(args.dll)
    except Exception as e:
        print(f"[Fatal] {e}", flush=True)
        sys.exit(1)

    # Step 3: Auto-detect or Connect
    connected = False

    if args.device:
        connected = reader.open_usb(args.device)
    else:
        # Auto-detection mode: Scan USB Direct Devices first!
        scanned_usb = reader.get_scanned_usb_devices()
        if scanned_usb:
            print(f"[Auto-Detect] Found USB Direct Device: {scanned_usb[0]}", flush=True)
            connected = reader.open_usb(scanned_usb[0])

    if not connected:
        print("[Fatal] Could not connect to any IOT Power device. Ensure device is plugged in and software is not occupying it.", flush=True)
        sys.exit(1)

    analyzer = None
    if args.analyze_peaks or args.threshold is not None:
        high_th = args.threshold if args.threshold is not None else 50.0
        analyzer = PeakAnalyzer(high_threshold=high_th, low_threshold=args.low_threshold)
        print(f"[Peak Analyzer] Enabled (High Threshold: {high_th} mA, Low Threshold: {analyzer.low_threshold} mA)", flush=True)

    polling_interval = 0.005 if (args.fast or analyzer) else args.interval

    print("\n--- Starting Real-time Current Data Reading (Press Ctrl+C to Stop) ---", flush=True)
    if not analyzer:
        print(f"{'Time':<10} | {'Pack ID':<10} | {'Voltage (V)':<12} | {'Current':<16} | {'Power (W)':<12}", flush=True)
        print("-" * 68, flush=True)
    else:
        print(f"[High-Speed Mode] Streaming data samples... (Interval: {polling_interval*1000:.1f}ms)", flush=True)

    csv_file = None
    if args.output:
        csv_file = open(args.output, "a", encoding="utf-8")
        if os.path.getsize(args.output) == 0:
            csv_file.write("timestamp,pack_id,voltage_v,current,unit,wattage_w\n")

    start_time = time.time()
    last_print_time = 0.0

    try:
        while True:
            if analyzer or args.fast:
                frames = reader.read_all_pending()
                if not frames:
                    # Single read fallback if no pending queue
                    single_frame = reader.read_once()
                    if single_frame:
                        frames = [single_frame]

                for data in frames:
                    if analyzer:
                        analyzer.feed(data["timestamp"], data["current_ma"])

                    if csv_file:
                        csv_file.write(f"{data['timestamp']:.3f},{data['pack_id']},{data['voltage']:.3f},{data['current']:.3f},{data['unit']},{data['wattage']:.3f}\n")

                if csv_file and frames:
                    csv_file.flush()

                # Periodic status line in analyzer mode
                now = time.time()
                if analyzer and (now - last_print_time >= 1.0):
                    last_print_time = now
                    peaks_found = len(analyzer.durations_ms)
                    latest_curr = frames[-1]["current_ma"] if frames else 0.0
                    print(f"[{time.strftime('%H:%M:%S')}] Total Samples: {analyzer.total_samples} | Peaks Found: {peaks_found} | Current: {latest_curr:.2f} mA", flush=True)

            else:
                data = reader.read_once()
                t_str = time.strftime("%H:%M:%S", time.localtime(data["timestamp"]))
                c_str = f"{data['current']:.3f} {data['unit']}"
                
                print(f"{t_str:<10} | {data['pack_id']:<10} | {data['voltage']:<12.3f} | {c_str:<16} | {data['wattage']:<12.3f}", flush=True)

                if csv_file:
                    csv_file.write(f"{data['timestamp']:.3f},{data['pack_id']},{data['voltage']:.3f},{data['current']:.3f},{data['unit']},{data['wattage']:.3f}\n")
                    csv_file.flush()

            if args.duration is not None and (time.time() - start_time) >= args.duration:
                print(f"\n[Info] Completed {args.duration}s reading duration.", flush=True)
                break

            time.sleep(polling_interval)

    except KeyboardInterrupt:
        print("\n[Info] Stopping real-time reading...", flush=True)
    finally:
        reader.close()
        if csv_file:
            csv_file.close()

        if analyzer:
            res = analyzer.summary()
            print("\n" + "=" * 50, flush=True)
            print("         CURRENT PEAK ANALYSIS REPORT         ", flush=True)
            print("=" * 50, flush=True)
            if res.get("peak_count", 0) > 0:
                print(f"Total Samples Recorded : {res['total_samples']}", flush=True)
                print(f"Detected Peak Count    : {res['peak_count']}", flush=True)
                print(f"Avg Peak Duration      : {res['avg_duration_ms']:.2f} ms (Min: {res['min_duration_ms']:.2f}ms, Max: {res['max_duration_ms']:.2f}ms)", flush=True)
                print(f"Duration Std Dev       : {res['std_duration_ms']:.2f} ms", flush=True)
                print(f"Avg Pulse Period       : {res['avg_period_ms']:.2f} ms", flush=True)
                print(f"Avg Peak Current       : {res['avg_peak_current_ma']:.2f} mA", flush=True)
                print(f"Avg Idle Current       : {res['avg_idle_current_ma']:.2f} mA", flush=True)
                print(f"Duty Cycle             : {res['duty_cycle_percent']:.2f} %", flush=True)
            else:
                print(res.get("msg", "No peaks detected."), flush=True)
            print("=" * 50 + "\n", flush=True)

if __name__ == "__main__":
    main()


