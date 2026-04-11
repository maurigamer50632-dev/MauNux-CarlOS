const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const pty = require('node-pty');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.static(__dirname));

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'gui_index.html'));
});

app.get('/newtab', (req, res) => {
  res.send(`<!DOCTYPE html>
<html>
<head>
<style>
  * { margin:0; padding:0; box-sizing:border-box; }
  body { background: linear-gradient(135deg, #0f0f1a, #1a1a2e);
    display:flex; justify-content:center; align-items:center;
    height:100vh; font-family:'Segoe UI',sans-serif; color:white; flex-direction:column; gap:20px; }
  .logo { font-size:72px; }
  h1 { font-size:28px; font-weight:700; background: linear-gradient(90deg,#6c63ff,#3b82f6); -webkit-background-clip:text; -webkit-text-fill-color:transparent; }
  p { color:rgba(255,255,255,0.4); font-size:13px; }
  .search-bar { display:flex; gap:8px; margin-top:10px; }
  .search-bar input { background:rgba(255,255,255,0.08); border:1px solid rgba(255,255,255,0.15);
    border-radius:24px; padding:10px 20px; color:white; font-size:14px; width:360px; outline:none; }
  .search-bar input:focus { border-color:#6c63ff; }
  .search-bar button { background:#6c63ff; border:none; color:white; padding:10px 18px;
    border-radius:24px; cursor:pointer; font-size:14px; }
  .ua { font-size:11px; color:rgba(255,255,255,0.2); margin-top:8px; font-family:monospace; }
</style>
</head>
<body>
  <div class="logo">🐢</div>
  <h1>MauNux OS — Carl OS v12.02 LTS</h1>
  <p>Tu sistema operativo web. Basado en Carl OS. Impulsado por Firefox.</p>
  <div class="search-bar">
    <input type="text" id="q" placeholder="Busca en la web..." onkeydown="if(event.key==='Enter')go()" autofocus />
    <button onclick="go()">Buscar</button>
  </div>
  <p class="ua">Mozilla/5.0 (X11; Linux x86_64; rv:148.0) Gecko/20100101 Firefox/148.0</p>
  <script>
    function go() {
      const q = document.getElementById('q').value.trim();
      if (!q) return;
      const url = q.startsWith('http') ? q : 'https://www.google.com/search?q=' + encodeURIComponent(q);
      window.location.href = '/proxy?url=' + encodeURIComponent(url);
    }
  </script>
</body>
</html>`);
});

const FIREFOX_UA = 'Mozilla/5.0 (X11; Linux x86_64; rv:148.0) Gecko/20100101 Firefox/148.0';

const BLOCKED_HEADERS = new Set([
  'x-frame-options',
  'content-security-policy',
  'content-security-policy-report-only',
]);

app.get('/proxy', async (req, res) => {
  const url = req.query.url;
  if (!url) return res.status(400).send('No URL');

  try {
    const response = await fetch(url, {
      headers: {
        'User-Agent': FIREFOX_UA,
        'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8',
        'Accept-Language': 'es-ES,es;q=0.8,en-US;q=0.5,en;q=0.3',
        'Accept-Encoding': 'identity',
        'Upgrade-Insecure-Requests': '1',
      },
      redirect: 'follow',
    });

    const contentType = response.headers.get('content-type') || 'text/html';

    for (const [key, value] of response.headers.entries()) {
      if (!BLOCKED_HEADERS.has(key.toLowerCase())) {
        try { res.setHeader(key, value); } catch(e) {}
      }
    }
    res.setHeader('X-MauNux-Browser', `Firefox/148.0 (X11; Linux x86_64)`);

    const isHtml = contentType.includes('text/html');
    const body = isHtml ? await response.text() : await response.arrayBuffer();

    if (isHtml) {
      const base = new URL(url);
      const baseTag = `<base href="${base.origin}${base.pathname}" />`;
      const uaOverride = `<script>
(function() {
  const UA = '${FIREFOX_UA}';
  try { Object.defineProperty(navigator, 'userAgent',  { get: () => UA, configurable: true }); } catch(e) {}
  try { Object.defineProperty(navigator, 'platform',   { get: () => 'Linux x86_64', configurable: true }); } catch(e) {}
  try { Object.defineProperty(navigator, 'appVersion', { get: () => '5.0 (X11)', configurable: true }); } catch(e) {}
  try { Object.defineProperty(navigator, 'oscpu',      { get: () => 'Linux x86_64', configurable: true }); } catch(e) {}
})();
</script>`;
      const patched = body.replace(/<head>/i, `<head>${baseTag}${uaOverride}`);
      res.status(response.status).send(patched);
    } else {
      res.status(response.status).send(Buffer.from(body));
    }
  } catch (err) {
    res.status(200).send(`<!DOCTYPE html>
<html><head><style>
  body { background:#0f0f1a; color:white; font-family:'Segoe UI',sans-serif;
    display:flex; flex-direction:column; align-items:center; justify-content:center; height:100vh; gap:16px; margin:0; }
  .icon { font-size:64px; }
  h2 { color:#ff5f57; margin:0; }
  p { color:rgba(255,255,255,0.5); font-size:13px; text-align:center; }
  .url { font-family:monospace; color:#6c63ff; font-size:12px; }
  .note { background:rgba(255,255,255,0.05); border-radius:8px; padding:12px 20px; font-size:12px; color:rgba(255,255,255,0.4); max-width:400px; text-align:center; }
</style></head>
<body>
  <div class="icon">🦊</div>
  <h2>No se puede conectar</h2>
  <p class="url">${url}</p>
  <p>Este sitio bloqueó la conexión desde MauNux Firefox.</p>
  <div class="note">
    💡 Sitios como Discord usan WebSockets y autenticación propia que requieren acceso directo.<br><br>
    Intenta con: <b>google.com</b>, <b>wikipedia.org</b>, <b>github.com</b>
  </div>
  <p style="font-size:11px;color:rgba(255,255,255,0.2);">Mozilla/5.0 (X11; Linux x86_64; rv:148.0) Gecko/20100101 Firefox/148.0</p>
</body></html>`);
  }
});

