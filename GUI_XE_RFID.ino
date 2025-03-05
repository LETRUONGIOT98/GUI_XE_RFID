#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Tạo thể hiện của MFRC522
Servo servo; // Tạo thể hiện của servo
char* allowedCard1 = "D3F7B40F"; // Thay thế bằng UID của thẻ được phép
#define led 7
void setup() {
  Serial.begin(9600); // Khởi tạo giao tiếp serial
  SPI.begin(); // Khởi tạo bus SPI
  mfrc522.PCD_Init(); // Khởi tạo MFRC522
  servo.attach(8); // Kết nối servo với chân 8
  servo.write(90); // Đặt vị trí ban đầu của servo ở 90 độ
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  delay(500); // Chờ servo di chuyển
}
void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Nếu có thẻ mới và đọc thành công
    Serial.println("RFID được phát hiện!");
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) {
        uid += "0"; // Thêm số 0 đầu nếu byte nhỏ hơn 0x10
      }
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase(); // Chuyển sang chữ in hoa
    Serial.print("UID: ");
    Serial.println(uid);
    
    if (uid.equals(allowedCard1)) {
      // Nếu UID trùng với bất kỳ thẻ nào được phép
      Serial.println("Truy cập được phép!");
      servo.write(0); // Di chuyển servo về 0 độ
      delay(5000); // Chờ 5 giây
      servo.write(90); // Di chuyển servo về 90 độ
      delay(500); // Chờ servo di chuyển
    } else {
      // Nếu UID không trùng với bất kỳ thẻ nào được phép
      digitalWrite(led, HIGH);
      Serial.println("Truy cập bị từ chối!");
      delay(2000);
      digitalWrite(led, LOW);
    }
    
    mfrc522.PICC_HaltA(); // Dừng PICC
    mfrc522.PCD_StopCrypto1(); // Dừng mã hóa trên PCD
  } else {
    // Nếu không có thẻ mới được phát hiện
    Serial.println("Không có RFID được phát hiện");
  }
  delay(100); // Đợi một chút giữa các vòng lặp
}
