import machine
import neopixel
import network
import urequests
import time

# Hardware & Network Configuration
LED_PIN = 28
NUM_LEDS = 12
pixels = neopixel.NeoPixel(machine.Pin(LED_PIN), NUM_LEDS)

SSID = "Your_Network_Name"
PASSWORD = "Your_Password"

# OctoPrint Server Configuration
PRINTER_IP = "192.168.1.XX" # Replace with your local printer IP address
API_KEY = "YOUR_OCTOPRINT_API_KEY_HERE" # Get this from OctoPrint Settings -> API
URL = f"http://{PRINTER_IP}/api/printer"
HEADERS = {"X-Api-Key": API_KEY}

# Connect to Wi-Fi
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(SSID, PASSWORD)
while not wlan.isconnected():
    time.sleep(1)
print("Connected to Network!")

# -------------------------------------------------------------
# ANIMATION LOOPS
# -------------------------------------------------------------
def active_research_animation():
    # Intense Factorio science pack swirl logic
    current_color = (0, 255, 0) # Logistics Green as example
    for i in range(NUM_LEDS):
        for p in range(NUM_LEDS): pixels[p] = (0,0,0)
        pixels[i] = current_color
        pixels[(i - 1) % NUM_LEDS] = (0, 100, 0)
        pixels.write()
        time.sleep(0.05)

def idle_breathing_animation():
    # Calm slow pulse when printer is resting
    for step in range(10, 50):
        pixels.fill((int(step * 0.5), 0, int(step * 0.8))) # Dim Purple
        pixels.write()
        time.sleep(0.02)
    for step in range(50, 10, -1):
        pixels.fill((int(step * 0.5), 0, int(step * 0.8)))
        pixels.write()
        time.sleep(0.02)

# -------------------------------------------------------------
# MAIN APP LOOP
# -------------------------------------------------------------
printer_status = "Operational"

while True:
    try:
        # Check printer status from API over the air
        response = urequests.get(URL, headers=HEADERS, timeout=3)
        
        if response.status_code == 200:
            data = response.json()
            # Extract state text: "Printing", "Operational" (Idle), or "Error"
            printer_status = data["state"]["text"]
        elif response.status_code == 409:
            # Code 409 means printer is completely off/disconnected from OctoPrint
            printer_status = "Offline"
        response.close()
    except Exception as e:
        printer_status = "Network Error" # Printer is powered off entirely

    # Route animation depending on live data response
    if "Printing" in printer_status:
        # Loop the wild research animation a few times before testing status again
        for _ in range(10): 
            active_research_animation()
    else:
        # Play one loop of slow pulse if idle, offline, or paused
        idle_breathing_animation()
        
    time.sleep(1) # Delay buffer to prevent overloading the network
