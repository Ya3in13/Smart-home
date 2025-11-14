# main_video_test.py
from ultralytics import YOLO
import cv2
import os
from datetime import datetime

# ==================== تنظیمات ====================
USE_GPU = False                    # True → GPU | False → CPU
MODEL_PATH = "yolov11n.pt"     # مدل آموزش‌دیده  /bestv1.pt   / yolov11n.pt
VIDEO_SOURCE = "test3.mp4"   # یا 0 برای وب‌کم
OUTPUT_PATH = "results/output_video.mp4"
SHOW_WINDOW = True                # نمایش زنده
SAVE_VIDEO = True                 # ذخیره ویدیو
# ================================================

# --- مسیرها ---
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MODEL_PATH = os.path.join(SCRIPT_DIR, "models", MODEL_PATH)
VIDEO_SOURCE = os.path.join(SCRIPT_DIR, VIDEO_SOURCE) if not str(VIDEO_SOURCE).isdigit() else int(VIDEO_SOURCE)
OUTPUT_PATH = os.path.join(SCRIPT_DIR, OUTPUT_PATH)

# --- لود مدل ---
print("Loading model...")
model = YOLO(MODEL_PATH)
if USE_GPU:
    model.to('cuda')
print(f"Model loaded on {'GPU' if USE_GPU else 'CPU'}")

# --- باز کردن ویدیو ---
cap = cv2.VideoCapture(VIDEO_SOURCE)
if not cap.isOpened():
    print(f"Error: Cannot open video source: {VIDEO_SOURCE}")
    exit(1)

# --- تنظیمات ویدیو ---
fps = cap.get(cv2.CAP_PROP_FPS)
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

print(f"Video: {width}x{height} @ {fps:.1f} FPS | Total frames: {total_frames}")

# --- تنظیمات ذخیره ---
fourcc = cv2.VideoWriter_fourcc(*'mp4v')
out = None
if SAVE_VIDEO:
    out = cv2.VideoWriter(OUTPUT_PATH, fourcc, fps, (width, height))

# --- پردازش فریم به فریم ---
frame_idx = 0
start_time = datetime.now()

print("Starting inference on video... Press 'q' to quit.")

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    frame_idx += 1

    # --- تشخیص ---
    results = model(frame, device='cuda' if USE_GPU else 'cpu', verbose=False)[0]
    annotated_frame = results.plot()

    # --- نمایش FPS ---
    elapsed = (datetime.now() - start_time).total_seconds()
    current_fps = frame_idx / elapsed if elapsed > 0 else 0
    cv2.putText(annotated_frame, f"FPS: {current_fps:.1f}", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    # --- ذخیره ---
    if SAVE_VIDEO and out:
        out.write(annotated_frame)

    # --- نمایش ---
    if SHOW_WINDOW:
        cv2.imshow('YOLOv11 Video Detection', annotated_frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

# --- پایان ---
cap.release()
if SAVE_VIDEO and out:
    out.release()
cv2.destroyAllWindows()

duration = (datetime.now() - start_time).total_seconds()
print(f"\nDone! Processed {frame_idx} frames in {duration:.1f}s")
print(f"Average FPS: {frame_idx/duration:.1f}")
if SAVE_VIDEO:
    print(f"Output saved: {OUTPUT_PATH}")
    #==================== پایان ====================#
