// سایدبار
const sidebar = document.getElementById('sidebar');
const toggleBtn = document.getElementById('toggleBtn');

toggleBtn.addEventListener('click', () => {
  sidebar.classList.toggle('collapsed');
  localStorage.setItem('sidebar', sidebar.classList.contains('collapsed'));
});

if (localStorage.getItem('sidebar') === 'true') {
  sidebar.classList.add('collapsed');
}

// تغییر وضعیت نور
function toggleLight(name) {
  fetch(`/toggle/${encodeURIComponent(name)}`, { method: 'POST' })
    .then(r => r.json())
    .then(data => {
      if (data.success) {
        const btn = document.querySelector(`[data-name="${name}"] .toggle`);
        btn.classList.toggle('on');
        btn.classList.toggle('off');
      }
    });
}

// انیمیشن بار دما
setTimeout(() => {
  document.querySelector('.bar').style.width = '70%';
}, 500);