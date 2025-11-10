from flask import Flask, render_template , redirect , url_for , request  # ایمپورت کتابخانه Flask و توابع مورد نیاز

app = Flask(__name__)

@app.route('/')  # تعریف مسیر اصلی سایت
def index():
    # کاربر از '/' میره به '/home'
    return redirect(url_for('home'))  # ریدایرکت به تابع home

@app.route('/home')
def home():
    # رندر کردن قالب home.html و ارسال پارامترها به قالب
    return render_template("home.html", name=home, title="Home Page", username="Yasin")

if __name__ == '__main__':
    # اجرای برنامه روی همه اینترفیس‌ها و پورت 80
    app.run(host='0.0.0.0', port=80)
       


       