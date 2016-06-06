/*
Diseño de sensor de temperatura y humedad relativa con DHT11
             -Dickson Chacon C.I 9.028.201
             -Pierina Contreras C.I 18.391.229
             -Yessica Besson C.I 19.865.410
             -Lucy Velasco C.I 18.878.093
             -Erwin Sanchez C.I 19.253.159
*/
char GLCD_DataPort at PORTD;

sbit GLCD_CS1 at RB5_bit;
sbit GLCD_CS2 at RB6_bit;
sbit GLCD_RS  at RB4_bit;
sbit GLCD_RW  at RB3_bit;
sbit GLCD_EN at RB2_bit;
sbit GLCD_RST at RB7_bit;

sbit GLCD_CS1_Direction at TRISB5_bit;
sbit GLCD_CS2_Direction at TRISB6_bit;
sbit GLCD_RS_Direction at TRISB4_bit;
sbit GLCD_RW_Direction at TRISB3_bit;
sbit GLCD_EN_Direction at TRISB2_bit;
sbit GLCD_RST_Direction at TRISB7_bit;

sbit Data at RA0_bit;
sbit DataDir at TRISA0_bit;
char message1[] = "Temp = ";
char message2[] = "Hum = ";
unsigned short TOUT = 0, CheckSum, i;
unsigned short T_Byte1, T_Byte2, RH_Byte1, RH_Byte2;

//DHT11 DEFINITONS
sbit DHT11_Pin at PORTB.B0;
sbit DHT11_DIR at TRISB.B0;
extern unsigned int DHT11_TMP;
extern unsigned int DHT11_HUM;
extern char DHT11_CHKSM;
//END OF DHT11 DEFINITONS

long veri;
int isi,nem;
char bekleme=0;
char txt[7];


// ------------------------------------------------------
// GLCD Picture name: electrocare2.4.bmp
// GLCD Model: KS0108 128x64
// ------------------------------------------------------

