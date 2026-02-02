# ============================================================
# ===============   AUTOMATIC ANPR PIPELINE   =================
#    YOLO (car + plate)  → Crop Plate → OCR → Annotate
#               GPU optimized + batching
# ============================================================

import cv2
import os
import time
from datetime import datetime
from ultralytics import YOLO
import easyocr
import torch

from ultralytics import YOLO




# ==================== تنظیمات ====================
USE_GPU = True               # استفاده از GPU
MODEL_PATH = "models/bestv1.pt"
INPUT_FOLDER = "test"    #test/dataset-car
OUTPUT_FOLDER = "results"
LOG_FOLDER = "log"
IMG_SIZE = 640
BATCH_SIZE = 4               # مقدار مناسب برای GPU
CONF_THRESHOLD = 0.4         # فیلتر کردن دتکشن ضعیف
OCR_LANG = ['fa', 'en']      # OCR فارسی + انگلیسی
# =================================================

model = YOLO(MODEL_PATH)
print("Classes:", model.names)

# ساخت مسیر‌ها
os.makedirs(OUTPUT_FOLDER, exist_ok=True)
os.makedirs(LOG_FOLDER, exist_ok=True)

# فایل لاگ
LOG_FILE = os.path.join(LOG_FOLDER, datetime.now().strftime("run_%Y%m%d_%H%M%S.log"))
def log(*msg):
    line = " ".join(map(str, msg))
    print(line)
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        print(line, file=f)

# ------------------ تعیین استفاده از GPU -------------------
device = "cuda" if (USE_GPU and torch.cuda.is_available()) else "cpu"
log(f"Using device: {device.upper()}")

# ------------------ بارگذاری مدل YOLO ----------------------
log("Loading YOLO model...")
t0 = time.time()
model = YOLO(MODEL_PATH)
if device == "cuda":
    model.to("cuda")
log(f"Model loaded in {time.time() - t0:.2f} sec")

# ------------------ OCR (EasyOCR) ---------------------------
log("Initializing OCR...")
ocr_reader = easyocr.Reader(OCR_LANG, gpu=(device == "cuda"))
log("OCR Ready!\n")

# ------------------ لیست تصاویر -----------------------------
images = [f for f in os.listdir(INPUT_FOLDER)
          if f.lower().endswith((".jpg", ".png", ".jpeg", ".bmp"))]

if len(images) == 0:
    log("No images found!")
    exit()

log(f"Found {len(images)} images.\n")

# ------------------ پردازش تصویر ---------------------------
def process_one_image(img_name):
    img_path = os.path.join(INPUT_FOLDER, img_name)
    out_path = os.path.join(OUTPUT_FOLDER, img_name)

    img = cv2.imread(img_path)
    if img is None:
        log(f"Could not read {img_path}")
        return

    # YOLO detection
    result = model(img, imgsz=IMG_SIZE, device=device, conf=CONF_THRESHOLD, verbose=False)[0]
    boxes = result.boxes

    plate_text = "N/A"
    plate_found = False

    # پردازش دتکشن‌ها
    for box in boxes:
        cls = int(box.cls)
        conf = float(box.conf)
        x1, y1, x2, y2 = box.xyxy[0]

        # فرض: کلاس 1 = پلاک
        if cls == 1:
            plate_found = True

            crop = img[int(y1):int(y2), int(x1):int(x2)]
            if crop.size == 0:
                continue

            # OCR
            ocr_out = ocr_reader.readtext(crop)
            if len(ocr_out):
                plate_text = ocr_out[0][1]

            # نوشتن روی تصویر
            cv2.rectangle(img, (int(x1), int(y1)), (int(x2), int(y2)), (0,255,0), 2)
            cv2.putText(img, plate_text, (int(x1), int(y1)-10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,255,0), 2)

    cv2.imwrite(out_path, img)

    log(f"> {img_name} | Plate: {plate_text} | Plate Found: {plate_found}")

# ------------------ اجرای اصلی -----------------------------
start = time.time()

log("Starting ANPR Pipeline...\n")

for img_name in images:
    process_one_image(img_name)

log("\n================ SUMMARY ================")
log(f"Device: {device.upper()}")
log(f"Images processed: {len(images)}")
log(f"Total time: {time.time() - start:.2f} sec")
log(f"Output: {OUTPUT_FOLDER}")
log("========================================\n")
