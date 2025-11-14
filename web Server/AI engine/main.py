# main.py
from ultralytics import YOLO
import cv2
import os
import time
from datetime import datetime
from concurrent.futures import ThreadPoolExecutor
from functools import partial

# ==================== تنظیمات اصلی ====================
USE_GPU = False          # فقط این رو تغییر بده: True → GPU | False → CPU
MAX_WORKERS = 1         # فقط برای CPU: 2–6 | برای GPU: 1 (بهتر)
BATCH_SIZE = 1          # برای GPU می‌تونی 4–8 بذاری (در آینده)
# =====================================================

# --- مسیرها ---

datetimeStart = datetime.now()
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MODEL_PATH = os.path.join(SCRIPT_DIR, "yolov11n.pt")
TEST_FOLDER = os.path.join(SCRIPT_DIR, "test")
OUTPUT_FOLDER = os.path.join(SCRIPT_DIR, "results")
LOG_FOLDER = os.path.join(SCRIPT_DIR, "log")

os.makedirs(OUTPUT_FOLDER, exist_ok=True)
os.makedirs(LOG_FOLDER, exist_ok=True)

LOG_FILE = os.path.join(LOG_FOLDER, datetime.now().strftime("run_%Y%m%d_%H%M%S.log"))

def log(*msg):
    line = " ".join(map(str, msg))
    print(line)
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        print(line, file=f)

# --- تشخیص دستگاه ---
device = 'cuda' if USE_GPU and os.system("nvidia-smi >nul 2>&1") == 0 else 'cpu'
if USE_GPU and device == 'cpu':
    log("Warning: GPU requested but not available. Falling back to CPU.")
log(f"Using device: {device.upper()}")

# --- شروع لاگ ---
log("\n" + "="*70)
log("MHA-python-test-YOLOv11")
log(f"YOLOv11 Batch Inference ({'GPU' if device == 'cuda' else 'CPU'} + {'Parallel' if MAX_WORKERS > 1 else 'Sequential'})")
log(f"Start: {datetime.now():%Y-%m-%d %H:%M:%S}")
log(f"Model: {MODEL_PATH}")
log(f"Input: {TEST_FOLDER}")
log(f"Output: {OUTPUT_FOLDER}")
log(f"Log: {LOG_FILE}")
log("="*70 + "\n")

# --- اعتبارسنجی ---
if not os.path.isfile(MODEL_PATH):
    log(f"Error: Model not found: {MODEL_PATH}")
    exit(1)
if not os.path.isdir(TEST_FOLDER):
    log(f"Error: Test folder not found: {TEST_FOLDER}")
    exit(1)

# --- بارگذاری مدل ---
log(f"Loading model on {device}...")
t0 = time.perf_counter()
model = YOLO(MODEL_PATH)
if device == 'cuda':
    model.to('cuda')
model_load_time = time.perf_counter() - t0
log(f"Model loaded in {model_load_time:.3f}s\n")

# --- اسکن تصاویر ---
IMG_EXTS = {'.jpg', '.jpeg', '.png', '.bmp', '.tiff'}
images = [f for f in os.listdir(TEST_FOLDER) if os.path.splitext(f)[1].lower() in IMG_EXTS]
if not images:
    log("No images found.")
    exit(0)

log(f"Found {len(images)} image(s). Starting inference...\n")

# --- تابع پردازش ---
def process_image(model, filename, idx, total):
    img_path = os.path.join(TEST_FOLDER, filename)
    out_path = os.path.join(OUTPUT_FOLDER, f"result_{filename}")

    t1 = time.perf_counter()
    result = model(img_path, device=device, verbose=False)[0]
    t2 = time.perf_counter()
    inf_time = t2 - t1
    fps = 1 / inf_time if inf_time > 0 else 0

    annotated = result.plot()
    success = cv2.imwrite(out_path, annotated)

    obj_count = len(result.boxes) if result.boxes is not None else 0

    log(f"[{idx}/{total}] {filename}")
    log(f"    -> {out_path}")
    log(f"    Inf: {inf_time:.3f}s | FPS: {fps:.1f} | Obj: {obj_count} | {'Success' if success else 'Failed'}")
    return inf_time, obj_count

# --- اجرای موازی (فقط برای CPU) ---
total_inf_time = 0
obj_counts = []

if device == 'cpu' and MAX_WORKERS > 1:
    # CPU: موازی برای I/O
    log(f"Using {MAX_WORKERS} threads for CPU parallel I/O...")
    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
        func = partial(process_image, model)
        tasks = [executor.submit(func, img, i+1, len(images)) for i, img in enumerate(images)]
        for future in tasks:
            inf_time, obj_count = future.result()
            total_inf_time += inf_time
            obj_counts.append(obj_count)
else:
    # GPU یا CPU تک‌ترد: ترتیبی
    log("Running sequentially (GPU or single-thread CPU)...")
    for idx, img in enumerate(images, 1):
        inf_time, obj_count = process_image(model, img, idx, len(images))
        total_inf_time += inf_time
        obj_counts.append(obj_count)

# --- خلاصه ---
total_time = time.perf_counter() - t0
avg_inf = total_inf_time / len(images)
avg_fps = 1 / avg_inf if avg_inf > 0 else 0

log("\n" + "="*70)
log("SUMMARY")
log(f"Device: {device.upper()}")
log(f"Images: {len(images)}")
log(f"Total time: {total_time:.3f}s")
log(f"Model load: {model_load_time:.3f}s")
log(f"Inference: {total_inf_time:.3f}s")
log(f"Avg inf: {avg_inf:.3f}s/image")
log(f"Avg FPS: {avg_fps:.1f}")
log(f"Total objects: {sum(obj_counts)}")
log(f"Results: {OUTPUT_FOLDER}")
log(f"Log: {LOG_FILE}")
log(f"Start: {datetimeStart:%Y-%m-%d %H:%M:%S}")
log(f"End: {datetime.now():%Y-%m-%d %H:%M:%S}")
log("="*70 + "\n")