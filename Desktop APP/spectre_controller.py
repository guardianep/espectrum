import tkinter as tk
from tkinter import messagebox
import psutil
import serial
import serial.tools.list_ports
import threading

class SpectreController:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Spectre Audio Mixer Controller - BETA")
        self.root.geometry("400x500")
        
        self.serial_port = None
        self.connect_serial()
        self.setup_ui()
    
    def connect_serial(self):
        try:
            self.serial_port = serial.Serial('COM3', 115200, timeout=1)
            print("Connected to ESP32 on COM3")
        except:
            print("Could not connect to ESP32. Running in offline mode.")
            self.serial_port = None
    
    def get_running_apps(self):
        apps = set()
        try:
            for proc in psutil.process_iter(['name']):
                try:
                    name = proc.info['name']
                    if name.endswith('.exe'):
                        name = name[:-4]
                    apps.add(name)
                except (psutil.NoSuchProcess, psutil.AccessDenied):
                    pass
        except:
            apps = {"Chrome", "Spotify", "Discord", "VSCode", "Firefox"}
        
        return sorted(list(apps))[:20]
    
    def setup_ui(self):
        title = tk.Label(self.root, text="Spectre Slider Configuration", 
                        font=("Arial", 14, "bold"))
        title.pack(pady=10)
        
        frame1 = tk.Frame(self.root)
        frame1.pack(pady=10, padx=10, fill="x")
        tk.Label(frame1, text="Slider 1:", font=("Arial", 10)).pack(side="left")
        self.slider1_app = tk.StringVar()
        dropdown1 = tk.OptionMenu(frame1, self.slider1_app, *self.get_running_apps())
        dropdown1.pack(side="left", padx=10, fill="x", expand=True)
        
        frame2 = tk.Frame(self.root)
        frame2.pack(pady=10, padx=10, fill="x")
        tk.Label(frame2, text="Slider 2:", font=("Arial", 10)).pack(side="left")
        self.slider2_app = tk.StringVar()
        dropdown2 = tk.OptionMenu(frame2, self.slider2_app, *self.get_running_apps())
        dropdown2.pack(side="left", padx=10, fill="x", expand=True)
        
        frame3 = tk.Frame(self.root)
        frame3.pack(pady=10, padx=10, fill="x")
        tk.Label(frame3, text="Slider 3:", font=("Arial", 10)).pack(side="left")
        self.slider3_app = tk.StringVar()
        dropdown3 = tk.OptionMenu(frame3, self.slider3_app, *self.get_running_apps())
        dropdown3.pack(side="left", padx=10, fill="x", expand=True)
        
        save_btn = tk.Button(self.root, text="Save Configuration", 
                            command=self.save_config, bg="#4CAF50", 
                            fg="white", font=("Arial", 11, "bold"))
        save_btn.pack(pady=15)
        
        self.config_label = tk.Label(self.root, text="", fg="blue", 
                                     font=("Arial", 9))
        self.config_label.pack(pady=10)
        
        status_text = "✓ Connected to ESP32" if self.serial_port else "✗ Offline Mode"
        self.status_label = tk.Label(self.root, text=status_text, 
                                     fg="green" if self.serial_port else "red",
                                     font=("Arial", 9))
        self.status_label.pack(pady=5)
    
    def save_config(self):
        config_str = f"S1:{self.slider1_app.get()}|S2:{self.slider2_app.get()}|S3:{self.slider3_app.get()}"
        
        if self.serial_port:
            try:
                self.serial_port.write((config_str + "\n").encode())
                self.config_label.config(text=f"✓ Sent to ESP32\n{config_str}")
            except Exception as e:
                self.config_label.config(text=f"✗ ESP32 Connection Failed\n{str(e)}")
        else:
            self.config_label.config(text=f"✓ Saved (offline)\n{config_str}")
        
        messagebox.showinfo("Saved", "Configuration updated!")
    
    def run(self):
        self.root.mainloop()

if __name__ == "__main__":
    app = SpectreController()
    app.run()