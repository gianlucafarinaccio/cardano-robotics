//-----------------------------------------------------------------------------
// Title:         i2c_Flex_LCD
// Description:   Driver for common LCD with 1/2/3 or 4 row modules using 
//                PCF8574T interface board with I2C protocol.
// ----------------------------------------------------------------------------
//                CCS Forum-Library    http://www.ccsinfo.com/forum/
//-----------------------------------------------------------------------------
//
// lcd_init() Must be called before any other function.
//
// lcd_putc(c) Will display c on the next position of the LCD.
// 
//     \f Clear LCD dispay
//
//     lcd_gotoxy(x,y) Set write position on LCD (upper left is 1,1)
//
//-----------------------------------------------------------------------------
// LCD pins D0-D3 are not used.
//-----------------------------------------------------------------------------
//
// Commment   : Control of a compatible LCD HITACHI from a bus I2C with
//              an EXPANDER of I/O with connection I2C. The tests of these
//              routines have been programmed using the IC PCF8574T of Phillips.
//              I used 4 bits mode programming. The 8 bits mode programming
//              is possible if you use 2 x PCF8574T.
//
// As defined in the following structure the pin connection is as follows:
//
//  PCF8574T     LCD
//  ========     ======
//     P0        RS
//     P1        RW
//     P2        Enable 
//     P3        Led Backlight
//     P4        D4
//     P5        D5
//     P6        D6
//     P7        D7
//
//-----------------------------------------------------------------------------
//Okkio!!! Questi moduli x Arduino hanno indirizzi diversi. Per gli ultimi
//moduli comprati al distributore di Monterotondo si ha questo indirizzo
#define LCD_ADDR       0x4E //I2C slave address for LCD module
//-----------------------------------------------------------------------------

#define ON             1
#define OFF            0
#define RS             0b00000001  //P0 - PCF8574T Pin connected to RS
#define RW             0b00000010  //P1 - PCF8574T Pin connected to RW
#define EN             0b00000100  //P2 - PCF8574T Pin connected to EN
#define BACKLIGHT_LED  0b00001000  //P3 - PCF8574T Pin connected to BACKLIGHT LED
// These are the line addresses for most 4x20 LCDs.
#define lcd_line_one   0x80   // LCD RAM address for line 1
#define lcd_line_two   0xC0   // LCD RAM address for line 2
#define lcd_line_three 0x94   // LCD RAM address for line 3
#define lcd_line_four  0xD4   // LCD RAM address for line 4
 
int8 lcd_line;
byte address;
int1 lcd_backlight=ON;

void i2c_send_nibble(unsigned char data){   
  i2c_start(Debug);
  delay_us(20);
  i2c_write(Debug,LCD_ADDR); //the slave addresse
  delay_us(20);
  i2c_write(Debug,data);
  delay_us(20);
  i2c_stop(Debug);
  delay_us(20);
}

void lcd_send_byte(unsigned char data){
  if(lcd_backlight) data=data|EN|BACKLIGHT_LED; else data=data|EN; //set pin EN
    i2c_send_nibble(data);
  data=data-4;       //toggle EN back to 0
  i2c_send_nibble(data);
}
   
void lcd_clear(){
  lcd_send_byte(0x00);
  lcd_send_byte(0x10);
  delay_ms(2);
}

void lcd_init(){
  delay_ms(200); //LCD power up delay       
//Request works on the command by set the RS = 0 R/W = 0 write
  lcd_send_byte(0x00);
  lcd_send_byte(0x10);
  lcd_send_byte(0x00);
  lcd_send_byte(0x00);
  lcd_send_byte(0x10);
//First state in 8 bit mode
  lcd_send_byte(0x30);
  lcd_send_byte(0x30);
//Then set to 4-bit mode
  lcd_send_byte(0x30);
  lcd_send_byte(0x20);
//mode 4 bits, 2 lines, characters 5 x 7 (28 h)
  lcd_send_byte(0x20);
  lcd_send_byte(0x80);
//no need cursor on (0Ch)
  lcd_send_byte(0x00);
  lcd_send_byte(0xC0);
//the cursor moves to the left (06 h)
  lcd_send_byte(0x00);
  lcd_send_byte(0x60);
//clears the display
  lcd_clear();
}

void lcd_gotoxy( byte x, byte y){     
 static char data;
  switch(y){
    case 1:  address= lcd_line_one;
             lcd_line=1;
             break;
    case 2:  address= lcd_line_two;
             lcd_line=2;
             break;
    case 3:  address= lcd_line_three;
             lcd_line=3;
             break;
    case 4:  address= lcd_line_four;    
             break;
    default: address= lcd_line_one;
             lcd_line=4;
             break; 
  }
  address+=x-1;
  data=address&0xF0;
  lcd_send_byte(data);
  data=address&0x0F;
  data=data<<4;
  lcd_send_byte(data);
}

//Display the character on LCD screen.
void LCD_PUTC(char in_data){
 char data;     
  switch(in_data){ 
    case '\f': lcd_clear()    ;  
               break; 
    case '\n': lcd_line++;
               lcd_gotoxy(1,lcd_line);
               break;
    case '\r':
              lcd_gotoxy(1, ++lcd_line);
              break;
/*
    case '\2': lcd_gotoxy(1,2);  
               break;
    case '\3': lcd_gotoxy(1,3);  
               break;
    case '\4': lcd_gotoxy(1,4);  
               break;
*/
    default: data=in_data&0xF0;
             data=data|RS; //set RS pin to 1
             lcd_send_byte(data);
             data=in_data&0x0F;
             data=data<<4;
             data=data|RS; //set RS pin to 1
             lcd_send_byte(data);
             break;
  }
}
