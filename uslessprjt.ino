#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#define SCREEN_ADDRESS 0x3C 
// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MPU6050 settings
MPU6050 mpu;

// Ball position
int ballX = SCREEN_WIDTH / 2;
int ballY = SCREEN_HEIGHT / 2;
int ballSize = 5; // Radius of the ball

void setup() {
  Serial.begin(115200);
  
  // Initialize the OLED display
  display.begin(SCREEN_ADDRESS, OLED_RESET);
  display.clearDisplay();
   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();
  
  // Check if the MPU6050 is connected
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
  
  Serial.println("MPU6050 connected");
}

void loop() {
  // Read accelerometer data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  
  // Map the accelerometer values to the ball's position
  // Adjust sensitivity by scaling the accelerometer values
  ballY += ax / 100;  // Change the divisor to adjust sensitivity
  ballX += ay / 100;  // Invert Y axis for screen coordinates

  // Keep the ball within screen boundaries
  if (ballX < ballSize) ballX = ballSize;
  if (ballX > SCREEN_WIDTH - ballSize) ballX = SCREEN_WIDTH - ballSize;
  if (ballY < ballSize) ballY = ballSize;
  if (ballY > SCREEN_HEIGHT - ballSize) ballY = SCREEN_HEIGHT - ballSize;

  // Clear the display
  display.clearDisplay();
  
  // Draw the ball
  display.fillCircle(ballX, ballY, ballSize, SSD1306_WHITE);
  
  // Display the position
  display.setCursor(0, 0);
  display.print("X: ");
  display.print(ballX);
  display.print(" Y: ");
  display.println(ballY);
 
  
  display.display();
  
  delay(50);  // Adjust delay for smoother movement
}