const code char electrocare[1024] = {
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 192, 192, 224, 224, 224, 112, 112, 112, 112,  48,  48,  48,  48,  48, 112, 112, 112,  96, 224, 224, 192, 192, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0, 192, 224, 248, 124,  62, 142,  71,  39, 147, 201, 101, 100, 114,  58, 186, 153,  89,  93,  93,  93,  93,  93, 153, 153, 186,  50, 114, 228, 237, 201, 147,  39,  15,  30,  60,  56, 112, 224, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0, 248, 255, 127,   7, 225,  28,   3, 120, 124, 127, 115,  97,  64,  64,  96,  96, 112, 126, 126, 126,  63,  63,  62, 126, 254, 254, 252, 249, 242, 196,  25, 227,   7,  63, 254, 248, 248, 240, 248, 252, 255, 254, 240,   0,   0, 128, 128, 128,   0,   0, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,   0,   0, 128, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0,   0,   0, 128,   0,   0,   0,
  0,   0, 255, 255, 248,   0,  63, 194,   2, 122, 250, 250,  58,  26,  26,  26,  26,  58, 122, 250, 248, 240, 224, 224, 224, 240, 248, 255, 255, 255,  63,  31, 192,  63, 128, 224, 255, 255, 255, 255, 255, 255, 255, 255,  63,   0,  63,  36,  36,  32,   0,   0,  63,   0,  28,  42,  42,  46,   0,   0,  62,  34,  34,   0,   2,  63,  34,   0,   0,  62,   2,   0,  28,  34,  34,  34,  28,   0,  62,   2,   2,  62,   0,   0,  62,   0,   0,  31,  32,  32,  32,   0,   0,  58,  42,  62,   0,   0,  62,   2,   2,   0,  60,  42,  42,  44,   0,   0,  32,   0,   0,   0,   0,  30,  33,  32,  32,  33,   0,  32,   0,   0,  48,  14,   1,  15,  56,   0,
  0,   0,   0,   3,  15,  63, 124, 241, 226, 204, 145,  39,  76, 152, 144,  48, 112,  96, 100, 233, 203, 203, 203, 203, 203, 203, 203, 233, 100, 116, 114,  57, 156, 206,  67,   1, 129, 193, 193, 225, 127,  31,  15,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   1,   3,   7,  15,  14,  30,  28,  57,  57,  58,  50, 114, 116, 116, 116, 116, 116, 116, 116, 114,  50,  58,  57,  29,  28,  14,  14,   7,   7,   3,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
void main()
{
  ADCON1=0B1110;//Todos los pines diogitales
  Adc_init();
  DHT11_init();
  Glcd_Init(); //Inicialización del display gráfico.
  delay_ms(6000);
  Glcd_fill(0x00);                                  //Limpiar Pantalla
  while(1)
  {
   Dht11_Start();
   DHT11_Read();
   IntToStr(DHT11_TMP,txt);
   Glcd_Write_Text(txt, 10, 5, 1); // tmp
   IntToStr(DHT11_HUM,txt);
   Glcd_Write_Text(txt, 10, 4, 1); // hum
   delay_ms(1000);
  }
 }
/*
void main()
{
  ADCON1=0B1110;//Todos los pines diogitales
  Adc_init();
  DHT11_init();
  Glcd_Init(); //Inicialización del display gráfico.
  Glcd_Image(electrocare);
  delay_ms(6000);
  Glcd_fill(0x00);                                  //Limpiar Pantalla
  Glcd_Write_Text("Dickson Chacon", 10, 1, 1);
  Glcd_Write_Text("Pierina Contreras", 10, 2, 1);
  Glcd_Write_Text("Yessica Besson", 10, 3, 1);
  Glcd_Write_Text("Lucy Velasco", 10, 4, 1);
  Glcd_Write_Text("Erwin Sanchez", 10, 5, 1);
  delay_ms(11000);
  Glcd_fill(0x00);                                 //Limpiar Pantalla

  while(1)
  {
   Dht11_Start();
   DHT11_Read();

   if(DHT11_CHKSM==((DHT11_TMP>>8)+(DHT11_HUM>>8)+(DHT11_TMP&0xff)+(DHT11_HUM&0xff)))
   {
    IntToStr(DHT11_TMP>>8,txt);
    Glcd_Write_Text(txt, 10, 5, 1); // tmp
    IntToStr(DHT11_HUM>>8,txt);
    Glcd_Write_Text(txt, 10, 4, 1); // hum
  }
 }
}
//end
sbit Data at RA0_bit;
sbit DataDir at TRISA0_bit;
char message1[] = "Temperatura = 00.0 C";
char message2[] = "Humedad   = 00.0 %";
unsigned short TOUT = 0, CheckSum, i;
unsigned short T_Byte1, T_Byte2, RH_Byte1, RH_Byte2;

void StartSignal(){
  DataDir = 0;     // Data port is output
  Data    = 0;
  Delay_ms(25);    // Low for at least 18us
  Data    = 1;
  Delay_us(30);    // High for 20-40 us
  DataDir = 1;     // Data port is input

}

unsigned short CheckResponse(){
  TOUT = 0;
  TMR2 = 0;
  T2CON.TMR2ON = 1;      // Start TMR2 while waiting for sensor response
  while(!Data && !TOUT); // If there's no response within 256us, the Timer2 overflows
  if (TOUT) return 0;    // and exit
  else {
   TMR2 = 0;
   while(Data && !TOUT);
   if (TOUT) return 0;
   else {
    T2CON.TMR2ON = 0;
    return 1;
   }
  }
}

unsigned short ReadByte(){
  unsigned short num = 0, t;
  DataDir = 1;
  for (i=0; i<8; i++){
   while(!Data);
   TMR2 = 0;
   T2CON.TMR2ON = 1;  // Start TMR2 from 0 when a low to high data pulse
   while(Data);       // is detected, and wait until it falls low again.
   T2CON.TMR2ON = 0;  // Stop the TMR2 when the data pulse falls low.
   if(TMR2 > 40) num |= 1<<(7-i);  // If time > 40us, Data is 1
  }
  return num;
}

void interrupt(){
  if(PIR1.TMR2IF){
   TOUT = 1;
   T2CON.TMR2ON = 0; // stop timer
   PIR1.TMR2IF  = 0; // Clear TMR0 interrupt flag
  }
}

void main() {
  unsigned short check;
  TRISB = 0b00000000;
  PORTB = 0;
  TRISA = 0b00100001;
  CMCON = 7;
  INTCON.GIE = 1;    //Enable global interrupt
  INTCON.PEIE = 1;   //Enable peripheral interrupt
  // Configure Timer2 module
  PIE1.TMR2IE = 1;  // Enable Timer2 interrupt
  T2CON = 0;        // Prescaler 1:1, and Timer2 is off initially
  PIR1.TMR2IF =0;   // Clear TMR INT Flag bit
  TMR2 = 0;
  Lcd_Init();
  Lcd_Cmd(_Lcd_Clear);
  Lcd_Cmd(_LCD_CURSOR_OFF);

  do {
    Delay_ms(1000);
    StartSignal();
    check = CheckResponse();
    if (!check) {
     Lcd_Cmd(_Lcd_Clear);
     Lcd_Out(1, 1, "No response");
     Lcd_Out(2, 1, "from the sensor");
    }
    else{

     RH_Byte1 = ReadByte();
     RH_Byte2 = ReadByte();
     T_Byte1 = ReadByte();
     T_Byte2 = ReadByte();
     CheckSum = ReadByte();
     // Check for error in Data reception
     if (CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF))
     {
      message1[7]  = T_Byte1/10 + 48;
      message1[8]  = T_Byte1%10 + 48;
      message1[10] = T_Byte2/10 + 48;
      message2[7]  = RH_Byte1/10 + 48;
      message2[8]  = RH_Byte1%10 + 48;
      message2[10] = RH_Byte2/10 + 48;
      message1[11] = 223;     // Degree symbol
      Lcd_Cmd(_Lcd_Clear);
      Lcd_Out(1, 1, message1);
      Lcd_Out(2, 1, message2);
      }

      else{
       Lcd_Cmd(_Lcd_Clear);
       Lcd_Out(1, 1, "Checksum Error!");
       Lcd_Out(2, 1, "Trying Again ...");
      }
    }

  }while(1);
}*/