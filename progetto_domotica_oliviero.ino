#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] =  { 192,168,1,20 };

const int MAX_PAGENAME_LEN = 8; // max characters in page name 
char buffer[MAX_PAGENAME_LEN+1]; // additional character for terminating null

EthernetServer server(80);



void setup()
{
EthernetClient client;
  if (client.connect("dynupdate.no-ip.com", 80)) {
    Serial.println("connessione in corso...");
    client.println("GET /nic/update?hostname=olivierognn.ddns.net HTTP/1.0");
    client.println("Host: dynupdate.no-ip.com");
    client.println("Authorization: Basic b2xpdmllcm9nbm46Q2FsYWlvMDk="); 
    client.println("User-Agent: Arduino Sketch/1.0 user@host.com");
    client.println();
  } 
  else {
    Serial.println("connessione fallita");
  }
  
  Serial.begin(9600);
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  
  int pinLuci = 2;
  int pinCancello = 3;
  int pinPortone = 5;//sul pin 4 si accendeva per un secondo
  
  //i relais funzionano in logica invertita così li spengo subito
  digitalWrite(pinPortone, 1);
  digitalWrite(pinLuci, 1);
  digitalWrite(pinCancello, 1);
  

 
  
  Ethernet.begin(mac, ip);
  server.begin();
  digitalWrite(10,HIGH);
  delay(2000);
}

void loop()
{
  EthernetClient client = server.available();
  if (client) 
  {
    int type = 0;
    while (client.connected()) 
     {
        if (client.available()) 
        {
        // GET, POST, or HEAD
         memset(buffer,0, sizeof(buffer)); // clear the buffer | scrive l'array con 0
         if(client.readBytesUntil('/',buffer,sizeof(buffer)))
         { 
          if(strcmp(buffer,"POST ") == 0)  //cerca la scritta post http://ruturajv.wordpress.com/2005/12/25/http-post-request/
          {
            client.find("\n\r"); // skip to the body
            // find string starting with "pin", stop on first blank line
            // the POST parameters expected in the form pinDx=Y
            // where x is the pin number and Y is 0 for LOW and 1 for HIGH
            while(client.findUntil("pinD", "\n\r")){
              int pin = client.parseInt();       // the pin number
              int val = client.parseInt();       // 0 or 1
              pinMode(pin, OUTPUT);
              digitalWrite(pin, val);
              
            }
          }
          sendHeader(client,"Domotica");
          //create HTML button to control pin 2
          client.println("<h2 align='center'>Domotica Oliviero</h2>");
          client.println("<table width='600' height='300' align='center' border='0'>");


          //Cancello
            int pinLuci = 2;
            client.print("<tr><td width='90%'><h3>Luci</h3>");
            
            client.print(" </td>");
            client.print("<td>");
            client.print("<form action='/' method='POST'><p><input type='hidden' name='pinD");
            client.print(pinLuci);
            client.print("'");
            client.print(" value='0'><input type='submit' value='On'/></form>");
            client.println("</tr>");


            //Luci
             int pinCancello = 3;
            client.print("<tr><td width='90%'><h3>Cancello</h3>");
            
            client.print(" </td>");
            client.print("<td>");
            client.print("<form action='/' method='POST'><p><input type='hidden' name='pinD");
            client.print(pinCancello);
            client.print("'");
            client.print(" value='0'><input type='submit' value='On'/></form>");
            client.println("</tr>");


            //Portone
             int pinPortone = 4;
            client.print("<tr><td width='90%'><h3>Portone</h3>");
            
            client.print(" </td>");
            client.print("<td>");
            client.print("<form action='/' method='POST'><p><input type='hidden' name='pinD");
            client.print(pinPortone);
            client.print("'");
            client.print(" value='0'><input type='submit' value='On'/></form>");
            client.println("</tr>");

            


          /*
          for(int i=2;i<=3;i++)
           {
            client.print("<tr><td>digital pin ");
            client.print(i);
            client.print(" </td><td>");
            client.print("<form action='/' method='POST'><p><input type='hidden' name='pinD");
            client.print(i);
            client.print("'");
            client.println(" value='0'><input type='submit' value='Off'/></form>");
            client.print(" </td><td>");
            client.print("<form action='/' method='POST'><p><input type='hidden' name='pinD");
            client.print(i);
            client.print("'");
            client.print(" value='1'><input type='submit' value='On'/></form>");
            client.print(" </td><td>stato: ");
            if(digitalRead(i)==1)
               client.print("ON");
             else
               client.print("OFF");
            client.println("</td></tr>");
            }

            */
       /*   
          for(int i=5;i<10;i++)
           {
            client.print("<tr><td>Pulsante ");
            client.print(i);
            client.print(" </td><td>");
            client.print("<form action='/' method='POST'><p><input type='hidden' name='pinD");
            client.print(i);
            client.print("'");
            client.println(" value='0'><input type='submit' value='Off'/></form>");
            client.print(" </td><td>");
            client.print("<form action='/' method='POST'><p><input type='hidden' name='pinD");
            client.print(i);
            client.print("'");
            client.print(" value='1'><input type='submit' value='On'/></form>");
            client.print(" </td><td>stato: ");
            if(digitalRead(i)==1)
               client.print("ON");
             else
               client.print("OFF");
            client.println("</td></tr>");
            }
          */
          
          client.println("</table>");
          client.println("</body></html>");
          client.stop();


          delay(10);
          digitalWrite(pinLuci, 1);
          digitalWrite(pinCancello, 1);
          digitalWrite(pinPortone, 1);
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}

void sendHeader(EthernetClient client, char *title)
{
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.print("<!DOCTYPE html><head><title>");
  client.print(title);
  client.println("</title>");



  client.println("<style> input {color: green; font-weight: bold; font-size: 70px; width:200px; height:200px; text-transform: uppercase; }    h3{color: orangered; font-weight: bold; font-size: 60px;   }</style>");
  client.println("</head><body>");


  
}   