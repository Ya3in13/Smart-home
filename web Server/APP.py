from flask import Flask, render_template, redirect, url_for, request, jsonify

app = Flask(__name__)

# شبیه‌سازی وضعیت دستگاه‌ها
devices = {
    "light": {"status": "on", "intensity": 80},
    "ac": {"status": "cooling", "temp": 23},
    "vacuum": {"status": "running", "battery": 25},
    "wifi": {"status": "connected", "speed": 36},
}

@app.route('/')
def index():
    return redirect(url_for('dashboard'))

@app.route('/home')
def dashboard():
    return render_template('home.html', devices=devices, username="Yasin")

@app.route('/cam')
def cam():
    return render_template('cam.html', devices=devices, username="Yasin")

@app.route('/update', methods=['POST'])
def update_device():
    data = request.json
    device = data.get('device')
    key = data.get('key')
    value = data.get('value')
    
    if device in devices and key in devices[device]:
        devices[device][key] = value
        return jsonify({"success": True, "devices": devices})
    
    return jsonify({"success": False, "error": "Invalid device or key"}), 400

@app.route('/devices')
def get_devices():
    return jsonify(devices)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, debug=True)