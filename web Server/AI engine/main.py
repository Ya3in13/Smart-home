# main.py
from ultralytics import YOLO
import cv2
import os
import time
from datetime import datetime

# --- مسیرها ---
current_dir = os.path.dirname(os.path.abspath(__file__))
model_path = os.path.join(current_dir, "yolov11n.pt")
test_folder = os.path.join(current_dir, "test")
output_folder = os.path.join(current_dir, "results")
log_folder = os.path.join(current_dir, "log")

# --- ایجاد پوشه‌های خروجی و لاگ ---
os.makedirs(output_folder, exist_ok=True)
os.makedirs(log_folder, exist_ok=True)

# --- فایل لاگ با نام زمانی ---
log_filename = datetime.now().strftime("run_%Y%m%d_%H%M%S.log")
log_path = os.path.join(log_folder, log_filename)

# --- تابع نوشتن در لاگ (هم کنسول، هم فایل) ---
def log_print(*args, **kwargs):
    message = " ".join(map(str, args))
    print(message, **kwargs)
    with open(log_path, "a", encoding="utf-8") as f:
        print(message, file=f, **kwargs)

# --- شروع لاگ ---
log_print(f"\n{'='*60}")
log_print(f"MHA-python-test-YOLOv11")
log_print(f"YOLOv11 Inference Batch Run")
log_print(f"Start time: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
log_print(f"Model: {model_path}")
log_print(f"Input folder: {test_folder}")
log_print(f"Output folder: {output_folder}")
log_print(f"Log file: {log_path}")
log_print(f"{'='*60}\n")

# --- بررسی وجود فایل مدل ---
if not os.path.exists(model_path):
    log_print(f"Error: model not found!\n   {model_path}")
    exit()

# --- بررسی وجود پوشه test ---
if not os.path.exists(test_folder):
    log_print(f"Error: test folder not found!\n   {test_folder}")
    exit()

# --- مرحله ۱: بارگذاری مدل ---
log_print("Loading YOLOv11 model...")
t_start = time.perf_counter()
model = YOLO(model_path)
t_model = time.perf_counter()
model_load_time = t_model - t_start
log_print(f"Model loaded successfully. Time: {model_load_time:.3f}s\n")

# --- لیست فایل‌های تصویری ---
image_extensions = ('.jpg', '.jpeg', '.png', '.bmp', '.tiff')
image_files = [
    f for f in os.listdir(test_folder)
    if f.lower().endswith(image_extensions)
]

if not image_files:
    log_print("No image files found in 'test' folder.")
    log_print(f"Total time: {time.perf_counter() - t_start:.3f}s")
    log_print(f"{'='*60}")
    exit()

log_print(f"Found {len(image_files)} image(s). Starting inference...\n")

# --- حلقه روی تصاویر ---
total_inference_time = 0
inference_times = []

for idx, filename in enumerate(image_files, 1):
    img_path = os.path.join(test_folder, filename)
    output_path = os.path.join(output_folder, f"result_{filename}")

    # --- تشخیص ---
    t1 = time.perf_counter()
    results = model(img_path)[0]
    t2 = time.perf_counter()
    inference_time = t2 - t1
    total_inference_time += inference_time
    inference_times.append(inference_time)

    # --- اعمال نتایج ---
    annotated_img = results.plot()

    # --- ذخیره ---
    save_success = cv2.imwrite(output_path, annotated_img)

    # --- لاگ هر تصویر ---
    status = "Success" if save_success else "Failed"
    log_print(f"[{idx}/{len(image_files)}] {filename}")
    log_print(f"    -> {output_path}")
    log_print(f"    Inference: {inference_time:.3f}s | FPS: {1/inference_time:.1f} | Status: {status}")

    # --- نمایش تعداد اشیاء تشخیص داده شده (اختیاری) ---
    num_objects = len(results.boxes) if results.boxes is not None else 0
    log_print(f"    Detected objects: {num_objects}")

# --- خلاصه نهایی ---
total_time = time.perf_counter() - t_start
avg_inference = total_inference_time / len(image_files)
avg_fps = 1 / avg_inference if avg_inference > 0 else 0

log_print("\n" + "="*60)
log_print("SUMMARY")
log_print(f"Processed images: {len(image_files)}")
log_print(f"Total time: {total_time:.3f}s")
log_print(f"Model load time: {model_load_time:.3f}s")
log_print(f"Inference total: {total_inference_time:.3f}s")
log_print(f"Average inference: {avg_inference:.3f}s/image")
log_print(f"Average FPS: {avg_fps:.1f}")
log_print(f"Results saved in: {output_folder}")
log_print(f"Log saved in: {log_path}")
log_print(f"End time: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
log_print("="*60 + "\n")