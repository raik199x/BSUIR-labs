#include <stdio.h>
#include <dos.h>
#include <conio.h>

int ConvertToDecimal(int BCD) {
    return ((BCD / 16 * 10) + (BCD % 16));
}
int ConvertToBCD(int decimal){
    return ((decimal / 10 * 16) + (decimal % 10));
}

//+++++++++++++++++++++++++++++++++++++++++DATA SEG++++++++++++++++++
void interrupt (*oldfuncDelay)();// pointer to old interruption of delay
void interrupt (*oldfuncAlarm)();// pointer to old interruption of alarm
 char * days_of_week [] =                                         //+
{ "Sun\0", "Mon\0", "Tue\0",                                      //+
  "Wed\0","Thu\0","Fri\0","Sat\0"};                               //+
unsigned int DelayTime = 0; int alarmOn = 0;                      //+
char * months [] =                                                //+
{"Jan\0", "Feb\0", "Mar\0","Apr\0","May\0","Jun\0",               //+
"Jul\0", "Aug\0", "Sep\0", "Oct\0", "Nov\0", "Dec\0"};            //+
//----------------------------------------DATA SEG+++++++++++++++++++
void OutputCurrentDate(){
  int Param;
  //Year
  outp(0x70,0x09);
  Param = ConvertToDecimal(inp(0x71));
  printf("20");
  if(Param < 9)
    printf("0");
  printf("%d, ",Param);
  //Month
  outp(0x70,0x08);
  Param = ConvertToDecimal(inp(0x71)); --Param;
  printf("%s, ",months[Param]);
  //Day
  outp(0x70,0x06);
  Param = ConvertToDecimal(inp(0x71));
  printf("%s\n", days_of_week[Param]);
  return; 
}

void OutputCurrentTime(){
  int Param;
  //Hours
  outp(0x70,0x04);
  Param = ConvertToDecimal(inp(0x71));
  if(Param < 9)
    printf("0");
  printf("%d:",Param);
  //Minutes
  outp(0x70,0x02);
  Param = ConvertToDecimal(inp(0x71));
  if(Param < 9)
    printf("0");
  printf("%d:", Param);
  //Seconds
  outp(0x70,0x00);
  Param = ConvertToDecimal(inp(0x71));
  if(Param < 9)
    printf("0");
  printf("%d\n",Param);
  return;
}

void interrupt MyNewDelay(){
  DelayTime++;
  outp(0x70, 0x0C); // выбор адреса в памяти CMOS (запись)
  inp(0x71); // данные по этому адресу (запись/чтение)
  // посыл сигнала контроллерам прерываний об окончании прерывания
  outp(0x20, 0x20);
  outp(0xA0, 0x20);
}

void SetTimeDate(){
  int Param = -1;
  disable();
  system("cls");
  //Writting year
  outp(0x70,0x09);
  while(Param < 0 || Param > 99){
    printf("Enter your year (two digits): ");
    scanf("%d",&Param);
  }
  outp(0x71,ConvertToBCD(Param));
  Param = -1;
  //Writting Month
  outp(0x70,0x08);
  while(Param < 1 || Param > 12){
    printf("\nEnter Month: ");
    scanf("%d",&Param);
  }
  outp(0x71,ConvertToBCD(Param));
  Param = -1;
  //Writting day of week
  outp(0x70,0x06);
  while(Param < 1 || Param > 7){
    printf("Enter day of week (1-7): ");
    scanf("%d",&Param);
  }
  outp(0x71,ConvertToBCD(Param));
  Param = -1;
  //Writting hour
  outp(0x70,0x04);
  while(Param < 0 || Param > 23){
    printf("Enter hours (0-23): ");
    scanf("%d",&Param);
  }
  outp(0x71,ConvertToBCD(Param));
  //Writting Minutes
  Param = -1;
  outp(0x70,0x02);
  while(Param < 0 || Param > 59){
    printf("Enter Minutes (0-59): ");
    scanf("%d",&Param);
  }
  outp(0x71,ConvertToBCD(Param));
  Param = -1;
  //Writting Seconds
  outp(0x70,0x00);
  while(Param < 0 || Param > 59){
    printf("Enter seconds (0-59): ");
    scanf("%d",&Param);
  }
  outp(0x71,ConvertToBCD(Param));

  enable();
}

