from flask import Flask, render_template, jsonify

app = Flask(__name__)

# داده‌های شبیه‌سازی شده (در واقعیت از Home Assistant API)
weather = {
    "current": "Partly cloudy",
    "temp": 19.4,
    "forecast": [
        {"day": "Thu", "temp": 28.6, "icon": "sun"},
        {"day": "Fri", "temp": 27.1, "icon": "cloud"},
        {"day": "Sat", "temp": 25.3, "icon": "rain"},
        {"day": "Sun", "temp": 28.9, "icon": "sun"},
    ]
}

lights = [
    {"name": "Ambient Lamp", "state": "on"},
    {"name": "Dining Room", "state": "off"},
    {"name": "Living Room", "state": "off"},
    {"name": "Dome Lights", "state": "off"},
    {"name": "Ambient Puck", "state": "off"},
    {"name": "Ambient LEDs", "state": "on", "brightness": 49},
]

energy = {
    "solar": 87,
    "grid": 209,
    "home": 295,
    "boiler100": 25.8,
    "boiler150": 25.3,
}

@app.route('/')
def index():
    return render_template('home.html', 
                         weather=weather, 
                         lights=lights, 
                         energy=energy,
                         username="یاسین")

@app.route('/toggle/<device>', methods=['POST'])
def toggle(device):
    # شبیه‌سازی تغییر وضعیت
    for light in lights:
        if light["name"] == device:
            light["state"] = "on" if light["state"] == "off" else "off"
    return jsonify(success=True)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, debug=True)