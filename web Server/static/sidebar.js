// toggle collapse / expand
(function(){
  const sidebar = document.getElementById('sidebar');
  const btn = document.getElementById('toggleBtn');

  // load saved state from localStorage
  const saved = localStorage.getItem('sidebar-collapsed');
  if (saved === 'true') sidebar.classList.add('collapsed');

  btn.addEventListener('click', (e)=>{
    sidebar.classList.toggle('collapsed');
    const collapsed = sidebar.classList.contains('collapsed');
    // rotate icon
    btn.querySelector('i').style.transform = collapsed ? 'rotate(180deg)' : 'rotate(0deg)';
    // save state
    localStorage.setItem('sidebar-collapsed', collapsed ? 'true' : 'false');
  });

  // set initial icon rotation
  if (sidebar.classList.contains('collapsed')) {
    btn.querySelector('i').style.transform = 'rotate(180deg)';
  }
})();