void SetDelay(){
  unsigned int newDelay;
  system("cls");
  printf("Enter new delay: ");
  scanf("%d",&newDelay);

  disable();
  oldfuncDelay = getvect(0x70);
  setvect(0x70,MyNewDelay);
  enable();

  // размаскировка линии сигнала запроса от ЧРВ
  // 0xA1 - новое значение счетчика для системного таймера
  outp(0xA1, inp(0xA1) & 0xFE); // 0xFE = 1111 1110
  // 0-й бит в 0 для разрешения прерывания от ЧРВ 

  outp(0x70, 0xB); // выбор регистра В
  outp(0x71, inp(0x71) | 0x40); // 0x40 = 0100 0000
  // 6-й бит регистра B установлен в 1 для периодического прерывания

  DelayTime = 0;
  while (DelayTime <= newDelay);
  setvect(0x70, oldfuncDelay);
  return;
}

unsigned int check;
void interrupt newAlarm(){ // новый обработчик прерываний будильника
	system("cls");
	alarmOn = 2;
    check = 0;
    //resseting alarm
    // проверка на наличие установленного будильника
    if (oldfuncAlarm == NULL)
        return;
    disable(); // запрет на прерывание

    // возврат старого прерывания
    setvect(0x4A, oldfuncAlarm);  // 0x4A - текущая дата и время в формате BCD
    outp(0xA1, (inp(0xA0) | 0x01)); // 0x01 - 0000 0001 (пересчет частоты прерывания)

    // проверка на доступность значений для чтения/записи
    do {
        outp(0x70, 0xA); // выбор регистра A
        check = inp(0x71) & 0x80;  // 0x80 - 1000 0000
        // 7-й бит в 1 для обновления времени
    } while (check);

    // запись нулевых значений в регистр будильника
    outp(0x70, 0x05); // 0x05 - часы
    outp(0x71, 0x00);

    outp(0x70, 0x03); // 0x03 - минуты
    outp(0x71, 0x00);

    outp(0x70, 0x01); // 0x01 - секунды
    outp(0x71, 0x00);

    outp(0x70, 0xB); // выбор регистра B
    outp(0x71, (inp(0x71) & 0xDF)); // 0xDF - 1101 1111
    // 5-й бит в 0 для запрета прерывания будильника

    enable(); // разрешение на прерывание
}

void SetAlarm(){
  int hour = -1,sec = -1,minutes = -1;
  check = 0;
  system("cls");
     //Writting hour
  outp(0x70,0x04);
  while(hour < 0 || hour > 23){
    printf("Enter hours for alarm (0-23): ");
    scanf("%d",&hour);
  }
  hour = ConvertToBCD(hour);
  //Writting Minutes
  outp(0x70,0x02);
  while(minutes < 0 || minutes > 59){
    printf("Enter Minutes for alarm (0-59): ");
    scanf("%d",&minutes);
  }
  minutes = ConvertToBCD(minutes);
  //Writting Seconds
  outp(0x70,0x00);
  while(sec < 0 || sec > 59){
    printf("Enter seconds for alarm (0-59): ");
    scanf("%d",&sec);
  }
  sec = ConvertToBCD(sec);
    disable(); // запрет на прерывание

    // проверка на доступность значений для чтения/записи
    do {
        outp(0x70, 0xA); // выбор регистра A
        check = inp(0x71) & 0x80; // 0x80 - 1000 0000
        // 7-й бит в 1 для обновления времени
    } while (check);

    // установка часов в регистр будильника
    outp(0x70, 0x05);
    outp(0x71, hour);

    // установка минут в регистр будильника
    outp(0x70, 0x03);
    outp(0x71, minutes);

    // установка секунд в регистр будильника
    outp(0x70, 0x01);
    outp(0x71, sec);

    outp(0x70, 0xB); // выбор регистра B
    outp(0x71, (inp(0x71) | 0x20)); // 0x20 - 0010 0000
    // 5-й бит регистра B установлен в 1 для разрешения прерывания будильника

    // переопределение прерывания будильника
    oldfuncAlarm = getvect(0x4A); // 0x4A - 1001 010 (обновление времени)
    setvect(0x4A, newAlarm); // 0x4A - текущая дата и время в формате BCD
    outp(0xA1, (inp(0xA0) & 0xFE)); // 0xFE - 1111 1110
    // 0-й бит в 0 для разрешения прерывания от ЧРВ

    enable(); // разрешение на прерывание
    alarmOn = 1;
}

int main(){
  while(1){
    system("cls");
    OutputCurrentTime();
    OutputCurrentDate();
    if(alarmOn == 2)
      printf("Alarm is ringing!\n");
    printf("1. Set time+date\n"
           "2. Set alarm\n"
           "3. Set Delay\n"
           "4. Quit Program\n");
    delay(500); // making a delay for user to press a key
    if(kbhit()) // if any key was presed
    switch(getch()){
      case '1': SetTimeDate(); break;
      case '2': SetAlarm(); break;
      case '3': SetDelay(); break;
      case '4': return 0;
      default: break;

    }
  }
  return 0;
}
