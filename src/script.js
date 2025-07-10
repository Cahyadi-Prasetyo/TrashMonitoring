// script.js

// Ganti konfigurasi berikut dengan milik Anda
const firebaseConfig = {
  apiKey: "AIzaSyD1ysbuA3eZAv10qVjiUJCeSdpGIQ-XVT0",
  authDomain: "tes1-31669.firebaseapp.com",
  databaseURL: "https://tes1-31669-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "tes1-31669",
  storageBucket: "tes1-31669.firebasestorage.app",
  messagingSenderId: "118557297592",
  appId: "1:118557297592:web:e0fbbf4d2b4e854e6ddd4d",
  measurementId: "G-B56THS67W7"
};

firebase.initializeApp(firebaseConfig);
const db = firebase.database();

function updateUI(data) {
  document.getElementById('humidity').textContent = data.humidity !== undefined ? data.humidity : '-';
  document.getElementById('mq2Value').textContent = data.mq2Value !== undefined ? data.mq2Value : '-';
  document.getElementById('distance').textContent = data.distance !== undefined ? data.distance : '-';

  // Status tutup
  const tutupStatus = document.getElementById('tutupStatus');
  if (data.tutupTerbuka) {
    tutupStatus.innerHTML = '<span class="status-icon">🔓</span>TERBUKA';
    tutupStatus.className = "status open";
  } else {
    tutupStatus.innerHTML = '<span class="status-icon">🔒</span>TERTUTUP';
    tutupStatus.className = "status closed";
  }

  // Status gas
  const gasStatus = document.getElementById('gasStatus');
  if (data.gasDetected) {
    gasStatus.innerHTML = '<span class="status-icon">⚠️</span>GAS TERDETEKSI!';
    gasStatus.className = "status gas-danger";
  } else {
    gasStatus.innerHTML = '<span class="status-icon">✅</span>AMAN';
    gasStatus.className = "status gas-safe";
  }
}

// Perhatikan perubahan di sini: ambil data dari node 'DATA'
db.ref('DATA').on('value', (snapshot) => {
  const data = snapshot.val() || {};
  updateUI(data);
});