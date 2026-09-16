
# ESP32 Denizcilik Telemetri Ağ Geçidi & Gerçek Zamanlı İzleme Kokpiti



Bu proje, açık deniz seyrüsefer ortamları için tasarlanmış, uçtan uca çalışan düşük gecikmeli bir **IoT Deniz Telemetri Hattı** prototipidir. Masadaki bir ESP32 mikrodenetleyicisinden çıkan telemetri verileri; bulut katmanı üzerinden saniyeler içinde karanlık temalı, profesyonel bir VTS (Gemi Trafik Hizmetleri) istasyon paneline aktarılır ve dinamik rota izleme algoritmasıyla görselleştirilir.


## Ekran Görüntüsü



##  Simülasyon Dinamikleri (Açık Deniz Senaryosu)

Donanım sensörleri (GPS / NMEA anemometre) sahaya çıkmadan önce sistemi doğrulamak adına firmware içinde matematiksel bir seyir simülatörü geliştirilmiştir:

* **Gerçekçi Rota Çizimi:** Ege Denizi'nde (Bodrum & Kos Kanalı açık suları) belirlenen başlangıç referansından itibaren güneybatı rotasında dinamik enlem/boylam delta hesaplaması.
* **Denizcilik Hız Dalgalanması (SOG):** Teknenin akıntı ve dalga direnci simüle edilerek hız değeri **11.5 - 14.5 knot** aralığında dinamik gürültü (noise) ile üretilir.
* **Çevresel Veri Entegrasyonu:** Rüzgar şiddeti eş zamanlı olarak sensör parametresi gibi pakete gömülür.
* **Dinamik Rota Takibi (Polyline):** İstemci tarafında her telemetri paketi hafızadaki diziye işlenerek geçmiş seyir izi (trail) kesikli neon çizgilerle haritaya çizdirilir.


## Uçtan Uca Sistem Mimarisi

```text
[ ESP32 Mikrodenetleyici ]
       │
       ▼ (WiFi 802.11 b/g/n)
  JSON Serileştirme (ArduinoJson)
       │
       ▼ MQTT Paket Yayını (Port: 1883 / TCP)
[ HiveMQ Dağıtık Bulut Broker ]
       │
       ▼ WSS Tüneli (Port: 8884 / TLS)
[ Web Tabanlı İzleme Konsolu ]
  ├── Leaflet.js (Coğrafi Projeksiyon)
  ├── Esri Dark Canvas (Koyu Harita Katmanı)
  └── Canlı Telemetri & Ham Paket Akış Terminali

```


## Temel Özellikler

* **Optimize Edilmiş Veri Yükü (Payload):** Ağ tıkanıklığını önlemek adına NMEA telemetri verileri JSON formatında kompakt bayt dizileri halinde paketlenir.
* **Sıfır Sunucu Gereksinimi:** Web paneli, harici bir backend sunucusuna ihtiyaç duymadan doğrudan tarayıcı üzerinden WebSocket aracılığıyla MQTT broker'ına el sıkışır.
* **Profesyonel Denizcilik Arayüzü:** Koyu mavi tonlarda göz yormayan gece modu tasarımı, anlık koordinat göstergeleri ve canlı paket akış terminali.
* **Oto-Odaklama ve Harita Takibi:** Tekne yeni bir telemetri noktasına ulaştığında harita kadrajı nesneyi merkezde tutar (`map.panTo`).


## Teknoloji Yığını

| Katman | Teknoloji / Kütüphane | Açıklama |
| --- | --- | --- |
| **Donanım** | ESP32 DevKit V1 | 240 MHz Çift Çekirdek Tensilica, 2.4 GHz Entegre Wi-Fi |
| **Gömülü Yazılım** | C++ / Arduino | Mikrodenetleyici firmware mantığı ve simülasyon |
| **Protokol** | MQTT & WebSockets | Düşük gecikmeli hafif IoT mesajlaşma |
| **JSON Motoru** | ArduinoJson v6 | Statik bellek tahsisli hızlı JSON serileştirme |
| **Harita Altyapısı** | Leaflet.js + Esri World Dark | Koyu tema vektörel harita ve dinamik izleme |
| **Arayüz** | Vanilla JS, Modern CSS3 | Sıfır harici CSS kütüphanesi ile yüksek performans |


## Kurulum ve Test

### 1. Donanım Hazırlığı

1. Arduino IDE üzerinde `PubSubClient` ve `ArduinoJson` kütüphanelerini kurun.
2. `esp32_firmware.ino` dosyasındaki ağ değişkenlerini kendi yerel ağınıza göre güncelleyin:
```cpp
const char* ssid = "WIFI_ADINIZ";
const char* password = "WIFI_SIFRENIZ";

```


3. Kodu `115200 baud` ayarında ESP32 kartınıza yükleyin.

### 2. Arayüzün Açılması

* Masaüstündeki `index.html` dosyasını çift tıklayarak tarayıcınızda açın.
* Broker bağlantısı kurulduğu an harita üzerindeki tekne Bodrum açıklarında seyrüsefere başlayacak ve sayaçlar canlı akacaktır.


## Güvenlik ve Gizlilik Beyanı

* Bu depoda yer alan kaynak kodlar hiçbir sabit kişisel ağ şifresi içermez.
* Aktarılan telemetri koordinatları tamamen eğitim, test ve portföy demonstrasyonu amacıyla matematiksel olarak kurgulanmıştır; gerçek bir fiziksel varlığın konumunu yansıtmaz.


