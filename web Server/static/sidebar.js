/*
  sidebar.js
  توضیح: مدیریت رفتار جمع/باز شدن نوار کناری (sidebar)

  عملکرد اصلی:
  - با کلیک روی دکمه toggle وضعیت کلاس `collapsed` روی المان سایدبار تغییر می‌کند.
  - وضعیت (collapsed یا expanded) در `localStorage` ذخیره می‌شود تا بعد از بارگذاری مجدد صفحه حفظ شود.
  - آیکون دکمه بر اساس وضعیت می‌چرخد تا کاربر وضعیت را بهتر متوجه شود.
*/
(function(){
  // گرفتن ارجاعات به المان‌های مورد نیاز
  const sidebar = document.getElementById('sidebar');
  const btn = document.getElementById('toggleBtn');

  // بارگذاری وضعیت ذخیره شده از localStorage (اگر قبلاً کاربر سایدبار را جمع کرده باشد)
  const saved = localStorage.getItem('sidebar-collapsed');
  if (saved === 'true') sidebar.classList.add('collapsed');

  // رویداد کلیک روی دکمه: تغییر حالت جمع/باز و ذخیره وضعیت
  btn.addEventListener('click', (e)=>{
    // تغییر کلاس برای حالت جمع/باز
    sidebar.classList.toggle('collapsed');
    const collapsed = sidebar.classList.contains('collapsed');

    // چرخش آیکون برای نشان دادن جهت (نمایش بصری تغییر حالت)
    btn.querySelector('i').style.transform = collapsed ? 'rotate(180deg)' : 'rotate(0deg)';

    // ذخیره وضعیت فعلی در localStorage (true یا false)
    localStorage.setItem('sidebar-collapsed', collapsed ? 'true' : 'false');
  });

  // تنظیم اولیهٔ چرخش آیکون در صورتی که سایدبار از قبل جمع باشد
  if (sidebar.classList.contains('collapsed')) {
    btn.querySelector('i').style.transform = 'rotate(180deg)';
  }
})();
