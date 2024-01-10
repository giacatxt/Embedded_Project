
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#define RST_PIN         4           // Configurable, see typical pin layout above
#define SS_PIN          5          // Configurable, see typical pin layout above

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // điều khiển màn hình


Servo MyServo; // điều khiển vị trí và góc quay của servo motor
static const int servoPin = 13;//thiết lập để servo motor sẽ được điều khiển thông qua chân số 13 của vi điều khiển.
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance đọc dữ liệu thẻ từ
String previousName = "";
int dem = 0;
int UID[4];
int ID1[4] = {179, 42, 138, 253}; 
//*****************************************************************************************//
void setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC. Nhân dữ liệu qua cổng Serial vs tốc độ 9600 bit/s
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
  
  MyServo.attach(servoPin); // servo motor sẽ được điều khiển thông qua chân số 13 của vi điều khiển.

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);// SSD1306_SWITCHCAPVCC là một chế độ cung cấp điện áp chuyển đổi, được sử dụng để tối ưu hóa hiệu suất.
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(3, 20);
  display.println("Hello from ESP8266");
  display.display();
}
// Hàm hiện thị ra OLED

void displayData(String data) {
  
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(3, 20);
  display.print(data);
  display.display();

}
//*****************************************************************************************//
void loop() {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Nếu không có thẻ mới thì quay lại loop
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards.Nếu đọc dữ liệu fail thì quay lại loop
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //Hiển thị thông tin chi tiết về thẻ RFID đã được phát hiện lên cổng Serial, 

  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

  //-------------------------------------------

  Serial.print(F("Name: "));

  byte buffer1[18];

  block = 4;
  len = 18;

  //------------------------------------------- GET FIRST NAME
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  /*
  mfrc522.PCD_Authenticate: Đây là lời gọi hàm để bắt đầu quá trình xác thực. Phương thức này thuộc về lớp MFRC522 và thực hiện việc xác thực thẻ RFID.

(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)): Các tham số truyền vào hàm:

MFRC522::PICC_CMD_MF_AUTH_KEY_A: Chỉ định lệnh xác thực với khóa A.
4: Chỉ định khối (block) cần xác thực trên thẻ RFID. Trong trường hợp này, là khối thứ 4.
&key: Địa chỉ của biến chứa thông tin về khóa (key) sẽ được sử dụng trong quá trình xác thực.
&(mfrc522.uid): Địa chỉ của biến chứa UID (Unique Identifier) của thẻ RFID. Thông tin này cũng được sử dụng trong quá trình xác thực.
*/
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    // return;
  }
  else {
      status = mfrc522.MIFARE_Read(block, buffer1, &len);
      /*
      mfrc522.MIFARE_Read: Đây là lời gọi hàm để bắt đầu quá trình đọc dữ liệu từ thẻ RFID. Phương thức này thuộc về lớp MFRC522 và được sử dụng để đọc dữ liệu từ một khối trên thẻ MIFARE.

      (block, buffer1, &len): Các tham số truyền vào hàm:

      block: Chỉ định khối (block) cần đọc dữ liệu từ trên thẻ RFID.
      buffer1: Địa chỉ của bộ đệm (buffer) mà dữ liệu sẽ được sao chép vào.
      &len: Địa chỉ của biến chứa độ dài tối đa của dữ liệu bạn muốn đọc. Sau khi hoàn thành, biến này sẽ chứa số byte thực sự đã đọc được từ thẻ.
      Sau khi thực hiện lời gọi hàm này, dữ liệu từ khối trên thẻ RFID sẽ được sao chép vào bộ đệm buffer1, và mã trạng thái của quá trình đọc sẽ được lưu vào biến status. Bạn có thể kiểm tra giá trị của status để xác định xem quá trình đọc đã thành công hay không.
      */
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  Serial.print("UID của thẻ: ");   
  for (byte i = 0; i < mfrc522.uid.size; i++) { 
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");   
    UID[i] = mfrc522.uid.uidByte[i];
    Serial.print(UID[i]);
  }
  if (UID[0] == ID1[0] && UID[1] == ID1[1] && UID[2] == ID1[2] && UID[3] == ID1[3]) {
    dem ++;
    if ((dem % 2) == 1) {
      MyServo.write(90);
    } else {
      MyServo.write(-90);
    }
  }
  /*
  Check uid của thẻ có trùng với id set sẵn không. Nếu trùng thì dem=1 tức là quay sang 1 hướng góc 90 , nếu ko thì quay sang hướng còn lại
  */
  String buffer1String = "";
  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
    }
    if ((buffer1[i] >= 32 && buffer1[i] <= 126) && buffer1[i] != 32){
      buffer1String += (char)buffer1[i];
    }
  }
  /*
  Xử lý thông tin từ buffer1 để tạo ra một chuỗi tên (buffer1String) và in ra nó trên cổng Serial, loại bỏ các khoảng trắng và các ký tự không hiển thị.
  */
  buffer1String += " ";
  Serial.print(" ");
  //---------------------------------------- GET LAST NAME

  byte buffer2[18];
  block = 1;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  //PRINT LAST NAME
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i] );
     if ((buffer2[i] >= 32 && buffer2[i] <= 126) && buffer2[i] != 32){
      buffer1String += (char)buffer2[i];
    }
  }
  // buffer1String += String((char*)buffer2);
  // Check if the name has changed
  if (buffer1String != previousName) {
    // Update OLED only if the name is different
    previousName = buffer1String;  // Update the previous name
    displayData(previousName);
    // MyServo.write(90);
    Serial.println(previousName);
  }


  //----------------------------------------

  Serial.println(F("\n**End Reading**\n"));

  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();//Gửi lệnh "halt" tới thẻ đang được đọc, đánh dấu kết thúc quá trình truy cập thẻ hiện tại.
  mfrc522.PCD_StopCrypto1();//: Dừng quá trình mã hóa trên thẻ. Điều này quan trọng để đảm bảo sự an toàn trong quá trình sử dụng thẻ RFID.
  }

  
}
//*****************************************************************************************//
