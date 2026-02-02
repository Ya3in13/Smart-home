# main_optimized.py
from ultralytics import YOLO
import cv2
import os
import time
from datetime import datetime
from concurrent.futures import ThreadPoolExecutor

# ==================== تنظیمات ====================
USE_GPU = True
BATCH_SIZE = 30      # RTX 3050 = 8 تا 16
IMG_SIZE = 640       # کیفیت + سرعت مناسب
#=================================================

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MODEL_PATH = os.path.join(SCRIPT_DIR, "models", "bestv1.pt")
TEST_FOLDER = os.path.join(SCRIPT_DIR, "test/dataset-car")
OUTPUT_FOLDER = os.path.join(SCRIPT_DIR, "results")
LOG_FOLDER = os.path.join(SCRIPT_DIR, "log")

os.makedirs(OUTPUT_FOLDER, exist_ok=True)
os.makedirs(LOG_FOLDER, exist_ok=True)

log_file = os.path.join(LOG_FOLDER, datetime.now().strftime("run_%Y%m%d_%H%M%S.log"))

def log(msg):
    print(msg)
    with open(log_file, "a", encoding="utf-8") as f:
        f.write(msg + "\n")

# ------------------- Device ----------------------
device = "cuda" if USE_GPU else "cpu"
log(f"Using device: {device}")

# ------------------- Load model -------------------
log("Loading model...")
t0 = time.perf_counter()
model = YOLO(MODEL_PATH)
model.to(device)
log(f"Model load time: {time.perf_counter() - t0:.3f}s")

# ------------------- Load images to RAM ----------
IMG_EXTS = {".jpg", ".jpeg", ".png", ".bmp"}
files = [f for f in os.listdir(TEST_FOLDER) if os.path.splitext(f)[1].lower() in IMG_EXTS]

if not files:
    log("No images found.")
    exit()

log(f"Found {len(files)} images. Preloading to RAM...")

images = []
names = []

for f in files:
    path = os.path.join(TEST_FOLDER, f)
    img = cv2.imread(path)
    if img is None:
        continue
    images.append(img)
    names.append(f)

log(f"Loaded {len(images)} images into RAM.")

# ------------------- Batch Inference ------------
log("Starting batch inference...\n")

total_inf_time = 0
idx = 0
results_all = []

for i in range(0, len(images), BATCH_SIZE):
    batch_imgs = images[i:i+BATCH_SIZE]
    t1 = time.perf_counter()

    results = model(batch_imgs, imgsz=IMG_SIZE, device=device, verbose=False)

    t2 = time.perf_counter()
    inf_time = t2 - t1
    total_inf_time += inf_time

    log(f"Batch {i//BATCH_SIZE + 1} | Size: {len(batch_imgs)} | Time: {inf_time:.3f}s")

    results_all.extend(results)

# ------------------- Async Saving -----------------
log("\nSaving annotated results...")

def save_result(result, name):
    out_path = os.path.join(OUTPUT_FOLDER, f"result_{name}")
    cv2.imwrite(out_path, result.plot())

with ThreadPoolExecutor(max_workers=6) as pool:
    for r, name in zip(results_all, names):
        pool.submit(save_result, r, name)

# ------------------- SUMMARY ----------------------
avg_inf = total_inf_time / len(results_all)
avg_fps = 1 / avg_inf if avg_inf else 0

log("\n" + "="*60)
log("SUMMARY")
log(f"Device: {device}")
log(f"Images: {len(images)}")
log(f"Batch size: {BATCH_SIZE}")
log(f"Total inference time: {total_inf_time:.3f}s")
log(f"Avg time per image: {avg_inf:.4f}s")
log(f"Avg FPS: {avg_fps:.1f}")
log(f"Results saved in: {OUTPUT_FOLDER}")
log("="*60 + "\n")
