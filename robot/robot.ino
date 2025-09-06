// Định nghĩa chân điều khiển động cơ
#define AIN1 26  // Động cơ trái IN1
#define AIN2 25  // Động cơ trái IN2
#define PWMA 27  // PWM động cơ trái
#define BIN1 32  // Động cơ phải IN1
#define BIN2 23  // Động cơ phải IN2
#define PWMB 33  // PWM động cơ phải
#define STBY 22  // Standby

// Tốc độ PWM (0 - 255), mặc định 100
int speedMotor = 100;

void setup() {
  // Khai báo các chân
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH); // Kích hoạt IC

  // Khởi tạo Serial
  Serial.begin(115200);
  Serial.println("Nhap lenh: w (tien), s (lui), a (trai), d (phai), q (dung), v[speed] (dat toc do, vi du: v100)");

  // Dừng động cơ khi khởi động
  stopMotors();
}

// Hàm điều khiển động cơ
void motorA_forward(int speed) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed);
}

void motorA_backward(int speed) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, speed);
}

// void motorB_forward(int speed) {
//   digitalWrite(BIN1, HIGH);
//   digitalWrite(BIN2, LOW);
//   analogWrite(PWMB, speed);
// }

// void motorB_backward(int speed) {
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, HIGH);
//   analogWrite(PWMB, speed);
// }

void stopMotors() {
  analogWrite(PWMA, 0);
  // analogWrite(PWMB, 0);
  Serial.println("Dung");
}

// Hàm di chuyển tiến
void moveForward() {
  motorA_forward(speedMotor);
  // motorB_forward(speedMotor);
  Serial.println("Tien");
}

// Hàm di chuyển lùi
void moveBackward() {
  motorA_backward(speedMotor);
  // motorB_backward(speedMotor);
  Serial.println("Lui");
}

// Hàm xoay trái
void turnLeft() {
  motorA_backward(speedMotor);
  // motorB_forward(speedMotor);
  Serial.println("Xoay trai");
}

// Hàm xoay phải
void turnRight() {
  motorA_forward(speedMotor);
  // motorB_backward(speedMotor);
  Serial.println("Xoay phai");
}

void loop() {
  // Kiểm tra nếu có dữ liệu từ Serial
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Đọc toàn bộ dòng
    command.trim(); // Loại bỏ khoảng trắng hoặc ký tự thừa

    // Xử lý lệnh
    if (command == "w") {
      moveForward();
    } else if (command == "s") {
      moveBackward();
    } else if (command == "a") {
      turnLeft();
    } else if (command == "d") {
      turnRight();
    } else if (command == "q") {
      stopMotors();
    } else if (command.startsWith("v")) {
      // Xử lý lệnh đặt tốc độ, ví dụ: v100
      String speedStr = command.substring(1); // Lấy phần sau 'v'
      int newSpeed = speedStr.toInt();
      if (newSpeed >= 0 && newSpeed <= 255) {
        speedMotor = newSpeed;
        Serial.println("Toc do cap nhat: " + String(speedMotor));
      } else {
        Serial.println("Toc do khong hop le! Nhap tu 0 den 255.");
      }
    } else {
      Serial.println("Lenh khong hop le! Su dung: w, s, a, d, q, v[speed]");
    }
  }
}