app.get('/carlstore', (req, res) => {
  res.send(`<!DOCTYPE html>
<html>
<head>
<style>
  * { margin:0; padding:0; box-sizing:border-box; }
  body { background:#0f0f1a; color:white; font-family:'Segoe UI',sans-serif; padding:20px; }
  h2 { color:#6c63ff; margin-bottom:4px; }
  .sub { color:rgba(255,255,255,0.4); font-size:13px; margin-bottom:20px; }
  .pkg { background:rgba(255,255,255,0.05); border:1px solid rgba(255,255,255,0.08);
    border-radius:10px; padding:14px 16px; margin:10px 0;
    display:flex; align-items:center; gap:14px; }
  .pkg-icon { font-size:34px; }
  .pkg-info h4 { margin:0 0 4px; font-size:14px; }
  .pkg-info p { margin:0; color:rgba(255,255,255,0.4); font-size:12px; }
  .install-btn { margin-left:auto; background:linear-gradient(135deg,#6c63ff,#3b82f6);
    border:none; color:white; padding:7px 16px; border-radius:8px; cursor:pointer; font-size:13px; font-weight:600; }
  .install-btn:hover { filter:brightness(1.15); }
  .install-btn.installed { background:rgba(40,200,80,0.2); border:1px solid #28c840; color:#28c840; cursor:default; }
</style>
</head>
<body>
<h2>📦 Carl Store</h2>
<p class="sub">Paquetes nativos .carl para MauNux / Carl OS</p>
${[
  ['💬','Discord','Chat de voz, vídeo y texto'],
  ['🎵','Spotify','Música en streaming'],
  ['💻','VS Code Web','Editor de código en el navegador'],
  ['🎮','RetroArch Web','Emulador de videojuegos retro'],
  ['🖼️','GIMP Web','Editor de imágenes avanzado'],
  ['📧','Thunderbird Web','Cliente de correo electrónico'],
  ['📺','VLC Web','Reproductor multimedia'],
  ['🗒️','Notion Web','Notas y organización'],
].map(([icon, name, desc]) => `
<div class="pkg">
  <div class="pkg-icon">${icon}</div>
  <div class="pkg-info"><h4>${name}</h4><p>${desc}</p></div>
  <button class="install-btn" onclick="this.textContent='✔ Instalado';this.className='install-btn installed'">Instalar</button>
</div>`).join('')}
</body>
</html>`);
});

io.on('connection', (socket) => {
  console.log('Terminal conectada');

  const shell = pty.spawn('./main', [], {
    name: 'xterm-color',
    cols: 80,
    rows: 24,
    cwd: process.cwd(),
    env: process.env,
  });

  shell.onData(data => socket.emit('output', data));
  socket.on('input', data => shell.write(data));
  socket.on('resize', ({ cols, rows }) => {
    try { shell.resize(cols, rows); } catch(e) {}
  });
  socket.on('disconnect', () => {
    try { shell.kill(); } catch(e) {}
  });
});

const PORT = process.env.PORT || 5000;
server.listen(PORT, '0.0.0.0', () => {
  console.log(`MauNux GUI corriendo en http://0.0.0.0:${PORT}`);
});
